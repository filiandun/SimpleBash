#ifndef GREP_PARSER_H
#define GREP_PARSER_H

#include "grep_config.h"

typedef enum {
    GREP_PARSER_SUCCESS = 0,

    GREP_PARSER_ERROR_INVALID_OPT,
    GREP_PARSER_ERROR_INVALID_ARG,
    GREP_PARSER_ERROR_INVALID_STATE,

    GREP_PARSER_ERROR_NOT_INIT,
    GREP_PARSER_ERROR_NULL_PTR,
    GREP_PARSER_ERROR_ALLOC_MEM,

    GREP_PARSER_ERROR_RUNTIME
} grep_parser_status_code_t;

typedef struct {
    grep_parser_status_code_t code;
    const char *message;
} grep_parser_status_t;

grep_parser_status_t grep_parser_run(const int argc, char *argv[], grep_config_t *config);

#endif
