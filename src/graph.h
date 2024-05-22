#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <igraph.h>
#include "board.h"

#include "move.h"

#define SIZEOF_GRAPH sizeof(igraph_t)

void add_tile_to_world_graph(struct world_t *world, const struct move_t *move);

int* get_connected_vertices(const igraph_t *graph, int vertex_id, int *num_vertices);

void add_tile_graph(igraph_t * graph, const struct tile_t * tile, unsigned int from, unsigned int to);

int * get_neighbour_tile(const struct placed_tile_t * tiles, const struct move_t *move);

void link_tiles(struct world_t * world, int * neighbour);

void igraph_to_dot(igraph_t * graph, char * s);

void create_empty_graph(igraph_t * graph);

void copy_graph(igraph_t * to, igraph_t * from);

#endif // _GRAPH_H_