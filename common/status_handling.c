#include "status_handling.h"

#include <stdio.h>

const char* get_status_str(status_code_t code) {
    switch (code) {
        case STATUS_SUCCESS:                return "Success";

        case STATUS_ERR_NULL_PTR:           return "Null pointer";
        case STATUS_ERR_MEMORY:             return "Memory allocation failed";
        case STATUS_ERR_INVALID_ARGUMENT:   return "Invalid argument";

        case STATUS_ERR_FILE:               return "File error";
        case STATUS_ERR_FILE_NOT_FOUND:     return "File not found";
        case STATUS_ERR_FILE_ACCESS_DENIED: return "Access denied";
        case STATUS_ERR_FILE_READ:          return "Read error";
        case STATUS_ERR_FILE_WRITE:         return "Write error";

        case STATUS_ERR_INVALID_OPTION:     return "Invalid option";
        case STATUS_ERR_MISSING_ARGUMENT:   return "Missing argument";

        case STATUS_ERR_SYSTEM:             return "System error";
        case STATUS_ERR_RUNTIME:            return "Runtime error";
        case STATUS_ERR_UNKNOWN:            return "Unknown error";
        
        default:                            return "Undefined status code";
    }
}

void print_status(const status_info_t *status) {
    if (status == NULL) return;

    if (status->type != NULL) fprintf(stderr, "%s", status->type);
    fprintf(stderr, "[%d]", status->code);

    if (status->app != NULL) fprintf(stderr, "[%s]", status->app);
    if (status->module != NULL) fprintf(stderr, "[%s]", status->module);

    if (status->message != NULL) fprintf(stderr, " %s", status->message);
    if (status->detail != NULL) fprintf(stderr, " (%s)", status->detail);
    
    fprintf(stderr, "\n");
}