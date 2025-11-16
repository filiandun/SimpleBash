#include "cat_core.h"

#include <stdio.h>

#include "file_io.h"

static int process_file(const char *file, int *line_number, int *is_new_line, int *is_prev_line_empty, const cat_config_t *config);
static void apply_cat_options(int ch, const cat_config_t* cat_config, int* is_new_line, int* line_number, int* is_prev_line_empty);
static void apply_option_num(int ch, const cat_config_t* cat_config, int* line_number);
static void apply_option_show_nonprinting(int ch);

int cat_run(cat_config_t* config) {
  if (config == NULL) {
    fprintf(stderr, "[ERROR] [cat++] [core] Null configuration\n");
    return -1;
  }

  int line_number = 1;
  int is_new_line = 1;
  int is_prev_line_empty;

  size_t file_count = ptr_array_get_length(&config->files);

  for (size_t i = 0; i < file_count; ++i) {
    if (process_file(config->files.data[i], &line_number, &is_new_line, &is_prev_line_empty, config) != 0) {
      return -1;
    }
  }

  return 0;
}

static int process_file(const char *file, int *line_number, int *is_new_line, int *is_prev_line_empty, const cat_config_t *config) {
  file_reader_t *file_reader = open_file(file);
  if (file_reader == NULL) {
    fprintf(stderr, "[ERROR] [cat++] [core] Cannot open file: %s\n", file);
    close_file(file_reader);
    return -1;
  }

  *is_prev_line_empty = 0;

  char ch = 0;
  do {
    read_next_ch(file_reader, &ch);
    if (ch == EOF) break;
    
    apply_cat_options(ch, config, is_new_line, line_number, is_prev_line_empty);
  } while (ch != EOF);

  close_file(file_reader);

  return 0;
}


static void apply_cat_options(int ch, const cat_config_t* cat_config, int* is_new_line, int* line_number, int* is_prev_line_empty) {
  if (*is_new_line) {
    if (CHECK_OPTION(cat_config->options, OPTION_SQUEEZE_BLANK) &&
        *is_prev_line_empty == 1 && ch == '\n')
      return;

    *is_prev_line_empty = (ch == '\n' ? 1 : 0);

    apply_option_num(ch, cat_config, line_number);
  }

  if (CHECK_OPTION(cat_config->options, OPTION_SHOW_ENDS) && ch == '\n') {
    printf("$\n");
  } else if (CHECK_OPTION(cat_config->options, OPTION_SHOW_TABS) && ch == '\t') {
    printf("^I");
  } else if (CHECK_OPTION(cat_config->options, OPTION_SHOW_NONPRINTING)) {
    apply_option_show_nonprinting(ch);
  } else {
    putchar(ch);
  }

  *is_new_line = (ch == '\n');
}

static void apply_option_num(int ch, const cat_config_t* cat_config, int* line_number) {
  if (CHECK_OPTION(cat_config->options, OPTION_NUMBER_NONBLANK)) {
    if (ch != '\n') {
      printf("%6d\t", (*line_number)++);
    }
  } else if (CHECK_OPTION(cat_config->options, OPTION_NUMBER)) {
    printf("%6d\t", (*line_number)++);
  }
}

static void apply_option_show_nonprinting(int ch) {
  if (ch < 32 && ch != '\n' && ch != '\t') {
    printf("^%c", ch + 64);
  } else if (ch == 127) {
    printf("^?");
  } else {
    putchar(ch);
  }
}