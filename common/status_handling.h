#ifndef STATUS_HANDLING_H
#define STATUS_HANDLING_H

#define ERROR_STR "[ERROR]"
#define WARN_STR "[WARN]"
#define INFO_STR "[INFO]"

typedef enum {
    STATUS_SUCCESS = 0,

    STATUS_ERR_NULL_PTR,
    STATUS_ERR_MEMORY,
    STATUS_ERR_INVALID_ARGUMENT,
    
    STATUS_ERR_FILE,
    STATUS_ERR_FILE_NOT_FOUND,
    STATUS_ERR_FILE_ACCESS_DENIED,
    STATUS_ERR_FILE_READ,
    STATUS_ERR_FILE_WRITE,
    
    STATUS_ERR_INVALID_OPTION,
    STATUS_ERR_MISSING_ARGUMENT,
    
    STATUS_ERR_SYSTEM,
    STATUS_ERR_RUNTIME,
    STATUS_ERR_UNKNOWN
} status_code_t;

typedef struct {
    status_code_t code;

    const char* type;
    
    const char *app;
    const char *module;

    const char *message;
    const char *detail;
} status_info_t;

const char* get_status_str(status_code_t code);
void print_status(const status_info_t *status);

#endif