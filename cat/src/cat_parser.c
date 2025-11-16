#include "cat_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define AVAILABLE_OPTIONS "benstvET"

static int handle_opt(int found_opt, cat_config_t *config);
static int handle_positionals(const int argc, char *argv[], cat_config_t *config);
static int add_file(char *file, cat_config_t *config);

int cat_parser_run(int argc, char *argv[], cat_config_t *config) {
  if (argv == NULL || config == NULL) {
    fprintf(stderr, "[ERROR] [cat++] [parser] Null arguments\n");
    return -1;
  }

  if (argc == 1) {
      config->options = OPTION_NONE;
  } else {
      int found_opt = 0;

      const struct option long_options[] = {
        { "number-nonblank", no_argument, NULL, 'b'},
        { "number", no_argument, NULL, 'n' },
        { "squeeze-blank", no_argument, NULL, 's' },
        { NULL, 0, NULL, 0 }
      };

      while ((found_opt = getopt_long(argc, argv, AVAILABLE_OPTIONS, long_options, NULL)) != -1) {
          if (handle_opt(found_opt, config) != 0) return -1;
      }
      
      if (handle_positionals(argc, argv, config) != 0) return -1;
  }

  return 0;
}

static int handle_opt(int found_opt, cat_config_t *config) {
  switch (found_opt) {
    case 'b': SET_OPTION(config->options, OPTION_NUMBER_NONBLANK); break;
    case 'v': SET_OPTION(config->options, OPTION_SHOW_NONPRINTING); break;
    case 'e': SET_OPTION(config->options, OPTION_SHOW_ENDS | OPTION_SHOW_NONPRINTING); break;
    case 'n': SET_OPTION(config->options, OPTION_NUMBER); break;
    case 's': SET_OPTION(config->options, OPTION_SQUEEZE_BLANK); break;
    case 't': SET_OPTION(config->options, OPTION_SHOW_TABS | OPTION_SHOW_NONPRINTING); break;
    case 'E': SET_OPTION(config->options, OPTION_SHOW_ENDS); break;
    case 'T': SET_OPTION(config->options, OPTION_SHOW_TABS); break;
    case '?': 
      fprintf(stderr, "[ERROR] [cat++] [parser] Unknown option: -%c\n", (char)optopt);
      return -1;
  }
  return 0;
}

static int handle_positionals(const int argc, char *argv[], cat_config_t *config) {
  if (optind >= argc) {
    fprintf(stderr, "[ERROR] [cat++] [parser] No files specified\n");
    return -1;
  }

  for (int i = optind; i < argc; ++i) {
    if (add_file(argv[i], config) != 0) {
      fprintf(stderr, "[ERROR] [cat++] [parser] Failed to add file: %s\n", argv[i]);
      return -1;
    }
  }

  return 0;
}

static int add_file(char *file, cat_config_t *config) {
  if (file == NULL) return -1;
  if (ptr_array_add(&config->files, file) != PTR_ARRAY_SUCCESS) return -1;

  return 0;
}