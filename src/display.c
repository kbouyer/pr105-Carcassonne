#include "display.h"



/**
 * Computes a Boundbox corresponding to extremums positions of the world. 
 * 
 * @param world the world to compute the boundbox from
 * 
 * @return the boundbox of the world
*/
struct boundbox calculate_boundbox(const struct world_t *world) {
    struct boundbox box;
    if (world->num_placed_tiles == 0) {
        // Si aucune tuile n'a été placée, la boundbox est vide
        box.topleft.x = 0;
        box.topleft.y = 0;
        box.downright.x = 0;
        box.downright.y = 0;
    } else {
        // Initialisation des coordonnées de la boundbox avec celles de la première tuile placée
        box.topleft.x = world->tiles[0].x;
        box.topleft.y = world->tiles[0].y;
        box.downright.x = world->tiles[0].x;
        box.downright.y = world->tiles[0].y;

        // Parcours des tuiles placées pour trouver les coordonnées extrêmes
        for (size_t i = 1; i < world->num_placed_tiles; ++i) {
            int x = world->tiles[i].x;
            int y = world->tiles[i].y;
            // Mise à jour de la boundbox si une tuile est située en dehors des limites actuelles
            if (x < box.topleft.x) {
                box.topleft.x = x;
            } else if (x > box.downright.x) {
                box.downright.x = x;
            }
            if (y > box.topleft.y) {
                box.topleft.y = y;
            } else if (y < box.downright.y) {
                box.downright.y = y;
            }
        }
    }
    return box;
}

/**
 * Returns the index of the tile in the world which position is pos 
 * 
 * @param position the position of the tile to find
 * @param world the world to search in
 * 
 * @return the index of the tile in the world, -1 if no tile is found
*/
int get_tile(struct position position, const struct world_t *world) {
    // Calcul de la boundbox du monde
    struct boundbox box = calculate_boundbox(world);

    // Vérification si la position est en dehors de la boundbox
    if (position.x < box.topleft.x || position.x > box.downright.x ||
        position.y > box.topleft.y || position.y < box.downright.y) {
        // Si la position est en dehors de la boundbox, retourne -1
        return -1;
    }

    // Parcours des tuiles placées pour trouver celle qui correspond à la position donnée
    for (size_t i = 0; i < world->num_placed_tiles; ++i) {
        if (world->tiles[i].x == position.x && world->tiles[i].y == position.y) {
            // Si la tuile correspond à la position, retourne son indice
            return i;
        }
    }

    // Si aucune tuile ne correspond à la position, retourne -1
    return -1;
}

/**
 * Structure representing a matrix of the world
*/
struct world_matrix {
    enum color_t matrix[5*MAX_TILES][5*MAX_TILES];
}; 

/**
 * Returns the color of a tile
 * 
 * @param color the color of the tile
 * 
 * @return the color of the tile
*/
char *tilecolor_to_color(enum color_t color) {
    char *colorstring = NULL; 
    switch (color)
    {
    case GREEN:
        colorstring = GREEN_COLOR; 
        break;

    case BROWN:
        colorstring=BROWN_COLOR;
        break;
    
    case BLUE:
        colorstring=BLUE_COLOR;
        break;
    
    case GRAY:
        colorstring=GRAY_COLOR;
        break;
    
    default:
        break;
    }
    return colorstring; 
}

/**
 * Shifts the world to the right and to the bottom
 * 
 * @param world the world to shift
 * 
 * @return the shifted world
*/
struct world_t *world_shift(struct world_t *world){
    for(size_t i = 0; i< world->num_placed_tiles; i++){
        world->tiles[i].x += MAX_TILES; 
        world->tiles[i].y += MAX_TILES; 
    }
    return world; 
}

/**
 * Initializes a world matrix
 * 
 * @param world the world to initialize the matrix from
 * 
 * @return the initialized world matrix
*/
struct world_matrix world_matrix_init(struct world_t *world) {
    struct world_matrix world_m; 
    //struct boundbox box = calculate_boundbox(world); 
    //struct world_t *shifted_world = world_shift(world); 
    for( size_t i = 0 ; i < world->num_placed_tiles ; i++ ) {
        // Les 9 cases du milieu 
        world_m.matrix[5*world->tiles[i].x][5*world->tiles[i].y] = world->tiles[i].tile.c[WN]; 
        world_m.matrix[5*world->tiles[i].x+1][5*world->tiles[i].y] = world->tiles[i].tile.c[NW];
        world_m.matrix[5*world->tiles[i].x+2][5*world->tiles[i].y] = world->tiles[i].tile.c[NN];
        world_m.matrix[5*world->tiles[i].x+3][5*world->tiles[i].y] = world->tiles[i].tile.c[NE];
        world_m.matrix[5*world->tiles[i].x+4][5*world->tiles[i].y] = world->tiles[i].tile.c[EN];

        world_m.matrix[5*world->tiles[i].x][5*world->tiles[i].y-1] = world->tiles[i].tile.c[WN]; 
        world_m.matrix[5*world->tiles[i].x+1][5*world->tiles[i].y-1] = world->tiles[i].tile.c[WN]; 
        world_m.matrix[5*world->tiles[i].x+2][5*world->tiles[i].y-1] = world->tiles[i].tile.c[NN]; 
        world_m.matrix[5*world->tiles[i].x+3][5*world->tiles[i].y-1] = world->tiles[i].tile.c[EN]; 
        world_m.matrix[5*world->tiles[i].x+4][5*world->tiles[i].y-1] = world->tiles[i].tile.c[EN]; 

        world_m.matrix[5*world->tiles[i].x][5*world->tiles[i].y-2] = world->tiles[i].tile.c[WW]; 
        world_m.matrix[5*world->tiles[i].x+1][5*world->tiles[i].y-2] = world->tiles[i].tile.c[WW]; 
        world_m.matrix[5*world->tiles[i].x+2][5*world->tiles[i].y-2] = world->tiles[i].tile.c[CENTER]; 
        world_m.matrix[5*world->tiles[i].x+3][5*world->tiles[i].y-2] = world->tiles[i].tile.c[EE]; 
        world_m.matrix[5*world->tiles[i].x+4][5*world->tiles[i].y-2] = world->tiles[i].tile.c[EE]; 

        world_m.matrix[5*world->tiles[i].x][5*world->tiles[i].y-3] = world->tiles[i].tile.c[WS]; 
        world_m.matrix[5*world->tiles[i].x+1][5*world->tiles[i].y-3] = world->tiles[i].tile.c[WS]; 
        world_m.matrix[5*world->tiles[i].x+2][5*world->tiles[i].y-3] = world->tiles[i].tile.c[SS]; 
        world_m.matrix[5*world->tiles[i].x+3][5*world->tiles[i].y-3] = world->tiles[i].tile.c[ES]; 
        world_m.matrix[5*world->tiles[i].x+4][5*world->tiles[i].y-3] = world->tiles[i].tile.c[ES]; 

        world_m.matrix[5*world->tiles[i].x][5*world->tiles[i].y] = world->tiles[i].tile.c[WS]; 
        world_m.matrix[5*world->tiles[i].x+1][5*world->tiles[i].y] = world->tiles[i].tile.c[SW];
        world_m.matrix[5*world->tiles[i].x+2][5*world->tiles[i].y] = world->tiles[i].tile.c[SS];
        world_m.matrix[5*world->tiles[i].x+3][5*world->tiles[i].y] = world->tiles[i].tile.c[SE];
        world_m.matrix[5*world->tiles[i].x+4][5*world->tiles[i].y] = world->tiles[i].tile.c[ES];
    }
    return world_m; 
} 

/**
 * Displays the world
 * 
 * @param world the world to display
*/
void world_display(struct world_t *world) {
    struct world_matrix matrix = world_matrix_init(world); 
    struct boundbox box = calculate_boundbox(world_shift(world)); 
    for(int i = box.topleft.x; i < box.downright.x ; i++ ){
        for(int j = box.topleft.y; i < box.downright.y; j++){
            struct position current_pos = {
                .x = i, 
                .y = j,
            };
            if(get_tile(current_pos,world)!= -1){
                enum color_t current_color = matrix.matrix[current_pos.x][current_pos.y]; 
                printf("%s\u2588", tilecolor_to_color(current_color)); 
            }
            else {
                printf(" "); 
            }
            printf(RESET_COLOR); 
        }
        printf("\n"); 
    }
}