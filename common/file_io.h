#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_SIZE (10 * 1024 * 1024)
#define MAX_LINE_LENGTH (64 * 1024)

typedef enum {
    FILE_READER_SUCCESS = 0,

    FILE_READER_ERROR_NOT_INIT,
    FILE_READER_ERROR_FILE_TOO_LARGE,
    FILE_READER_ERROR_LINE_TOO_LARGE,

    FILE_READER_ERROR_OPEN,
    FILE_READER_ERROR_READ,
    FILE_READER_ERROR_EOF,

    FILE_READER_ERROR_NOT_FOUND,
    FILE_READER_ERROR_ACCESS,

    FILE_READER_ERROR_NULL_PTR,
    FILE_READER_ERROR_INVALID_STATE,

    FILE_READER_ERROR_ALLOC_MEM,
} file_reader_error_t;

typedef struct {
    FILE* file;
} file_reader_t;

file_reader_t *open_file(const char *file);

file_reader_error_t read_all_file(const file_reader_t *file_reader, char **out_content, size_t *out_count);
file_reader_error_t read_next_line(file_reader_t *file_reader, char **out_line, size_t *out_count);
file_reader_error_t read_next_ch(file_reader_t *file_reader, char *out_ch);

size_t get_file_size(const file_reader_t *file_reader);

int print_file(file_reader_t *file_reader);

const char *file_io_error_str(file_reader_error_t file_reader_error);

file_reader_t *close_file(file_reader_t *file_reader);

#endif