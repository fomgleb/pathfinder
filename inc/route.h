#pragma once

#include "bridge.h"

typedef struct s_route {
    t_island *src_island;
    t_island *dst_island;
    t_list *bridges;

    int cur_pos;
    t_island *cur_island;
    t_bridge *cur_bridge;
} t_route;

t_list *mx_get_shortest_routes(t_island *src_island, t_island *dst_island);

