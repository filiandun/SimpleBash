#ifndef CAT_CONFIG_H
#define CAT_CONFIG_H

#include "ptr_array.h"

#define SET_OPTION(var, flag) ((var) |= (flag))
#define CLEAR_OPTION(var, flag) ((var) &= ~(flag))
#define CHECK_OPTION(var, flag) ((var) & (flag))

typedef enum {
    OPTION_NONE = 0,

    OPTION_NUMBER_NONBLANK = 1U << 0,              // -b
    OPTION_SHOW_ENDS = 1U << 1,                    // -e
    OPTION_NUMBER = 1U << 2,                       // -n
    OPTION_SQUEEZE_BLANK = 1U << 3,                // -s
    OPTION_SHOW_TABS = 1U << 4,                    // -t
    OPTION_SHOW_NONPRINTING = 1U << 5,             // -v
} cat_options_t;

typedef struct {
    cat_options_t options;

    ptr_array_t files;
} cat_config_t;

cat_config_t *cat_config_init();
void cat_config_free(cat_config_t *config);

#endif