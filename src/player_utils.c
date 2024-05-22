#include "player_utils.h"

/**
 * Computes the score of a move.
 * 
 * @param move the move to compute the score from
 * @param world the world to compute the score from
 * 
 * @return the score of the move
*/
int get_score_from_move(struct move_t move, struct world_t* world) {
    return compute_score(&move, world);
}

/**
 * Returns the best move from a list of playable moves.
 * 
 * @param move_playable the list of playable moves
 * @param world the world to play in
 * @param nb_available_move the number of available moves
 * @param tile the tile to play
 * @param id_player the id of the player
 * 
 * @return the best move
*/
struct move_t get_best_move(struct move_t move_playable[], struct world_t * world, int nb_available_move, struct tile_t tile, unsigned int id_player) {
    if ((nb_available_move == 0) || (move_playable[0].player_id == NUM_PLAYERS)) {
        struct move_t bad_move = {
            .player_id = id_player,
            .tile = tile,
            .x = 0,
            .y = 0,
            .meeple = NO_CONNECTION,
        };
        return bad_move; 
    }
    struct move_t best_move = move_playable[0];
    int best_score = get_score_from_move(move_playable[0], world);
    for(int i = 0; i < nb_available_move; i++) {
        int score_of_current_move = get_score_from_move(move_playable[i], world);
        if(score_of_current_move > best_score) {
            best_move = move_playable[i];
            best_score = score_of_current_move;
        }
    }
    return best_move;
}