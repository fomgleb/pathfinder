#include "../inc/bridge.h"
#include "../libmx/inc/libmx.h"

void mx_add_to_bridges(t_bridge **bridges, int *bridges_len, t_bridge new_bridge) {
    *bridges = mx_realloc(*bridges, (*bridges_len + 1) * sizeof(t_bridge));
    ((*bridges)[*bridges_len]).dst_island = new_bridge.dst_island;
    ((*bridges)[*bridges_len]).length = new_bridge.length;
    (*bridges_len)++;
}

t_bridge mx_new_bridge(t_island *island, int length) {
    t_bridge new_bridge = {island, length, 0};
    return new_bridge;
}

t_bridge *mx_get_shortest_untrodden_bridge(t_bridge *bridges, int bridges_len) {
    t_bridge *shortest_bridge = NULL;
    for (int i = 0; i < bridges_len; i++) {
        if (shortest_bridge == NULL || shortest_bridge->length > bridges[i].length) {
            shortest_bridge = &(bridges[i]);
        }
    }
    return shortest_bridge;
}

int mx_count_bridges_with_dst_island(t_bridge *bridges, int bridges_len, char *dst_island_name) {
    int counter = 0;
    for (int i = 0; i < bridges_len; i++) {
        if (mx_strcmp(bridges[i].dst_island->name, dst_island_name) == 0) {
            counter++;
        }
    }
    return counter;
}

