#include "../inc/pathfinder.h"

void mx_print_route(t_route *route) {
    mx_printstr("========================================\n");
    mx_printstr("Path: ");
    mx_printstr(route->src_island->name);
    mx_printstr(" -> ");
    mx_printstr(route->dst_island->name);
    mx_printstr("\nRoute: ");
    mx_printstr(route->src_island->name);
    for (t_list *i = route->bridges; i != NULL; i = i->next) {
        mx_printstr(" -> ");
        mx_printstr(((t_bridge *)i->data)->dst_island->name);
    }
    mx_printstr("\nDistance: ");
    int first_bridge_length = ((t_bridge *)route->bridges->data)[0].length;
    mx_printint(first_bridge_length);
    int sum = first_bridge_length;
    for (t_list *i = route->bridges->next; i != NULL; i = i->next) {
        t_bridge *bridge = (t_bridge *)i->data;
        mx_printstr(" + ");
        mx_printint(bridge->length);
        sum += bridge->length;
    }
    if (mx_list_size(route->bridges) > 1) {
        mx_printstr(" = ");
        mx_printint(sum);
    }
    mx_printstr("\n========================================\n");
}

char *mx_bridges_to_str(t_list *bridges) {
    char *bridges_str = NULL;
    for (t_list *i = bridges; i != NULL; i = i->next) {
        t_bridge *bridge = (t_bridge *)i->data;
        int island_name_len = mx_strlen(bridge->dst_island->name);
        char *new_bridges_str = mx_strnew(mx_strlen(bridges_str) + island_name_len);
        if (bridges_str != NULL)
            mx_strcat(new_bridges_str, bridges_str);
        mx_strcat(new_bridges_str, bridge->dst_island->name);
        free(bridges_str);
        bridges_str = new_bridges_str;
    }
    return bridges_str;
}

bool mx_sort_routes(void *a, void *b) {
    t_route *a_route = (t_route *)a;
    t_route *b_route = (t_route *)b;
    char *a_bridges_str = mx_bridges_to_str(a_route->bridges);
    char *b_bridges_str = mx_bridges_to_str(b_route->bridges);
    bool result = mx_strcmp(a_bridges_str, b_bridges_str) > 0 ? true : false;
    free(a_bridges_str);
    free(b_bridges_str);
    return result;
}

int main(int argc, char **argv) {
    t_error error = {NO_ERRORS, NULL, -1};
    if (argc != 2) {
        error.error_enum = INVALID_NUMBER_OF_COMMAND_LINE_ARGUMENTS;
        mx_handle_error(error);
    }
    error.file_name = argv[1];
    int islands_len = 0;
    t_island *islands = mx_file_to_islands(argv[1], &islands_len, &error);
    if (error.error_enum != NO_ERRORS) {
        mx_handle_error(error);
    }

    int possible_island_pairs_len = 0;
    t_islands_pair *possible_island_pairs = mx_get_possible_island_pairs(islands, islands_len, &possible_island_pairs_len);

    for (int i = 0; i < possible_island_pairs_len; i++) {
        for (int j = 0; j < islands_len; j++) {
            islands[j].is_passed = false;
        }
        t_list *shortest_routes = mx_get_shortest_routes(possible_island_pairs[i].island1, possible_island_pairs[i].island2);
        mx_sort_list(shortest_routes, mx_sort_routes);
        // for (int j = 0; j < shortest_routes_len; j++) {
        //     mx_print_route(&shortest_routes[j]);
        // }
        for (t_list *j = shortest_routes; j != NULL ; j = j->next) {
            t_route *route = (t_route *)j->data;
            mx_print_route(route);
        }
        // for (int j = mx_list_size(shortest_routes) - 1; j >= 0; j--) {
        //     t_route *route = (t_route *)mx_get_by_index(shortest_routes, j)->data;
        //     if (route->cur_island == route->dst_island) {
        //         mx_print_route(route);
        //     }
        // }
        // Freeing
        mx_free_routes_list(&shortest_routes);
    }

    mx_free_islands(islands, islands_len);
    free(islands);
    free(possible_island_pairs);

    return 0;
}

