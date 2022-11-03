#include "../inc/island.h"

void mx_free_islands(t_island *islands, int islands_len) {
    for (int i = 0; i < islands_len; i++) {
        free(islands[i].name);
        free(islands[i].bridges);
    }
}

t_islands_pair *mx_get_possible_island_pairs(t_island *islands, int islands_len, int *possible_island_pairs_len) {
    int possible_island_pairs_count = (int)mx_pow(islands_len, 2) - (islands_len * (1 + islands_len)) / 2;
    t_islands_pair *possible_island_pairs = malloc(possible_island_pairs_count * sizeof(t_islands_pair));
    *possible_island_pairs_len = 0;

    for (int i = 0; i < islands_len; i++) {
        for (int j = i + 1; j < islands_len; j++) {
            possible_island_pairs[*possible_island_pairs_len].island1 = &islands[i];
            possible_island_pairs[(*possible_island_pairs_len)++].island2 = &islands[j];
        }
    }

    return possible_island_pairs;
}

t_island *mx_island_create(char *name, t_bridge *bridges, int bridges_len) {
    t_island *island = malloc(sizeof(t_island));
    island->name = name;
    island->bridges = bridges;
    island->bridges_len = bridges_len;
    return island;
}

t_island *mx_find_island_by_name(t_island *islands, int islands_len, char *name, int name_len) {
    for (int i = 0; i < islands_len; i++) {
        if (mx_strncmp(islands[i].name, name, name_len) == 0) {
            return &(islands[i]);
        }
    }
    return NULL;
}

bool mx_island_line_is_valid(char *line) {
    int curr_index = 0;
    if (!mx_isalpha(line[curr_index++])) {
        return false;
    }
    for (; line[curr_index] != '-'; curr_index++) {
        if (!mx_isalpha(line[curr_index])) {
            return false;
        }
    }
    curr_index++;
    for (; line[curr_index] != ','; curr_index++) {
        if (!mx_isalpha(line[curr_index])) {
            return false;
        }
    }
    curr_index++;
    for (; line[curr_index] != '\0' ; curr_index++) {
        if (!mx_isdigit(line[curr_index])) {
            return false;
        }
    }
    return true;
}

t_island *mx_file_to_islands(char *filename, int *islands_len, t_error *error) {
    int file = open(filename, O_RDONLY);
	if (file == -1) {
        error->error_enum = FILE_DOES_NOT_EXIST;
        return NULL;
    }
    char *str = mx_file_to_str(filename);
    if (str == NULL) {
        error->error_enum = FILE_IS_EMPTY;
        return NULL;
    }
    char **str_lines = mx_strsplit(str, '\n');
    if (!mx_num_in_str_is_positive(str_lines[0])) {
        error->error_enum = LINE_IS_NOT_VALID;
        error->line_number = 1;
        mx_free_str_arr(&str_lines);
        free(str);
        return NULL;
    }

    int line1_islands_count = mx_atoi(str_lines[0]);
    int lines_count = mx_strarr_len(str_lines);

    long long int sum_of_bridge_lengths = 0;

    *islands_len = 0;
    t_island *islands = malloc((lines_count - 1) * sizeof(t_island));

    for (int i = 1; i < lines_count; i++) {
        if (!mx_island_line_is_valid(str_lines[i])) {
            error->error_enum = LINE_IS_NOT_VALID;
            error->line_number = i + 1;
            mx_free_str_arr(&str_lines);
            free(str);
            mx_free_islands(islands, *islands_len);
            free(islands);
            return NULL;
        }

        int dash_index = mx_get_char_index(str_lines[i], '-');
        int comma_index = mx_get_char_index(str_lines[i], ',');

        int island1_name_len = dash_index;
        int island2_name_len = comma_index - dash_index - 1;

        char *island1_name_ptr = str_lines[i];
        char *island2_name_ptr = str_lines[i] + dash_index + 1;

        int bridge_length = mx_atoi(str_lines[i] + comma_index + 1);
        sum_of_bridge_lengths += bridge_length;

        t_island *island1 = mx_find_island_by_name(islands, *islands_len, island1_name_ptr, island1_name_len);
        t_island *island2 = mx_find_island_by_name(islands, *islands_len, island2_name_ptr, island2_name_len);

        if (island1 == NULL) {
            island1 = mx_island_create(mx_strndup(str_lines[i], island1_name_len), NULL, 0);
            islands[(*islands_len)++] = *island1;
            free(island1);
            island1 = &(islands[*islands_len - 1]);
        }
        if (island2 == NULL) {
            island2 = mx_island_create(mx_strndup(str_lines[i] + dash_index + 1, island2_name_len), NULL, 0);
            islands[(*islands_len)++] = *island2;
            free(island2);
            island2 = &(islands[*islands_len - 1]);
        }

        if (mx_strcmp(island1->name, island2->name) == 0) {
            error->error_enum = LINE_IS_NOT_VALID;
            error->line_number = i + 1;
            mx_free_str_arr(&str_lines);
            free(str);
            mx_free_islands(islands, line1_islands_count);    
            free(islands);
            return NULL;
        }

        mx_add_to_bridges(&(island1->bridges), &(island1->bridges_len), mx_new_bridge(island2, bridge_length));
        mx_add_to_bridges(&(island2->bridges), &(island2->bridges_len), mx_new_bridge(island1, bridge_length));
    }

    if (line1_islands_count != *islands_len) {
        error->error_enum = INVALID_NUMBER_OF_ISLANDS;
        mx_free_str_arr(&str_lines);
        free(str);
        mx_free_islands(islands, *islands_len);    
        free(islands);
        return NULL;
    }

    for (int i = 1; i < *islands_len; i++) {
        t_island *island1 = &islands[0];
        t_island *island2 = &islands[i];
        if (mx_count_bridges_with_dst_island(island1->bridges, island1->bridges_len, island2->name) > 1 ||
        mx_count_bridges_with_dst_island(island2->bridges, island2->bridges_len, island1->name) > 1) {
            error->error_enum = DUPLICATE_BRIDGES;
            mx_free_str_arr(&str_lines);
            free(str);
            mx_free_islands(islands, line1_islands_count);    
            free(islands);
            return NULL;
        }
    }

    if (sum_of_bridge_lengths > __INT_MAX__) {
        error->error_enum = BRIDGES_LENGTHS_SUM_IS_TOO_BIG;
        mx_free_str_arr(&str_lines);
        free(str);
        mx_free_islands(islands, line1_islands_count);
        free(islands);
        return NULL;
    }

    free(str);
    for (int i = 0; str_lines[i] != NULL; i++) {
        free(str_lines[i]);
    }
    free(str_lines);

    return islands;
}

