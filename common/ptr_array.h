#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <stdio.h>

typedef enum {
    PTR_ARRAY_SUCCESS = 0,

    PTR_ARRAY_ERROR_INVALID_ARG,
    PTR_ARRAY_ERROR_NOT_INIT,
    PTR_ARRAY_ERROR_NULL_PTR,
    PTR_ARRAY_ERROR_INVALID_STATE,
    PTR_ARRAY_ERROR_ALLOC_MEM,
} ptr_array_error_t;

typedef struct {
    void** data;
    size_t length;

    size_t capacity;
} ptr_array_t;

ptr_array_t *ptr_array_init(size_t length);
ptr_array_error_t ptr_array_static_init(ptr_array_t *ptr_array, size_t length);

ptr_array_error_t ptr_array_resize(ptr_array_t *ptr_array, size_t new_capacity);

size_t ptr_array_get_length(const ptr_array_t *ptr_array);

void ptr_array_print(const ptr_array_t *ptr_array, const char *separator);

ptr_array_error_t ptr_array_add(ptr_array_t *ptr_array, void *elem);
// ptr_array_error_t ptr_array_remove(ptr_array_t *ptr_array, size_t index);

ptr_array_error_t ptr_array_get(const ptr_array_t *ptr_array, size_t index, void **ptr_elem);
// ptr_array_error_t ptr_array_set(ptr_array_t *ptr_array, size_t index, void *ptr_elem);

ptr_array_error_t ptr_array_destroy(ptr_array_t *array);
ptr_array_error_t ptr_array_static_destroy(ptr_array_t *ptr_array);


#endif