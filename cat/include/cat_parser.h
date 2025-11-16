#ifndef CAT_PARSER_H
#define CAT_PARSER_H

#include <stdio.h>
#include <string.h>

#include "cat_config.h"

int cat_parser_run(const int argc, char* argv[], cat_config_t* config);

#endif