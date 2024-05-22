#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_graph.h"

// Définissez vos structures et constantes nécessaires
#define MAX_CONNECTIONS 13
#define MAX_TILES 72

// Supposons que PLACED_TILE_EMPTY est défini dans graph.h
#define PLACED_TILE_EMPTY ((struct placed_tile_t) { .tile=TILE_EMPTY, \
                                                    .x=-MAX_TILES-3, \
                                                    .y=-MAX_TILES-3, \
                                                    .id=0})

// Fonction de test pour add_tile_to_world_graph
int test_add_tile_to_world_graph() {
    struct world_t * world = createWorld();

    struct move_t move = {.player_id = -1, .x = 0, .y = 0, .tile = CARC_TILE_INIT, .meeple = NO_CONNECTION};

    add_tile_to_world_graph(world, &move);

    int vertices = igraph_vcount(world->graph_board);
    int edges = igraph_ecount(world->graph_board);

    deleteWorld(world);

    return (vertices == 13 && edges == 9);
}

// Fonction de test pour get_connected_vertices
// int test_get_connected_vertices() {
//     igraph_t graph;
//     igraph_vector_t edges;
//     igraph_vector_init(&edges, 0);
//     igraph_vector_push_back(&edges, 0);
//     igraph_vector_push_back(&edges, 1);
//     igraph_create(&graph, &edges, 0, IGRAPH_UNDIRECTED);

//     int num_vertices;
//     int* vertices = get_connected_vertices(&graph, 0, &num_vertices);

//     int result = (num_vertices == 2 && vertices[0] == 0 && vertices[1] == 1);

//     free(vertices);
//     igraph_destroy(&graph);
//     igraph_vector_destroy(&edges);

//     return result;
// }

// Fonction de test pour add_tile_graph
// int test_add_tile_graph() {
//     struct world_t * world = createWorld();
//     struct move_t move = {.player_id = 0, .x = 0, .y = 0, .tile = CARC_TILE_INIT, .meeple = NO_CONNECTION};

//     add_tile_graph(world->graph_board, &move.tile, 0, MAX_CONNECTIONS);

//     int vertices = igraph_vcount(world->graph_board);

//     igraph_destroy(&world->graph_board);

//     return (vertices == MAX_CONNECTIONS);
// }

// Fonction de test pour get_neighbour_tile
int test_get_neighbour_tile() {
    struct placed_tile_t tiles[MAX_TILES];
    struct move_t move = {.x = 1, .y = 1};
    for (int i = 0; i < MAX_TILES; i++) {
        tiles[i] = PLACED_TILE_EMPTY;
    }
    tiles[0] = (struct placed_tile_t){.id = 1, .x = 0, .y = 1}; // West neighbour

    int* neighbours = get_neighbour_tile(tiles, &move);

    int result = (neighbours[3] == 1);

    free(neighbours);

    return result;
}

// Fonction de test pour link_tiles
int test_link_tiles() {
    struct world_t world;
    igraph_t graph;
    world.graph_board = &graph;
    world.num_placed_tiles = 0;

    create_empty_graph(&graph);

    int neighbours[4] = {0, 0, 0, 1}; // West neighbour with ID 1

    link_tiles(&world, neighbours);

    int edges = igraph_ecount(world.graph_board);

    igraph_destroy(&graph);

    return (edges > 0);
}

