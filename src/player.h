#ifndef _CARCASSONNE_PLAYER_H_
#define _CARCASSONNE_PLAYER_H_

#define NUM_PLAYERS 2

#include "deck.h"
#include "move.h"

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name();

/* Player initialization. Is called once before the start of the game.
 * PARAM:
 * - player_id:    the id of the player, between 0 and NUM_PLAYERS-1
 * - first_move:   the first tile put on the board at (0,0) by id = -1
 * - config:       the configuration of the game
 * PRECOND:
 * - `id` is either 0 or 1
 * - `config` contains a `struct deck_t` that is heap-allocated and it
 *    is up to you (the player) to free it before the end of the program
 */
void initialize(unsigned int player_id, const struct move_t first_move,
                struct gameconfig_t config);

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *                  first move of the game, `previous_move` is equal to
                    a starting tile played by player_id == -1
 * - tile:          the tile that the player is supposed to play
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(const struct move_t previous_move, const struct tile_t tile);

/* Clean up the resources the player has been using. Is called once at
   the end of the game.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();

#endif // _CARCASSONNE_PLAYER_H_
