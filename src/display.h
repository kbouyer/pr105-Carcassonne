#ifndef _CARCASSONNE_DISPLAY_H_
#define _CARCASSONNE_DISPLAY_H_

#include "board.h"
#include "display.h"
#include "move.h"
#include "deck.h"
#include <stdio.h>

#define BOLD_TEXT "\033[1m"
#define UNDERLINE_TEXT "\033[4m"

#define RESET_COLOR "\033[0m"
#define BLACK_COLOR "\033[0;30m"
#define RED_COLOR "\033[0;31m"
#define GREEN_COLOR "\033[0;32m"
#define YELLOW_COLOR "\033[0;33m"
#define BLUE_COLOR "\033[0;34m"
#define BROWN_COLOR "\033[48;2;139;69;19m"
#define GRAY_COLOR "\033[1;37m"

struct position {
    int x;
    int y; 
}; 

struct boundbox {
    struct position topleft;
    struct position downright; 
}; 



struct boundbox calculate_boundbox(const struct world_t *world);

int get_tile(struct position position, const struct world_t *world); 

#endif