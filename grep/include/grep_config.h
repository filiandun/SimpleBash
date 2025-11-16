#ifndef GREP_CONFIG
#define GREP_CONFIG

#include "ptr_array.h"

#define SET_OPTION(var, flag) ((var) |= (flag))
#define CLEAR_OPTION(var, flag) ((var) &= ~(flag))
#define CHECK_OPTION(var, flag) ((var) & (flag))

typedef enum {
    OPTION_NONE = 0,

    OPTION_PATTERN = 1U << 0,                      // -e [pattern]
    OPTION_IGNORE_CASE = 1U << 1,                  // -i
    OPTION_INVERT_MATCH = 1U << 2,                 // -v
    OPTION_COUNT = 1U << 3,                        // -c
    OPTION_FILES_WITH_MATCHES = 1U << 4,           // -l
    OPTION_LINE_NUMBER= 1U << 5,                   // -n
    OPTION_NO_FILENAME = 1U << 6,                  // -h
    OPTION_NO_MESSAGES = 1U << 7,                  // -s
    OPTION_FILE = 1U << 8,                         // -f [file]
    OPTION_ONLY_MATCHING = 1U << 9,                // -o
} grep_options_t;

typedef struct {  
    grep_options_t options;

    ptr_array_t files;
    ptr_array_t patterns;
} grep_config_t;

grep_config_t *grep_config_init();
void grep_config_free(grep_config_t *config);

#endif