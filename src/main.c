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
        t_list *possible_routes_list = mx_get_shortest_routes(possible_island_pairs[i].island1, possible_island_pairs[i].island2);
        for (int j = mx_list_size(possible_routes_list) - 1; j >= 0; j--) {
            t_route *route = (t_route *)mx_get_by_index(possible_routes_list, j)->data;
            if (route->cur_island == route->dst_island) {
                mx_print_route(route);
            }
        }
        for (t_list *j = possible_routes_list; j != NULL; j = j->next) {
            t_route *route = (t_route *)j->data;
            mx_clear_list(&route->bridges);
            free(route);
        }
        mx_clear_list(&possible_routes_list);
    }

    mx_free_islands(islands, islands_len);
    free(islands);
    free(possible_island_pairs);

    

    (void)argc;

    return 0;
}

