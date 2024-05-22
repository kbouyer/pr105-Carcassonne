#include "graph.h"
#include <stdio.h>
#include <string.h>

/**
 * Adds a tile to the world graph.
 * 
 * @param world the world to add the tile to
 * @param move the move to add to the world
*/
void add_tile_to_world_graph(struct world_t *world, const struct move_t *move)
{
    unsigned int from = world->num_placed_tiles*MAX_CONNECTIONS;
    unsigned int to = (world->num_placed_tiles+1)*MAX_CONNECTIONS-1;
    add_tile_graph(world->graph_board, &(move->tile), from, to);
    int * neighbour = get_neighbour_tile(world->tiles, move);
    link_tiles(world, neighbour);
    free(neighbour);
}

/**
 * Returns the connected vertices of a vertex in a graph.
 * 
 * @param graph the graph to get the connected vertices from
 * @param vertex_id the id of the vertex to get the connected vertices from
 * @param num_vertices the number of connected vertices
 * 
 * @return the connected vertices of the vertex
*/
int* get_connected_vertices(const igraph_t *graph, int vertex_id, int *num_vertices) {
    igraph_vector_int_t result;
    igraph_error_t status;
    
    igraph_vector_int_init(&result, 0);
    
    status = igraph_subcomponent(graph, &result, (igraph_integer_t)vertex_id, IGRAPH_ALL);
    
    if (status != IGRAPH_SUCCESS) {
        igraph_vector_int_destroy(&result);
        *num_vertices = 0;
        return NULL;
    }
    
    *num_vertices = (int)igraph_vector_int_size(&result);
    
    int *connected_vertices = (int *)malloc(*num_vertices * sizeof(int));
    if (connected_vertices == NULL) {
        igraph_vector_int_destroy(&result);
        *num_vertices = 0;
        return NULL;
    }
    
    for (int i = 0; i < *num_vertices; i++) {
        connected_vertices[i] = VECTOR(result)[i];
    }
    
    igraph_vector_int_destroy(&result);
    
    return connected_vertices;
}

/**
 * Adds a tile graph to the world graph.
 * 
 * @param graph the graph to add the tile graph to
 * @param tile the tile to add to the graph
 * @param from the starting index of the tile graph
 * @param to the ending index of the tile graph
*/
void add_tile_graph(igraph_t * graph, const struct tile_t * tile, unsigned int from, unsigned int to){
    unsigned int component_center = tile->t[MAX_CONNECTIONS-1];
    unsigned int counter = 0;
    igraph_add_vertices(graph, MAX_CONNECTIONS, NULL);
    for (unsigned int i = from; i < to; i++) 
    {
        unsigned int component_i = tile->t[i-from];
        if (component_i == component_center && counter < 1){
            igraph_add_edge(graph, i, to);
            counter++;
        }
        for (unsigned int j = i + 1; j < to; j++)
        {
            unsigned int component_j = tile->t[j-from];
            if (component_i == component_j)
            {
                igraph_add_edge(graph, i, j);
                break;
            }
        }
    }
}

/**
 * Returns the neighbour tile of a move.
 * 
 * @param tiles the tiles to get the neighbour from
 * @param move the move to get the neighbour from
 * 
 * @return the neighbour tile of the move
*/
int * get_neighbour_tile(const struct placed_tile_t * tiles, const struct move_t *move){
    int * neighbours = malloc(sizeof(int)*4);
    for (unsigned int j = 0; j < 4; j++){
        neighbours[j] = 0;
    }
    for (unsigned int i = 0; i < MAX_TILES; i++){
        if (tiles[i].id == PLACED_TILE_EMPTY.id){
            break;
        }else{
            if ((tiles[i].x == move->x-1) && (tiles[i].y == move->y)){
                // West
                neighbours[3] = tiles[i].id;
            }else if ((tiles[i].x == move->x+1) && (tiles[i].y == move->y)){
                // East
                neighbours[1] = tiles[i].id;
            }else if ((tiles[i].y == move->y-1) && (tiles[i].x == move->x)){
                // South
                neighbours[2] = tiles[i].id;
            }else if((tiles[i].y == move->y+1) && (tiles[i].x == move->x)){
                // North
                neighbours[0] = tiles[i].id;
            }else{
                continue;
            }
        }
    }
    return neighbours;
}

/**
 * Links the tiles of the world.
 * 
 * @param world the world to link the tiles of
 * @param neighbour the neighbour to link the tiles to
*/
void link_tiles(struct world_t * world, int * neighbour){
    if (neighbour[2] != 0){
        igraph_add_edge(world->graph_board, 13*(neighbour[2]-1), 13*(world->num_placed_tiles)+8);
        igraph_add_edge(world->graph_board, 13*(neighbour[2]-1)+1, 13*(world->num_placed_tiles)+7);
        igraph_add_edge(world->graph_board, 13*(neighbour[2]-1)+2, 13*(world->num_placed_tiles)+6);
    }
    if (neighbour[3] != 0){
        igraph_add_edge(world->graph_board, 13*(neighbour[3]-1)+3, 13*(world->num_placed_tiles)+11);
        igraph_add_edge(world->graph_board, 13*(neighbour[3]-1)+4, 13*(world->num_placed_tiles)+10);
        igraph_add_edge(world->graph_board, 13*(neighbour[3]-1)+5, 13*(world->num_placed_tiles)+9);
    }
    if (neighbour[0] != 0){
        igraph_add_edge(world->graph_board, 13*(neighbour[0]-1)+6, 13*(world->num_placed_tiles)+2);
        igraph_add_edge(world->graph_board, 13*(neighbour[0]-1)+7, 13*(world->num_placed_tiles)+1);
        igraph_add_edge(world->graph_board, 13*(neighbour[0]-1)+8, 13*(world->num_placed_tiles));
    }
    if (neighbour[1] != 0){
        igraph_add_edge(world->graph_board, 13*(neighbour[1]-1)+9, 13*(world->num_placed_tiles)+5);
        igraph_add_edge(world->graph_board, 13*(neighbour[1]-1)+10, 13*(world->num_placed_tiles)+4);
        igraph_add_edge(world->graph_board, 13*(neighbour[1]-1)+11, 13*(world->num_placed_tiles)+3);
    }
}

/**
 * Converts an igraph to a dot file.
 * 
 * @param graph the graph to convert to a dot file
 * @param s the name of the dot file
 * 
 * To convert .dot into .png install graphviz on your computer and use this command in your DOT directory:
 * dot -Tpng filename.dot -o filename.png
*/
void igraph_to_dot(igraph_t * graph, char * s){
    FILE *outfile;
    char outpath[strlen(s)+5];
    strcpy(outpath, s);
    strcat(outpath, ".dot");
    outfile = fopen(outpath, "w");
    igraph_write_graph_dot(graph, outfile);
    fclose(outfile);
}

/**
 * Creates an empty graph.
 * 
 * @param graph the graph to create
*/
void create_empty_graph(igraph_t * graph){
    igraph_empty(graph, 0, IGRAPH_UNDIRECTED);
}

/**
 * Copies a graph.
 * 
 * @param to the graph to copy to
 * @param from the graph to copy from
*/
void copy_graph(igraph_t * to, igraph_t * from){
    igraph_copy(to, from);
}
