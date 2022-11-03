#include "../inc/route.h"

t_route *mx_create_route(t_island *src_island, t_island *dst_island, t_bridge *cur_bridge) {
    t_route *route = malloc(sizeof(t_route));
    route->src_island = src_island;
    route->dst_island = dst_island;
    route->bridges = NULL;
    route->cur_pos = 0;
    route->cur_island = src_island;
    route->cur_bridge = cur_bridge;
    return route;
}

t_route *mx_route_dup(t_route *route) {
    t_route *dup_route = mx_create_route(route->src_island, route->dst_island, route->cur_bridge);
    for (t_list *i = route->bridges; i != NULL; i = i->next) {
        mx_push_back(&dup_route->bridges, i->data);
    }
    dup_route->cur_pos = route->cur_pos;
    dup_route->cur_island = route->cur_island;
    return dup_route;
}

bool mx_sort(void *a, void *b) {
    return *(int *)a < *(int *)b ? true : false;
}

t_list *mx_get_shortest_routes(t_island *src_island, t_island *dst_island) {
    t_list *possible_routes = NULL;

    src_island->is_passed = true;

    for (int i = 0; i < src_island->bridges_len; i++) {
        t_route *new_route = mx_create_route(src_island, dst_island, &src_island->bridges[i]);
        mx_push_back(&possible_routes, new_route);
    }

    t_list *add_routes = NULL;
    t_list *del_route_indexes = NULL;
    t_list *set_is_passed_islands = NULL;

    bool dst_is_arrived = false;
    int index = 0;

    while (!dst_is_arrived) {
        mx_sort_list(del_route_indexes, mx_sort);
        for (t_list *i = del_route_indexes; i != NULL; i = i->next) {
            int indx = *(int *)i->data;
            t_list *node = mx_get_by_index(possible_routes, indx);
            t_route *route = (t_route *)node->data;
            mx_clear_list(&route->bridges);
            free(route);
            mx_pop_index(&possible_routes, indx);
            free(i->data);
        }
        mx_clear_list(&del_route_indexes);
        for (t_list *i = add_routes; i != NULL ; i = i->next) {
            t_route *route = (t_route *)i->data;
            mx_push_back(&possible_routes, route);
        }
        mx_clear_list(&add_routes);
        for (t_list *i = set_is_passed_islands; i != NULL; i = i->next) {
            ((t_island *)i->data)->is_passed = true;
        }
        mx_clear_list(&set_is_passed_islands);

        index = 0;
        for (t_list *i = possible_routes; i != NULL; i = i->next, index++) {
            t_route *route = (t_route *)i->data;        
            route->cur_pos += 1;
            if (route->cur_bridge->length == route->cur_pos) {
                mx_push_back(&route->bridges, route->cur_bridge);
                mx_push_back(&set_is_passed_islands, route->cur_island);
                route->cur_pos = 0;
                t_island *arrived_island = route->cur_bridge->dst_island;
                route->cur_island = arrived_island;
                if (arrived_island == dst_island) {
                    dst_is_arrived = true;
                    continue;
                }
                if (!arrived_island->is_passed)  {
                    for (int j = 0; j < arrived_island->bridges_len; j++) {
                        if (arrived_island->bridges[j].dst_island->is_passed) {
                            continue;
                        }
                        t_route *route_copy = mx_route_dup(route);
                        route_copy->cur_bridge = &arrived_island->bridges[j];
                        mx_push_back(&add_routes, route_copy);
                    }
                }
                int *stored_index = malloc(sizeof(int));
                *stored_index = index;
                mx_push_back(&del_route_indexes, stored_index);
            }
        }
    }

    for (t_list *i = del_route_indexes; i != NULL; i = i->next) {
        free((int *)i->data);
    }
    mx_clear_list(&del_route_indexes);
    for (t_list *i = add_routes; i != NULL; i = i->next) {
        t_route *route = (t_route *)i->data;
        mx_clear_list(&route->bridges);
        free(route);
    }
    mx_clear_list(&add_routes);
    mx_clear_list(&set_is_passed_islands);

    return possible_routes;
}

