#include "grep_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "file_io.h"

#define AVAILABLE_OPTIONS "e:ivclnhsf:o"

typedef enum {
    _GP_SUCCESS = 0,

    _GP_ERR_RUNTIME,

    _GP_ERR_FILE,
    _GP_ERR_OPT_ARG_NULL,
    _GP_ERR_INVALID_OPT,
    _GP_ERR_PATTERN_NOT_SET,
    _GP_ERR_FILE_NOT_SET,
} _gp_internal_status; 
// TODO убрать, так как излишняя сложность. Все это можно держать общем grep_parser_status_t.
// Также убрать структуру и оставить просто код ошибки, который и возвращать, но generate error оставить и сделать публичным, чтобы можно было по коду текст ошибки получить.

static _gp_internal_status handle_opt(int found_opt, grep_config_t *config);
static _gp_internal_status handle_positionals(const int argc, char *argv[], grep_config_t *config);

static _gp_internal_status add_pattern(char *pattern, grep_config_t *config);
static _gp_internal_status add_file(char *file, grep_config_t *config);

static _gp_internal_status read_patterns_from_file(const char *file, grep_config_t *config);

static const char *get_error_message(_gp_internal_status code);
static grep_parser_status_t generate_error(grep_parser_status_code_t code, const char *message); // TODO переписать под форматированную строку


grep_parser_status_t grep_parser_run(const int argc, char *argv[], grep_config_t* config) {
    if (argv == NULL || config == NULL) return generate_error(GREP_PARSER_ERROR_NULL_PTR, "some args is null"); 

    config->options = OPTION_NONE;

    if (argc == 1) {
        config->options = OPTION_NONE;
    } else {
        int found_opt = 0;
        while ((found_opt = getopt(argc, argv, AVAILABLE_OPTIONS)) != -1) {
            _gp_internal_status internal_status = handle_opt(found_opt, config);
            if (internal_status != _GP_SUCCESS) return generate_error(GREP_PARSER_ERROR_RUNTIME, get_error_message(internal_status));
        }

        _gp_internal_status internal_status = handle_positionals(argc, argv, config);
        if (internal_status != _GP_SUCCESS) return generate_error(GREP_PARSER_ERROR_RUNTIME, get_error_message(internal_status));
    }

    return generate_error(GREP_PARSER_SUCCESS, "success");
}

static _gp_internal_status handle_opt(int found_opt, grep_config_t *config) {
    switch (found_opt) {
        case 'e': SET_OPTION(config->options, OPTION_PATTERN); return add_pattern(optarg, config);
        case 'i': SET_OPTION(config->options, OPTION_IGNORE_CASE); break;
        case 'v': SET_OPTION(config->options, OPTION_INVERT_MATCH); break;
        case 'c': SET_OPTION(config->options, OPTION_COUNT); break;
        case 'l': SET_OPTION(config->options, OPTION_FILES_WITH_MATCHES); break;
        case 'n': SET_OPTION(config->options, OPTION_LINE_NUMBER); break;
        case 'h': SET_OPTION(config->options, OPTION_NO_FILENAME); break;
        case 's': SET_OPTION(config->options, OPTION_NO_MESSAGES); break;
        case 'f': SET_OPTION(config->options, OPTION_FILE); return read_patterns_from_file(optarg, config);
        case 'o': SET_OPTION(config->options, OPTION_ONLY_MATCHING); break;
        case '?': return _GP_ERR_INVALID_OPT;
    }
    return _GP_SUCCESS;
}

static _gp_internal_status handle_positionals(const int argc, char *argv[], grep_config_t *config) {
    if (!CHECK_OPTION(config->options, OPTION_PATTERN) && !CHECK_OPTION(config->options, OPTION_FILE)) {
        if (optind < argc) {
            add_pattern(argv[optind++], config);
        } else {
            return _GP_ERR_PATTERN_NOT_SET;
        }
    }

    if (optind < argc) {
        for (int i = optind; i < argc; ++i) {
            add_file(argv[i], config);
        }
    } else {
        return _GP_ERR_FILE_NOT_SET;
    }

    return _GP_SUCCESS;
}

static _gp_internal_status add_pattern(char *pattern, grep_config_t *config) {
    if (pattern == NULL) return _GP_ERR_OPT_ARG_NULL;
    if (ptr_array_add(&config->patterns, pattern) != PTR_ARRAY_SUCCESS) return _GP_ERR_RUNTIME;

    return _GP_SUCCESS; 
}

static _gp_internal_status add_file(char *file, grep_config_t *config) {
    if (file == NULL) return _GP_ERR_OPT_ARG_NULL;
    if (ptr_array_add(&config->files, file) != PTR_ARRAY_SUCCESS) return _GP_ERR_RUNTIME;

    return _GP_SUCCESS;
}

static _gp_internal_status read_patterns_from_file(const char *file, grep_config_t *config) {
    file_reader_t *file_reader = open_file(file);
    if (!file_reader) return _GP_ERR_FILE;

    char* line = NULL;
    size_t size = 0;
    while (read_next_line(file_reader, &line, &size) == FILE_READER_SUCCESS) {
        _gp_internal_status internal_status = add_pattern(line, config);
        if (internal_status != _GP_SUCCESS) return internal_status;
    }

    close_file(file_reader);

    return _GP_SUCCESS;
}

static const char *get_error_message(_gp_internal_status code) {
    switch (code) {
        case _GP_ERR_RUNTIME: return "runtime error"; break;
        case _GP_ERR_FILE: return "file reader error"; break;
        case _GP_ERR_OPT_ARG_NULL: return "option argument is null"; break;
        case _GP_ERR_INVALID_OPT: return "invalid option"; break;
        case _GP_ERR_PATTERN_NOT_SET: return "pattern is not set"; break;
        case _GP_ERR_FILE_NOT_SET: return "file is not set"; break;
        
        default: return "unknow error";
    }
}

static grep_parser_status_t generate_error(grep_parser_status_code_t code, const char *message) { // TODO переписать под форматированную строку
    return (grep_parser_status_t) { code, message };
}