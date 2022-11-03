#pragma once

#include "island.h"

typedef struct s_island t_island;

typedef struct s_bridge {
    t_island *dst_island;
    int length;
    int cur_pos;
} t_bridge;

void mx_add_to_bridges(t_bridge **bridges, int *bridges_len, t_bridge new_bridge);
t_bridge mx_new_bridge(t_island *island, int length);
t_bridge *mx_get_shortest_untrodden_bridge(t_bridge *bridges, int bridges_len);
int mx_count_bridges_with_dst_island(t_bridge *bridges, int bridges_len, char *dst_island_name);

