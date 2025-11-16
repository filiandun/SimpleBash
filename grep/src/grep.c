#include <stdlib.h>

#include "grep_config.h"
#include "grep_parser.h"
#include "grep_core.h"

int main(int argc, char* argv[]) {
    grep_config_t *config = grep_config_init();
    if (config == NULL) {
        fprintf(stderr, "[grep++][error][init] memory allocation failed\n");

        return EXIT_FAILURE;
    }
    
    grep_parser_status_t parser_status = grep_parser_run(argc, argv, config);
    if (parser_status.code != GREP_PARSER_SUCCESS) {
        fprintf(stderr, "[grep++][error][parsing] %s\n", parser_status.message);
        grep_config_free(config);

        return EXIT_FAILURE;
    }

    if (grep_run(config) > GREP_NO_MATCH) {
        fprintf(stderr, "[grep++][error][running]\n");
        grep_config_free(config);

        return EXIT_FAILURE;
    }

    grep_config_free(config);

    return EXIT_SUCCESS;
}