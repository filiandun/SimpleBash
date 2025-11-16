#include "cat_config.h"

#include <stdlib.h>

#define DEFAULT_SIZE 10

cat_config_t *cat_config_init() {
    cat_config_t *new_config = malloc(sizeof(cat_config_t));
    if (new_config == NULL) return NULL;

    if (ptr_array_static_init(&new_config->files, DEFAULT_SIZE)  != PTR_ARRAY_SUCCESS) {
        free(new_config); 
        return NULL;
    }

    new_config->options = 0;

    return new_config;
}

void cat_config_free(cat_config_t *config) {
    if (config != NULL) {
        ptr_array_static_destroy(&config->files); 

        free(config); 
    }
}