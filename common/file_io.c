#include "file_io.h"

file_reader_t *open_file(const char* file) {
    if (!file) return NULL;

    file_reader_t *new_file_reader = malloc(sizeof(file_reader_t));
    if (!new_file_reader) return NULL;

    new_file_reader->file = fopen(file, "r");
    if (!new_file_reader->file) {
        free(new_file_reader);
        return NULL;
    }

    return new_file_reader;
}

file_reader_error_t read_all_file(const file_reader_t *file_reader, char **out_content, size_t *out_count) {
    if (!file_reader) return FILE_READER_ERROR_NOT_INIT;
    if (!out_content) return FILE_READER_ERROR_NULL_PTR;
    if (!out_count) return FILE_READER_ERROR_NULL_PTR;

    if (!file_reader->file) return FILE_READER_ERROR_INVALID_STATE;
    
    size_t file_size = get_file_size(file_reader);
    if (file_size > MAX_FILE_SIZE) return FILE_READER_ERROR_FILE_TOO_LARGE;

    *out_count = file_size + 1;
    *out_content = malloc(*out_count);
    if (!*out_content) return FILE_READER_ERROR_ALLOC_MEM;
    
    size_t readed_bytes = fread(*out_content, 1, file_size, file_reader->file);
    if (readed_bytes != file_size) {
        free(*out_content);
        *out_content = NULL;
        return FILE_READER_ERROR_READ;
    }

    (*out_content)[file_size] = '\0';

    return FILE_READER_SUCCESS;
}

file_reader_error_t read_next_line(file_reader_t *file_reader, char **out_line, size_t *out_count) { 
    if (!file_reader) return FILE_READER_ERROR_NOT_INIT;
    if (!out_line) return FILE_READER_ERROR_NULL_PTR;
    if (!out_count) return FILE_READER_ERROR_NULL_PTR;

    if (!file_reader->file) return FILE_READER_ERROR_INVALID_STATE;

    size_t allocated_memory = 256;
    *out_line = malloc(allocated_memory);
    if (!*out_line) return FILE_READER_ERROR_ALLOC_MEM;

    *out_count = 0;

    int ch;
    while ((ch = fgetc(file_reader->file)) != EOF && ch != '\n') {
        if (*out_count >= MAX_LINE_LENGTH) {
            free(*out_line);
            return FILE_READER_ERROR_LINE_TOO_LARGE;
        }
        if (*out_count >= allocated_memory - 1) {
            char* buff_ptr_line = realloc(*out_line, allocated_memory *= 2);
            if (!buff_ptr_line) {
                free(*out_line);
                return FILE_READER_ERROR_ALLOC_MEM;
            }
            *out_line = buff_ptr_line;
        }
        (*out_line)[*out_count] = ch;
        (*out_count)++;
    }

    if (ch == EOF && *out_count == 0) {
        free(*out_line);
        *out_line = NULL;
        return FILE_READER_ERROR_EOF;
    }

    (*out_line)[*out_count] = '\0';

    return FILE_READER_SUCCESS;
}

file_reader_error_t read_next_ch(file_reader_t *file_reader, char *out_ch) {
    if (!file_reader) return FILE_READER_ERROR_NOT_INIT;
    if (!out_ch) return FILE_READER_ERROR_NULL_PTR;

    if (!file_reader->file) return FILE_READER_ERROR_INVALID_STATE;

    *out_ch = fgetc(file_reader->file);

    if (*out_ch == EOF) return FILE_READER_ERROR_EOF;

    return FILE_READER_SUCCESS;
}

size_t get_file_size(const file_reader_t *file_reader) {
    if (!file_reader) return 0;

    fseek(file_reader->file, 0, SEEK_END);
    size_t file_size = ftell(file_reader->file);
    fseek(file_reader->file, 0, SEEK_SET);

    return file_size;
}

int print_file(file_reader_t *file_reader) {
    if (!file_reader || !file_reader->file) return FILE_READER_ERROR_NOT_INIT;

    int buffer_length = 256;
    char buffer[buffer_length];

    while (fgets(buffer, buffer_length, file_reader->file)) {
        printf("%s", buffer);
    }
    
    return FILE_READER_SUCCESS;
}


const char* file_io_error_str(file_reader_error_t file_reader_error) {
    switch (file_reader_error) {
        case FILE_READER_SUCCESS: return "success";

        case FILE_READER_ERROR_NOT_INIT: return "file reader is not init";

        case FILE_READER_ERROR_OPEN: return "failed to open file";
        case FILE_READER_ERROR_READ: return "read error";
        case FILE_READER_ERROR_EOF: return "end of file";

        case FILE_READER_ERROR_NOT_FOUND: return "file not found";
        case FILE_READER_ERROR_ACCESS: return "no access to file";

        case FILE_READER_ERROR_ALLOC_MEM: return "error to alloc memory";

        default: return "unknown error";
    }
}

file_reader_t *close_file(file_reader_t *file_reader) {
    if (file_reader) {
        fclose(file_reader->file);
        free(file_reader);
    }

    return NULL;
}