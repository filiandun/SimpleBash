#include "ptr_array.h"

#include <stdlib.h>

ptr_array_t *ptr_array_init(size_t length) {    
    if (length == 0) length = 1;    

    ptr_array_t *new_dyn_array = malloc(sizeof(ptr_array_t));
    if (new_dyn_array == NULL) return NULL;

    new_dyn_array->data = calloc(sizeof(void*), (length * 2));
    if (new_dyn_array->data == NULL) {
        free(new_dyn_array);
        return NULL;
    }

    new_dyn_array->length = 0;
    new_dyn_array->capacity = length * 2;

    return new_dyn_array;
}

ptr_array_error_t ptr_array_static_init(ptr_array_t *ptr_array, size_t length) {
    if (ptr_array == NULL) return PTR_ARRAY_ERROR_NULL_PTR;
    if (length == 0) length = 1;    

    ptr_array->data = calloc(sizeof(void*), (length * 2));
    if (ptr_array->data == NULL) return PTR_ARRAY_ERROR_ALLOC_MEM;

    ptr_array->length = 0;
    ptr_array->capacity = length * 2;

    return PTR_ARRAY_SUCCESS;
}

ptr_array_error_t ptr_array_resize(ptr_array_t *ptr_array, size_t new_capacity) {
    if (ptr_array == NULL) return PTR_ARRAY_ERROR_NULL_PTR;
    if (new_capacity == 0) new_capacity = 1;

    void *buff_data = realloc(ptr_array->data, (sizeof(void*) * new_capacity));
    if (buff_data == NULL) return PTR_ARRAY_ERROR_ALLOC_MEM;

    ptr_array->data = buff_data;
    ptr_array->length = ptr_array->length > new_capacity ? new_capacity : ptr_array->length;
    ptr_array->capacity = new_capacity;

    return PTR_ARRAY_SUCCESS;
}

size_t ptr_array_get_length(const ptr_array_t *ptr_array) {
    if (ptr_array == NULL) return 0;
    
    return ptr_array->length;
}

void ptr_array_print(const ptr_array_t *ptr_array, const char *separator) {
    size_t length = ptr_array_get_length(ptr_array);
    for (size_t i = 0; i < length; ++i) {
        void* buff;
        ptr_array_get(ptr_array, i, &buff);
        char* str = (char*) buff;

        printf("[%ld] %s", i, str);
    
        if (i < length - 1) printf("%s", separator);
    }
}

ptr_array_error_t ptr_array_add(ptr_array_t *ptr_array, void *elem) {
    if (ptr_array == NULL) return PTR_ARRAY_ERROR_NULL_PTR;
    if (ptr_array->data == NULL) return PTR_ARRAY_ERROR_INVALID_STATE;

    if (ptr_array->length >= ptr_array->capacity - 1) {
        size_t new_capacity = ptr_array->capacity == 0 ? 1 : ptr_array->capacity * 2;
        if (ptr_array_resize(ptr_array, new_capacity) != PTR_ARRAY_SUCCESS) {
            return PTR_ARRAY_ERROR_ALLOC_MEM;
        }
    }

    ptr_array->data[ptr_array->length] = elem;
    ptr_array->length++;

    return PTR_ARRAY_SUCCESS;
}

// ptr_array_error_t ptr_array_remove(ptr_array_t *ptr_array, size_t index) {
//     return PTR_ARRAY_SUCCESS;
// }

ptr_array_error_t ptr_array_get(const ptr_array_t *ptr_array, size_t index, void **out_elem) {
    if (ptr_array == NULL) return PTR_ARRAY_ERROR_NULL_PTR;
    if (out_elem == NULL) return PTR_ARRAY_ERROR_NULL_PTR;

    if (ptr_array->data == NULL) return PTR_ARRAY_ERROR_INVALID_STATE;

    if (index >= ptr_array->length) return PTR_ARRAY_ERROR_INVALID_ARG;

    *out_elem = ptr_array->data[index];

    return PTR_ARRAY_SUCCESS;
}

// ptr_array_error_t ptr_array_set(ptr_array_t *ptr_array, size_t index, void *out_elem) {
//     return PTR_ARRAY_SUCCESS;
// }

ptr_array_error_t ptr_array_destroy(ptr_array_t *ptr_array) {
    if (ptr_array == NULL) return PTR_ARRAY_ERROR_NULL_PTR;

    free(ptr_array->data);
    free(ptr_array);

    ptr_array = NULL;

    return PTR_ARRAY_SUCCESS;
}

ptr_array_error_t ptr_array_static_destroy(ptr_array_t *ptr_array) {
    if (ptr_array == NULL) return PTR_ARRAY_ERROR_NULL_PTR;

    free(ptr_array->data);

    ptr_array->data = NULL;
    ptr_array->capacity = 0;
    ptr_array->length = 0;   

    return PTR_ARRAY_SUCCESS;
}