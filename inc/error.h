#pragma once

#include "../libmx/inc/libmx.h"

typedef enum e_error {
    NO_ERRORS,
    INVALID_NUMBER_OF_COMMAND_LINE_ARGUMENTS,
    FILE_DOES_NOT_EXIST,
    FILE_IS_EMPTY,
    LINE_IS_NOT_VALID,
    INVALID_NUMBER_OF_ISLANDS,
    DUPLICATE_BRIDGES,
    BRIDGES_LENGTHS_SUM_IS_TOO_BIG
} t_error_enum;

typedef struct s_error {
    t_error_enum error_enum;
    char *file_name;
    int line_number;
} t_error;


void mx_handle_error(t_error error);

