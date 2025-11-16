#include <stdio.h>
#include <stdlib.h>

#include "status_handling.h"

#include "cat_config.h"
#include "cat_parser.h"
#include "cat_core.h"

int main(int argc, char* argv[]) {
  cat_config_t* config = cat_config_init();
  if (config == NULL) {
    fprintf(stderr, "[ERROR] [cat++] [init] Memory allocation failed\n");
    return EXIT_FAILURE;
  }

  if (cat_parser_run(argc, argv, config) != 0) {
      cat_config_free(config);
      return EXIT_FAILURE;
  }

  if (cat_run(config) != 0) {
    cat_config_free(config);
    return EXIT_FAILURE;
  }

  cat_config_free(config);
  return EXIT_SUCCESS;
}