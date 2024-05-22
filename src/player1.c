#include "player.h"
#include <stdio.h>
#include <stdlib.h>

#include "player_utils.h"

///////////////////// INFORMATIONS DU JOUEUR ////////////////////////

char const* player1_name = "Bilal";  

unsigned int player1_id; 

struct gameconfig_t player1_gameconfig;

unsigned int player1_num_meeples; 

struct world_t * player1_world = NULL;

struct move_t player1_first_move;

/**
 * Function to get the name of the player
 * 
 * @return the name of the player
*/
char const* get_player_name(){ 
    return player1_name; 
}

/**
 * Function to initialize the player
 * 
 * @param player_id the id of the player
 * @param first_move the first move of the game
 * @param config the configuration of the game
*/
void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
    // On initialise les infos du joueur et de la partie que nous envoie le serveur. 
    player1_id = player_id; 

    player1_gameconfig = config;

    player1_world = createWorld();
    updatingWorld(player1_world, &first_move);
    player1_first_move = first_move;

    player1_num_meeples = player1_gameconfig.meeples; 
}

/**
 * Function to play the next move
 * 
 * @param previous_move the previous move of the player
 * @param tile the tile to play
 * 
 * @return the move to play
*/
struct move_t play(const struct move_t previous_move, const struct tile_t tile) {

    if (!(previous_move.x == 0 && previous_move.y == 0)){
        updatingWorld(player1_world, (struct move_t *) &previous_move);
    }

    struct tile_t tile_90_right = rotation_90_right(tile);
    struct tile_t tile_180_right = rotation_90_right(tile_90_right);
    struct tile_t tile_270_right = rotation_90_right(tile_180_right);

    int boundbox[4];
    set_boundbox(boundbox, player1_world);

    int available_move_size = 4*((boundbox[0]+1)-boundbox[1])*((boundbox[2]+1)-boundbox[3]);

    struct move_t available_move[available_move_size];
    for (int i = 0; i < available_move_size; i++){
        available_move[i].player_id = NUM_PLAYERS;
        available_move[i].x = MAX_TILES-3;
        available_move[i].y = MAX_TILES-3;
        available_move[i].tile = TILE_EMPTY;
        available_move[i].meeple = NO_CONNECTION;
    }

    int nb_available_move = get_available_move(available_move, available_move_size, tile, player1_world, player1_id, 0);
    nb_available_move = get_available_move(available_move, available_move_size, tile_90_right, player1_world, player1_id, nb_available_move);
    nb_available_move = get_available_move(available_move, available_move_size, tile_180_right, player1_world, player1_id, nb_available_move);
    nb_available_move = get_available_move(available_move, available_move_size, tile_270_right, player1_world, player1_id, nb_available_move);

    struct move_t move_to_play = get_best_move(available_move, player1_world, nb_available_move, tile, player1_id);

    updatingWorld(player1_world, &move_to_play);

    return move_to_play;
}

/**
 * Function to delete the deck and the world
*/
void finalize() {
    deck_free(player1_gameconfig.deck);
    deleteWorld(player1_world);
}
