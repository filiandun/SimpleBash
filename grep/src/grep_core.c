#include "grep_core.h"

#include <string.h>
#include <stdarg.h>

#include "file_io.h"
#include "grep_config.h"

#define REG_MATCH 0

static grep_status_t grep_process_file(const grep_config_t *grep_config, regex_t *regex, const char *file);

static grep_line_result_t grep_match_line(const char *line, regex_t *regex);
static grep_status_t grep_process_line(regex_t *regex, const char *line, const size_t count, const grep_config_t *grep_config, size_t line_count, size_t *matches_line_count, const char *file);

static void grep_output_summary(const grep_config_t *grep_config, const char *file, size_t matches_line_count);
static grep_status_t grep_output_line(const grep_config_t *grep_config, regex_t *regex, const char *line, const size_t count, size_t line_count, const char *file);
static void grep_output_prefix(const grep_config_t *grep_config, const char *file, size_t line_count);

static grep_status_t create_regex_from_patterns(grep_config_t *grep_config, regex_t *regex);
static void grep_error(const grep_config_t *grep_config, grep_status_t grep_status, const char *format_str, ...);

grep_status_t grep_run(grep_config_t *grep_config) {
    if (!grep_config) {
        grep_error(grep_config, GREP_ERROR_RUNTIME, "config is null");
        return GREP_ERROR_RUNTIME;
    }

    regex_t regex;
    if (create_regex_from_patterns(grep_config, &regex) != GREP_OK) {
        grep_error(grep_config, GREP_ERROR_REGEX, "error compile regex");
        return GREP_ERROR_REGEX;
    }

    grep_status_t final_status = GREP_NO_MATCH;
    for (size_t i = 0; i < ptr_array_get_length(&(grep_config->files)); ++i) {
        void *buff;
        if (ptr_array_get(&(grep_config->files), i, &buff) != PTR_ARRAY_SUCCESS) {
            return GREP_ERROR_RUNTIME;
        }
        
        const char* file = (char*)buff;
        grep_status_t pre_status = grep_process_file(grep_config, &regex, file);
        if (pre_status == GREP_OK) {
            final_status = GREP_OK;
        }
    }

    regfree(&regex);

    return final_status;
}


static grep_status_t grep_process_file(const grep_config_t *grep_config, regex_t *regex, const char *file) {
    size_t matches_line_count = 0;
    size_t line_count = 0;
    
    file_reader_t *file_reader = open_file(file);
    if (!file_reader) {
        grep_error(grep_config, GREP_ERROR_FILE, "could not open file '%s'", file);
        return GREP_ERROR_FILE;
    }
    
    char* line;
    size_t line_length;

    file_reader_error_t file_reader_status;
    while ((file_reader_status = read_next_line(file_reader, &line, &line_length)) == FILE_READER_SUCCESS) {
        line_count++;
        
        grep_status_t grep_status = grep_process_line(regex, line, line_length, grep_config, line_count, &matches_line_count, file);
        if (grep_status == GREP_ERROR_REGEX) {
            grep_error(grep_config, GREP_ERROR_REGEX, "error regex processing line '%ld' in file '%s'", line_count, file);
        }
        free(line);
    }

    if (file_reader_status != FILE_READER_ERROR_EOF) {
        grep_error(grep_config, GREP_ERROR_FILE, "error reading file '%s'", file);
        close_file(file_reader);

        return GREP_ERROR_FILE;
    }
    
    grep_output_summary(grep_config, file, matches_line_count);
    
    close_file(file_reader);

    return matches_line_count == 0 ? GREP_NO_MATCH : GREP_OK;
}

static grep_line_result_t grep_match_line(const char *line, regex_t *regex) {
    int reg_status = regexec(regex, line, 0, NULL, 0);
    
    if (reg_status == REG_MATCH) return GREP_LINE_MATCH;
    if (reg_status == REG_NOMATCH) return GREP_LINE_NO_MATCH;
    
    return GREP_LINE_ERROR;
}

static grep_status_t grep_process_line(regex_t *regex, const char *line, const size_t count, const grep_config_t *grep_config, size_t line_count, size_t *matches_line_count, const char *file) { 
    grep_line_result_t match_result = grep_match_line(line, regex);
    if (match_result == GREP_LINE_ERROR) return GREP_ERROR_REGEX;

    if (CHECK_OPTION(grep_config->options, OPTION_INVERT_MATCH)) match_result = (match_result == GREP_LINE_MATCH) ? GREP_LINE_NO_MATCH : GREP_LINE_MATCH; // -v
    
    if (match_result) {
        (*matches_line_count)++;
        if (!CHECK_OPTION(grep_config->options, OPTION_COUNT) && !CHECK_OPTION(grep_config->options, OPTION_FILES_WITH_MATCHES)) { // -c -l
            grep_output_line(grep_config, regex, line, count, line_count, file);
        }
    }
    
    return GREP_OK;
}

static void grep_output_summary(const grep_config_t *grep_config, const char *file, size_t matches_line_count) {
    if (CHECK_OPTION(grep_config->options, OPTION_FILES_WITH_MATCHES)) { // l
        if (matches_line_count > 0) {
            printf("%s\n", file);
        }
    } else if (CHECK_OPTION(grep_config->options, OPTION_COUNT)) { // -c
        if (CHECK_OPTION(grep_config->options, OPTION_NO_FILENAME)) { // -h
            printf("%zu\n", matches_line_count);
        } else if (ptr_array_get_length(&(grep_config->files)) > 1) {
            printf("%s:%zu\n", file, matches_line_count);
        } else {
            printf("%zu\n", matches_line_count);
        }
    }
}

static grep_status_t grep_output_line(const grep_config_t *grep_config, regex_t *regex, const char *line, const size_t count, size_t line_count, const char *file) {
    if (CHECK_OPTION(grep_config->options, OPTION_ONLY_MATCHING)) { // -o
        regmatch_t regmatch = {0};
        int offset = 0;
        
        while ((size_t)offset <= count) {
            int reg_status = regexec(regex, line + offset, 1, &regmatch, 0);
            
            if (reg_status == REG_NOMATCH) {
                return GREP_NO_MATCH;
            } else if (reg_status == REG_MATCH) {
                int start = offset + regmatch.rm_so;
                int end = offset + regmatch.rm_eo;
                
                grep_output_prefix(grep_config, file, line_count);
                printf("%.*s\n", end - start, line + start);
            } else {
                return GREP_ERROR_REGEX;
            }
            
            offset += regmatch.rm_eo;
            if (regmatch.rm_so == regmatch.rm_eo) offset++;
        }
    } else {
        grep_output_prefix(grep_config, file, line_count);
        printf("%s\n", line);
    }

    return GREP_OK;
}

static void grep_output_prefix(const grep_config_t *grep_config, const char *file, size_t line_count) {
    if (ptr_array_get_length(&(grep_config->files)) > 1 && !CHECK_OPTION(grep_config->options, OPTION_NO_FILENAME)) { // -h
        printf("%s:", file);
    }

    if (CHECK_OPTION(grep_config->options, OPTION_LINE_NUMBER)) { // -n
        printf("%zu:", line_count);
    }
}

static grep_status_t create_regex_from_patterns(grep_config_t *grep_config, regex_t *regex) {
    int reg_flags = REG_EXTENDED; // -i
    if (CHECK_OPTION(grep_config->options, OPTION_IGNORE_CASE)) reg_flags |= REG_ICASE;

    char combined_pattern[8192] = {0}; // TO DO динамически выделять память под паттерны
    if (ptr_array_get_length(&(grep_config->patterns)) == 1) {
        void *buff;
        if (ptr_array_get(&(grep_config->patterns), 0, &buff) != PTR_ARRAY_SUCCESS) return GREP_ERROR_RUNTIME;
        const char* pattern = (char*)buff;
        strcpy(combined_pattern, pattern);
    } else {
        for (size_t i = 0; i < ptr_array_get_length(&(grep_config->patterns)); ++i) {
            if (i > 0) strcat(combined_pattern, "|");
            void *buff;
            if (ptr_array_get(&(grep_config->patterns), i, &buff) != PTR_ARRAY_SUCCESS) return GREP_ERROR_RUNTIME;
            const char* pattern = (char*)buff;
            if (!pattern) continue;

            strcat(combined_pattern, "(");
            strcat(combined_pattern, pattern);
            strcat(combined_pattern, ")");
        }
    }

    int reg_status = regcomp(regex, combined_pattern, reg_flags);
    if (reg_status) {
        return GREP_ERROR_REGEX;
    }

    return GREP_OK;
}

static void grep_error(const grep_config_t *grep_config, grep_status_t grep_status, const char *format_str, ...) {
    if (CHECK_OPTION(grep_config->options, OPTION_NO_MESSAGES)) return;

    fprintf(stderr, "[s21_grep][error][%d]: ", grep_status);
    va_list args;
    va_start(args, format_str);
    vfprintf(stderr, format_str, args);
    va_end(args);

    fprintf(stderr, "\n");
}