#ifndef GREP_H
#define GREP_H

#include <regex.h>
#include <stdlib.h>

#include "grep_config.h"

typedef enum {
    GREP_LINE_NO_MATCH = 0,
    GREP_LINE_MATCH,

    GREP_LINE_ERROR = -1,
} grep_line_result_t;

typedef enum {
    GREP_OK = 0,
    GREP_NO_MATCH,

    GREP_ERROR_FILE,
    GREP_ERROR_REGEX,
    GREP_ERROR_RUNTIME
} grep_status_t;

grep_status_t grep_run(grep_config_t *grep_config);

#endif