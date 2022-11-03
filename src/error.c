#include "../inc/error.h"

void mx_handle_error(t_error error) {
    switch (error.error_enum) {
        case INVALID_NUMBER_OF_COMMAND_LINE_ARGUMENTS:
            mx_printerr("usage: ./pathfinder [filename]\n");
            break;
        case FILE_DOES_NOT_EXIST:
            mx_printerr("error: file ");
            mx_printerr(error.file_name);
            mx_printerr(" does not exist\n");
            break;
        case FILE_IS_EMPTY:
            mx_printerr("error: file ");
            mx_printerr(error.file_name);
            mx_printerr(" is empty\n");
            break;
        case LINE_IS_NOT_VALID:
            mx_printerr("error: line ");
            char *str_num = mx_itoa(error.line_number);
            mx_printerr(str_num);
            free(str_num);
            mx_printerr(" is not valid\n");
            break;
        case INVALID_NUMBER_OF_ISLANDS:
            mx_printerr("error: invalid number of islands\n");
            break;
        case DUPLICATE_BRIDGES:
            mx_printerr("error: duplicate bridges\n");
            break;
        case BRIDGES_LENGTHS_SUM_IS_TOO_BIG:
            mx_printerr("error: sum of bridges lengths is too big\n");
            break;
        default:
            mx_printerr("Switch is gone.\n");
            break;
    }

    exit(1);
}

