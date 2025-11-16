#include "grep_config.h"

#include <stdlib.h>

#define DEFAULT_SIZE 10

grep_config_t *grep_config_init() {
    grep_config_t *new_config = malloc(sizeof(grep_config_t));
    if (new_config == NULL) return NULL;

    if (ptr_array_static_init(&new_config->files, DEFAULT_SIZE)  != PTR_ARRAY_SUCCESS) {
        free(new_config); 
        return NULL;
    }

    if (ptr_array_static_init(&new_config->patterns, DEFAULT_SIZE) != PTR_ARRAY_SUCCESS) {
        ptr_array_destroy(&new_config->files); 
        free(new_config); 
        return NULL;
    }

    new_config->options = 0;

    return new_config;
}

void grep_config_free(grep_config_t *config) {
    if (config != NULL) {
        ptr_array_static_destroy(&config->files); 
        ptr_array_static_destroy(&config->patterns);

        free(config); 
    }
}