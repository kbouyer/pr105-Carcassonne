#include <stdio.h>
#include "test_board.h"

#define MAX_TILES 72
#define SIZEOF_GRAPH (sizeof(igraph_t))
#define MAX_CONNECTIONS 13

int test_createWorld() {
    struct world_t* world = createWorld();
    int result = (world != NULL) && (world->graph_board != NULL) && (world->num_placed_tiles == 0);
    deleteWorld(world);
    return result;
}

int test_deleteWorld() {
    struct world_t* world = createWorld();
    deleteWorld(world);
    return 1;
}

int test_updatingWorld() {
    struct world_t* world = createWorld();
    struct move_t move = {0, 0 ,0 , CARC_TILE_INIT, NO_CONNECTION};
    updatingWorld(world, &move);
    int result = (world->num_placed_tiles == 1) && (world->tiles[0].x == 0) && (world->tiles[0].y == 0);
    deleteWorld(world);
    return result;
}

int test_tileIsConnected() {
    struct world_t* world = createWorld();
    struct move_t move = {0, 0 ,0 , CARC_TILE_INIT, NO_CONNECTION};
    updatingWorld(world, &move);
    int* connected_tiles = tileIsConnected(world, 0, 0);
    int result = (connected_tiles == NULL);
    free(connected_tiles);
    deleteWorld(world);
    return result;
}

int test_can_connect_2_tiles() {
    struct move_t ToPlace = {.tile = CARC_TILE_GREEN, .x = 0, .y = 0, .meeple = NO_CONNECTION};
    struct placed_tile_t neighbor = {.tile = CARC_TILE_GREEN, .x = 0, .y = 1, .id = 1};
    int result = can_connect_2_tiles(ToPlace, neighbor, 1);
    return result;
}

int test_can_connect() {
    struct world_t* world = createWorld();
    struct move_t move = {0, 0 ,0 , CARC_TILE_GREEN, NO_CONNECTION};
    int connected_tiles[MAX_CONNECTIONS] = {0};
    int result = can_connect(move, connected_tiles, world);
    deleteWorld(world);
    return result;
}

int test_is_invalid() {
    struct world_t* world = createWorld();
    world->tiles[0].tile = CARC_TILE_INIT;
    world->tiles[0].x = 0;
    world->tiles[0].y = 0;
    world->tiles[0].id = 1;
    world->num_placed_tiles = 1;
    struct move_t move = {.player_id = 0, .x = 0, .y = -1, .tile = CARC_TILE_GREEN, .meeple = NO_CONNECTION};
    int result = (is_invalid(move, world) == 0);
    deleteWorld(world);
    return result;
}

int test_set_boundbox() {
    struct world_t* world = createWorld();
    int t[4];
    set_boundbox(t, world);
    int result = (t[0] == -MAX_TILES-2) && (t[1] == -MAX_TILES-4) && (t[2] == -MAX_TILES-2) && (t[3] == -MAX_TILES-4);
    deleteWorld(world);
    return result;
}

int test_get_available_move() {
    struct world_t* world = createWorld();
    struct move_t available_move[10];
    int counter = get_available_move(available_move, 10, CARC_TILE_GREEN, world, 1, 0);
    int result = (counter == 0);
    deleteWorld(world);
    return result;
}

int test_is_bord_plateau() {
    struct world_t* world = createWorld();
    struct move_t move = {0, 0 ,0 , CARC_TILE_INIT, NO_CONNECTION};
    updatingWorld(world, &move);
    int result = is_bord_plateau(world, 0);
    deleteWorld(world);
    return result;
}

int test_tile_has_neighbours_in_direction() {
    struct world_t* world = createWorld();
    struct move_t move = {0, 0 ,0 , CARC_TILE_INIT, NO_CONNECTION};
    updatingWorld(world, &move);
    int result = !(tile_has_neighbours_in_direction(0, NN, world, 0, 0));
    deleteWorld(world);
    return result;
}

int test_get_dir_from_local_id() {
    int result = (get_dir_from_local_id(0) == NN) && (get_dir_from_local_id(3) == EE) && (get_dir_from_local_id(6) == SS) && (get_dir_from_local_id(9) == WW);
    return result;
}
