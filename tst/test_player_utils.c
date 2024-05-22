#include <stdio.h>
#include <stdlib.h>
#include "test_player_utils.h"

int test_get_score_from_move() {
    struct world_t * world = createWorld();
    struct move_t init_move = {
        .player_id = -1,
        .tile = CARC_TILE_INIT,
        .x = 0,
        .y = 0,
        .meeple = NO_CONNECTION
    };

    updatingWorld(world, &init_move);

    struct move_t move = {
        .player_id = 0,
        .tile = CARC_TILE_TINI,
        .x = 0,
        .y = 1,
        .meeple = NO_CONNECTION
    };

    int score = get_score_from_move(move, world);

    int expected_score = 48;

    deleteWorld(world);

    return score == expected_score ? 1 : 0;
}

int test_get_best_move() {
    struct world_t * world = createWorld();
    struct move_t init_move = {
        .player_id = -1,
        .tile = CARC_TILE_INIT,
        .x = 0,
        .y = 0,
        .meeple = NO_CONNECTION
    };

    updatingWorld(world, &init_move);

    struct move_t move_playable[3] = {
        {.tile = CARC_TILE_INIT, .x = 1, .y = 0, .meeple = NO_CONNECTION, .player_id = 0},
        {.tile = CARC_TILE_INIT, .x = -1, .y = 1, .meeple = NO_CONNECTION, .player_id = 0},
        {.tile = CARC_TILE_TINI, .x = 0, .y = 1, .meeple = NO_CONNECTION, .player_id = 0}
    };

    struct tile_t tile = CARC_TILE_INIT;
    unsigned int id_player = 0;
    int nb_available_move = 3;

    struct move_t best_move = get_best_move(move_playable, world, nb_available_move, tile, id_player);

    deleteWorld(world);

    // The best move should be {2, 2, 0} since 2+2 is the highest score
    if (best_move.x == 0 && best_move.y == 1) {
        return 1;
    }
    return 0;
}