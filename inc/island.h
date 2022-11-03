#pragma once

#include "base_includes.h"
#include "bridge.h"
#include "error.h"

typedef struct s_bridge t_bridge;

typedef struct s_island {
    char *name;
    t_bridge *bridges;
    int bridges_len;
    bool is_passed;
} t_island;

typedef struct s_islands_pair {
    t_island *island1;
    t_island *island2;
} t_islands_pair;

void mx_free_islands(t_island *islands, int islands_len);
t_islands_pair *mx_get_possible_island_pairs(t_island *islands, int islands_len, int *possible_island_pairs_len);
t_island *mx_island_create(char *name, t_bridge *bridges, int bridges_len);
t_island *mx_find_island_by_name(t_island *islands, int islands_len, char *name, int name_len);
t_island *mx_file_to_islands(char *filename, int *islands_len, t_error *error);

