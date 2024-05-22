#ifndef _CARCASSONNE_BOARD_H
#define _CARCASSONNE_BOARD_H

#include <stdlib.h>
#include <igraph.h>
#include "move.h"

#define MAX_CONNECTIONS_PER_TILE 4 // dans le cas d'un carré

#define TILE_EMPTY ((struct tile_t) { .t={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, \
                               .c={ }})

#define MAX_TILES 72


struct placed_tile_t {
    struct tile_t tile;
    int x;
    int y;
    int id;
};

#define PLACED_TILE_EMPTY ((struct placed_tile_t) { .tile=TILE_EMPTY, \
                                                    .x=-MAX_TILES-3, \
                                                    .y=-MAX_TILES-3, \
                                                    .id=0})

struct world_t {
    struct placed_tile_t tiles[MAX_TILES]; // Tableau statique de toutes les tuiles placées
    igraph_t * graph_board;                // Graphe du monde entier
    size_t num_placed_tiles;               // Nombre de tuiles actuellement placées
};


struct world_t* createWorld();

void deleteWorld(struct world_t * world);

void updatingWorld(struct world_t * world, const struct move_t * move);

int * tileIsConnected(struct world_t* world, int x, int y);

int can_connect_2_tiles(struct move_t ToPlace, struct placed_tile_t neighboor, int i);

int can_connect(struct move_t ToPlace, int * connected_tiles,  struct world_t* world);

int is_invalid(struct move_t move, struct world_t* world); 

void set_boundbox(int t[4], struct world_t * world);

int get_available_move(struct move_t available_move[], int size, const struct tile_t tile, struct world_t * world, unsigned int player_id, int counter);

struct tile_t rotation_90_right(const struct tile_t tile);

int is_bord_plateau(const struct world_t *world, int id);

int tile_has_neighbours_in_direction(int tile_id, enum conn_t direction, const struct world_t *world, int x, int y);

enum conn_t get_dir_from_local_id(int id);

int get_score(int num_vertices_of_connected_component, int id, struct move_t *move);

int compute_score(struct move_t *move, struct world_t *world);

#endif // _CARCASSONNE_BOARD_H

