#ifndef __TEST_BOARD_H__
#define __TEST_BOARD_H__

#include "../src/board.h"
#include "../src/deck.h"

int test_createWorld();

int test_deleteWorld();

int test_updatingWorld();

int test_tileIsConnected();

int test_can_connect_2_tiles();

int test_can_connect();

int test_is_invalid();

int test_set_boundbox();

int test_get_available_move();

int test_rotation_90_right();

int test_is_bord_plateau();

int test_tile_has_neighbours_in_direction();

int test_get_dir_from_local_id();

int test_copy_world_with_new_graph();

#endif // __TEST_BOARD_H__