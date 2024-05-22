#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "graph.h"

/**
 * Create a new world
 * 
 * @return a pointer to the new world
*/
struct world_t* createWorld() {
    struct world_t* world = malloc(sizeof(struct world_t));
    for (unsigned int i = 0; i < MAX_TILES; i++){
        world->tiles[i] = PLACED_TILE_EMPTY;
    }
    world->graph_board = malloc(SIZEOF_GRAPH);

    create_empty_graph(world->graph_board);

    world->num_placed_tiles = 0;
    return world;
}

/**
 * Delete a world
 * 
 * @param world the world to delete
*/
void deleteWorld(struct world_t * world){
    igraph_destroy(world->graph_board);
    free(world->graph_board);
    free(world);
}

/**
 * Update the world with a new move
 * 
 * @param world the world to update
 * @param move the move to add to the world
*/
void updatingWorld(struct world_t * world, const struct move_t * move){
    add_tile_to_world_graph(world, move);
    world->num_placed_tiles++;
    struct placed_tile_t placed_tile = {.tile=move->tile, .x=move->x, .y=move->y, .id=world->num_placed_tiles};
    world->tiles[world->num_placed_tiles-1] = placed_tile;
}

/**
 * Check if a tile has connected tiles
 * 
 * @param world the world to check
 * @param x the x coordinate of the tile
 * @param y the y coordinate of the tile
 * 
 * @return a list of connected tiles
*/
int * tileIsConnected(struct world_t* world, int x, int y) {
    // alloue la mémoire pour la liste de voisin spatial de la tuile donnée

    int * connected_tiles = malloc(MAX_CONNECTIONS_PER_TILE * sizeof(int));

    for (unsigned int j = 0; j < 4; j++){
            connected_tiles[j] = 0;
    }
    for (unsigned int i = 0; i < MAX_TILES; i++){
        if (world->tiles[i].id == PLACED_TILE_EMPTY.id){
            break;
        }else{
            if ((world->tiles[i].x == x) && (world->tiles[i].y == y)){
                // Invalid move
                free(connected_tiles);
                return NULL;
            }else if ((world->tiles[i].x == x-1) && (world->tiles[i].y == y)){
                // West
                connected_tiles[3] = world->tiles[i].id;
            }else if ((world->tiles[i].x == x+1) && (world->tiles[i].y == y)){
                // East
                connected_tiles[1] = world->tiles[i].id;
            }else if ((world->tiles[i].y == y-1) && (world->tiles[i].x == x)){
                // South
                connected_tiles[2] = world->tiles[i].id;
            }else if((world->tiles[i].y == y+1) && (world->tiles[i].x == x)){
                // North
                connected_tiles[0] = world->tiles[i].id;
            }else{
                continue;
            }
        }
    }

    if (connected_tiles[0] && connected_tiles[1] && 
        connected_tiles[2] && connected_tiles[3]){
        // No connected tiles
        free(connected_tiles);
        return NULL;
    }

    return connected_tiles;
}

/**
 * Check if two tiles can be connected
 * 
 * @param ToPlace the tile to place
 * @param neighboor the neighboor tile
*/
int can_connect_2_tiles(struct move_t ToPlace, struct placed_tile_t neighboor, int i) {
    // enum conn_t connection = find_connection(ToPlace.x, ToPlace.y, neighboor.x, neighboor.y);
    switch (i) {
        case 3:
            if(!(ToPlace.tile.c[WW] == neighboor.tile.c[EE])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[WN] == neighboor.tile.c[ES])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[WS] == neighboor.tile.c[ES])) {
                return 0;
            }
            break; // Ajout de break
        case 1:
            if(!(ToPlace.tile.c[EE] == neighboor.tile.c[WW])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[EN] == neighboor.tile.c[WN])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[ES] == neighboor.tile.c[WS])) {
                return 0;
            }
            break; // Ajout de break
        case 0:
            if(!(ToPlace.tile.c[NN] == neighboor.tile.c[SS])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[NW] == neighboor.tile.c[SW])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[NE] == neighboor.tile.c[SE])) {
                return 0;
            }
            break; // Ajout de break
        case 2:
            if(!(ToPlace.tile.c[SS] == neighboor.tile.c[NN])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[SW] == neighboor.tile.c[NW])) {
                return 0;
            }
            else if(!(ToPlace.tile.c[SE] == neighboor.tile.c[NE])) {
                return 0;
            }
            break; // Ajout de break
        case NO_CONNECTION:
            return 0;
        default:
            return 0;
    }
    return 1;
}

/**
 * Check if all the tiles can be connected by color
 * 
 * @param move the move to place
 * @param connected_tiles the connected tiles
 * @param world the world
 * 
 * @return 1 if the tiles can be connected, 0 otherwise
*/
int can_connect(struct move_t move, int * connected_tiles,  struct world_t* world) { // verifie que toutes les tuiles connexe peuvent bien se connecter en terme de couleur
    for (int i = 0; i < MAX_CONNECTIONS_PER_TILE; i++) {
        if(connected_tiles[i] != 0) { // on verifie que ceux qui sont pas null
            if(!(can_connect_2_tiles(move, world->tiles[connected_tiles[i]-1], i))) {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * Check if a move is invalid
 * 
 * @param move the move to check
 * @param world the world
 * 
 * @return 1 if the move is invalid, 0 otherwise
*/
int is_invalid(struct move_t move, struct world_t* world) {
    // verifie, selon le gamemode, si la logique du coup est bien respecté
    // dans le cas sans meeple :
    int * connected_tiles = tileIsConnected(world, move.x, move.y);

    if(connected_tiles == NULL) {
        //free(connected_tiles); pour l'instant pas de soucis, à voir
        return 1;  // Aucun voisin geometrique à la tuile à placer
    }
    if(!(can_connect(move, connected_tiles, world))) {
        free(connected_tiles);
        return 1;  // Les tuiles ne sont pas connectées logiquement (couleur)
    }
    free(connected_tiles);
    return 0;
}

/**
 * Set the boundbox of the world
 * 
 * @param t the boundbox
 * @param world the world
*/
void set_boundbox(int t[4], struct world_t * world){
    int maxy = world->tiles[0].y;
    int miny = world->tiles[0].y;
    int maxx = world->tiles[0].x;
    int minx = world->tiles[0].x;
    for (unsigned int i = 0; i < world->num_placed_tiles; i++){
        if (maxy < world->tiles[i].y){
            maxy = world->tiles[i].y;
        } else if (miny > world->tiles[i].y){
            miny = world->tiles[i].y;
        } else if (maxx < world->tiles[i].x){
            maxx = world->tiles[i].x;
        } else if (minx > world->tiles[i].x){
            minx = world->tiles[i].x;
        } else {
            continue;
        }
    }
    t[0] = maxx+1;
    t[1] = minx-1;
    t[2] = maxy+1;
    t[3] = miny-1;
}

/**
 * Get the available moves
 * 
 * @param available_move all of the available moves
 * @param size the size of the available moves
 * @param tile the tile to place
 * @param world the world
 * @param player_id the player id
 * @param counter the counter
 * 
 * @return the number of available moves
*/
int get_available_move(struct move_t available_move[], int size, const struct tile_t tile, struct world_t * world, unsigned int player_id, int counter){
    for (unsigned int i = 0; i < MAX_TILES; i++){
        if (counter >= size || world->tiles[i].id == PLACED_TILE_EMPTY.id){
            break;
        }
        struct move_t test_move = {.player_id=player_id, .x=world->tiles[i].x, .y=world->tiles[i].y+1, .tile=tile, .meeple=NO_CONNECTION};
        if(!(is_invalid(test_move, world))){
            available_move[counter] = test_move;
            counter++;
        }
        test_move.y -= 2;
        if(!(is_invalid(test_move, world))){
            available_move[counter] = test_move;
            counter++;
        }
        test_move.y++;
        test_move.x++;
        if(!(is_invalid(test_move, world))){
            available_move[counter] = test_move;
            counter++;
        }
        test_move.x -= 2;
        if(!(is_invalid(test_move, world))){
            available_move[counter] = test_move;
            counter++;
        }
    }
    return counter;
}

/**
 * Rotate a tile 90 degrees to the right
 * 
 * @param tile the tile to rotate
 * 
 * @return the rotated tile
*/
struct tile_t rotation_90_right(const struct tile_t tile){
    struct tile_t rotate_tile;

    unsigned int v1 = tile.t[0];
    unsigned int v2 = tile.t[1];
    unsigned int v3 = tile.t[2];

    enum color_t c1 = tile.c[0];
    enum color_t c2 = tile.c[1];
    enum color_t c3 = tile.c[2];

    for (unsigned int i = 3; i < MAX_CONNECTIONS-1; i += 3){
        unsigned int save_v1 = tile.t[i];
        unsigned int save_v2 = tile.t[i+1];
        unsigned int save_v3 = tile.t[i+2];

        enum color_t save_c1 = tile.c[i];
        enum color_t save_c2 = tile.c[i+1];
        enum color_t save_c3 = tile.c[i+2];

        rotate_tile.t[i] = v1;
        rotate_tile.t[i+1] = v2;
        rotate_tile.t[i+2] = v3;

        rotate_tile.c[i] = c1;
        rotate_tile.c[i+1] = c2;
        rotate_tile.c[i+2] = c3;

        v1 = save_v1;
        v2 = save_v2;
        v3 = save_v3;

        c1 = save_c1;
        c2 = save_c2;
        c3 = save_c3;        
    }

    rotate_tile.t[0] = v1;
    rotate_tile.t[1] = v2;
    rotate_tile.t[2] = v3;

    rotate_tile.c[0] = c1;
    rotate_tile.c[1] = c2;
    rotate_tile.c[2] = c3;

    rotate_tile.t[MAX_CONNECTIONS-1] = tile.t[MAX_CONNECTIONS-1];
    rotate_tile.c[MAX_CONNECTIONS-1] = tile.c[MAX_CONNECTIONS-1];

    return rotate_tile;
}

/**
 * Check if a tile is on the edge of the board
 * 
 * @param world the world
 * @param id the id of the tile
 * 
 * @return 1 if the tile is on the edge, 0 otherwise
*/
int is_bord_plateau(const struct world_t *world, int id) {
    int local_id = id%13;
    int tile_id = id/13;
    enum conn_t direction = get_dir_from_local_id(local_id);
    if(direction == CENTER) {
        return 0;
    }
    else if (direction == NO_CONNECTION) {
        return 0;
    }
    
    int has_neighbours_in_dir = tile_has_neighbours_in_direction(tile_id, direction, world, world->tiles[tile_id].x, world->tiles[tile_id].y);
    return (!has_neighbours_in_dir);
}

/**
 * Check if a tile has neighbours in a direction
 * 
 * @param tile_id the id of the tile
 * @param direction the direction to check
 * @param world the world
 * @param x the x coordinate of the tile
 * @param y the y coordinate of the tile
 * 
 * @return 1 if the tile has neighbours in the direction, 0 otherwise
*/
int tile_has_neighbours_in_direction(int tile_id, enum conn_t direction, const struct world_t *world, int x, int y) {
    int x_tile = x;
    int y_tile = y;
    int x_expected;
    int y_expected;
    (void) tile_id;
    switch (direction) 
    {
        case NN: 
            x_expected = x_tile;
            y_expected = y_tile+1;
            break;
        case SS: 
            x_expected = x_tile;
            y_expected = y_tile-1;
            break;
        case EE:
            x_expected = x_tile+1;
            y_expected = y_tile;
            break;
        case WW:
            x_expected = x_tile-1;
            y_expected = y_tile;
            break;
        default:
            return 0;
    }
    for(size_t i = 0; i < world->num_placed_tiles; i++) {
        if(world->tiles[i].x == x_expected && world->tiles[i].y == y_expected) {
            return 1;
        }
    }
    return 0;
}

/**
 * Get the direction from a if of a node
 * 
 * @param id the local id
 * 
 * @return the direction
*/
enum conn_t get_dir_from_local_id(int id) {
    switch (id)
    {
        case 0:
            return NN;
            break;
        case 1:
            return NN;
            break;
        case 2:
            return NN;
            break;
        case 3:
            return EE;
            break;
        case 4:
            return EE;
            break;
        case 5:
            return EE;
            break;
        case 6:
            return SS;
            break;
        case 7:
            return SS;
            break;
        case 8:
            return SS;
            break;
        case 9:
            return WW;
            break;
        case 10:
            return WW;
            break;
        case 11:
            return WW;
            break;
        case 12:
            return CENTER;
            break;
        default:
            return NO_CONNECTION;
    }
    return NO_CONNECTION;
}

/**
 * Copy a graph
 * 
 * @param world the world
 * @param new_graph the new graph
 * 
 * @return a new world
*/
struct world_t* copy_world_with_new_graph(struct world_t* world, igraph_t* new_graph) {
    struct world_t* Newworld = malloc(sizeof(struct world_t));
    
    // Copier les tuiles de l'ancien monde vers le nouveau monde
    memcpy(Newworld->tiles, world->tiles, sizeof(world->tiles));
    
    // Copier le nouveau graphe dans le nouveau monde
    //Newworld->graph_board = malloc(SIZEOF_GRAPH);
    Newworld->graph_board = new_graph;
    //memcpy(Newworld->graph_board, new_graph, SIZEOF_GRAPH);
    
    // Copier les autres données
    Newworld->num_placed_tiles = world->num_placed_tiles;
    
    return Newworld;
}

/**
 * Compute the score of a move
 * 
 * @param move the move to compute the score
 * @param world the world
 * 
 * @return the score of the move
*/
int compute_score(struct move_t *move, struct world_t *world) {
    int nb_vertice_per_tile = 13;
    int nb_placed_tiles = world->num_placed_tiles;
    int vertices_ids[13];
    for (int i = 0; i < nb_vertice_per_tile; i++) {
        vertices_ids[i] = (nb_placed_tiles * nb_vertice_per_tile) + i; // put in vertices_ids, ids of current tile vertices
    }

    // copy the graph representing the world and update it with new tile
    igraph_t *graph_copy = malloc(SIZEOF_GRAPH);
    copy_graph(graph_copy, world->graph_board);
    unsigned int from = world->num_placed_tiles * MAX_CONNECTIONS;
    unsigned int to = (world->num_placed_tiles + 1) * MAX_CONNECTIONS - 1;
    add_tile_graph(graph_copy, &(move->tile), from, to);
    int *neighbour = get_neighbour_tile(world->tiles, move);
    struct world_t *NewWorld = copy_world_with_new_graph(world, graph_copy);
    updatingWorld(NewWorld, move);
    free(neighbour);
    
    // get connexe component containing ids of the new tile;
    int vertices_ids_treated[13] = {0};
    int score = 0;
    int *num_vertices = malloc(sizeof(int));

    for (int i = 0; i < nb_vertice_per_tile; i++) {
        if (vertices_ids_treated[vertices_ids[i] % 13] == 0) { // if vertex is not already treated
            vertices_ids_treated[vertices_ids[i] % 13] = 1; // mark id as treated
            int *connected_components = get_connected_vertices(graph_copy, vertices_ids[i], num_vertices); // get the connected component of vertices_ids[i]

            if (*num_vertices > 1) { // Only calculate the score if the component is not a singleton
                int ids_are_closed = 1;
                for (int j = 0; j < *num_vertices; j++) {
                    if (connected_components[j] / 13 == (int)world->num_placed_tiles) { // vertex is in current move
                        vertices_ids_treated[connected_components[j] % 13] = 1; // mark id as treated
                    }
                    if (is_bord_plateau(NewWorld, connected_components[j])) {
                        ids_are_closed = 0;
                    }
                    if (ids_are_closed == 0) {
                        for (int k = 0; k < *num_vertices; k++) {
                            if (connected_components[k] / 13 == (int)world->num_placed_tiles) {
                                vertices_ids_treated[connected_components[k] % 13] = 1;
                            }
                        }
                        break;
                    }
                }
                if (ids_are_closed == 1) {
                    score += get_score(*num_vertices, connected_components[0], move);
                }
            }
            free(connected_components);
        }
    }
    free(num_vertices);

    deleteWorld(NewWorld);
    return score;
}

/**
 * Get the score of a connected component
 * 
 * @param num_vertices_of_connected_component the number of vertices of the connected component
 * @param id the id of a node
 * @param move the move
 * 
 * @return the score of the connected component
*/
int get_score(int num_vertices_of_connected_component, int id, struct move_t *move) {
    enum color_t color_of_component;
    int local_id = id % 13;
    int score = 0;
    color_of_component = move->tile.c[local_id];
    switch (color_of_component)
    {
        case GREEN:
            score+=num_vertices_of_connected_component;
            break;
        case BROWN:
            score+= (8 * num_vertices_of_connected_component);
            break;
        case GRAY:
            score+= (4 * num_vertices_of_connected_component);
            break;
        default:
            break;       
    }
    return score;
}