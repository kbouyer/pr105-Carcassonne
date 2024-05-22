#ifndef _CARCASSONNE_PLAYER_UTILS_H
#define _CARCASSONNE_PLAYER_UTILS_H

#include "player.h"
#include "board.h"
#include "graph.h"

struct moveWithValue {
    struct placed_tile_t tiles[MAX_TILES]; // Tableau statique de toutes les tuiles placées
    igraph_t * graph_board;                // Graphe du monde entier
    size_t num_placed_tiles;               // Nombre de tuiles actuellement placées
};

int get_score_from_move(struct move_t move, struct world_t* world);

struct move_t get_best_move(struct move_t move_playable[], struct world_t * world, int nb_available_move, struct tile_t tile, unsigned int id_player);

#endif // _CARCASSONNE_PLAYER_UTILS_H_