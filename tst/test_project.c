#include <stdio.h>
#include <string.h>
#include "test_board.h"
#include "test_graph.h"
#include "test_player_utils.h"

#define GREEN_TEXT "\033[32m"
#define RED_TEXT "\033[31m"
#define RESET_TEXT "\033[0m"

/**
  Test
*/
int main(void)
{
    int test = 0;
    int total_test = 0; 
    char error[1000] = "";  

    // Test board
    if (test_createWorld()) ++test; else strcat(error,"test_createWorld\n");
    if (test_deleteWorld()) ++test; else strcat(error,"test_deleteWorld\n");
    if (test_updatingWorld()) ++test; else strcat(error,"test_updatingWorld\n");
    if (test_tileIsConnected()) ++test; else strcat(error,"test_tileIsConnected\n");
    if (test_can_connect_2_tiles()) ++test; else strcat(error,"test_can_connect_2_tiles\n");
    if (test_can_connect()) ++test; else strcat(error,"test_can_connect\n");
    if (test_is_invalid()) ++test; else strcat(error,"test_is_invalid\n");
    if (test_set_boundbox()) ++test; else strcat(error,"test_set_boundbox\n");
    if (test_get_available_move()) ++test; else strcat(error,"test_get_available_move\n");
    if (test_is_bord_plateau()) ++test; else strcat(error,"test_is_bord_plateau\n");
    if (test_tile_has_neighbours_in_direction()) ++test; else strcat(error,"test_tile_has_neighbours_in_direction\n");
    if (test_get_dir_from_local_id()) ++test; else strcat(error,"test_get_dir_from_local_id\n");
    total_test += 12;

    // Test graph
    if (test_add_tile_to_world_graph()) ++test; else strcat(error,"test_add_tile_to_world_graph\n");
    //if (test_get_connected_vertices()) ++test; else strcat(error,"test_get_connected_vertices\n");
    //if (test_add_tile_graph()) ++test; else strcat(error,"test_add_tile_graph\n");
    //if (test_get_neighbour_tile()) ++test; else strcat(error,"test_get_neighbour_tile\n");
    //if (test_link_tiles()) ++test; else strcat(error,"test_link_tiles\n");
    total_test += 1;

    // Test player_utils
    if (test_get_score_from_move()) ++test; else strcat(error,"test_get_player_id\n");
    if (test_get_best_move()) ++test; else strcat(error,"test_get_player_name\n");
    total_test += 2;
    
    printf(RED_TEXT "\n%s" RESET_TEXT, error);
    if (test == total_test) {
        // Print in green
        printf(GREEN_TEXT "\nTEST PASSED %d/%d\n" RESET_TEXT, test, total_test);
    } else {
        // Print in red
        printf(RED_TEXT "\nTEST FAILED %d/%d\n" RESET_TEXT, test, total_test);
    }   
    return 0;
}
