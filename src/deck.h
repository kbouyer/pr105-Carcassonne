#ifndef _CARCASSONNE_DECK_H_
#define _CARCASSONNE_DECK_H_

#include <stdlib.h>

#define CARC_TILE_EMPTY ((struct tile_t) { .t={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, \
                              .c={ }})
#define CARC_TILE_INIT  ((struct tile_t) { .t={0,0,0,1,2,3,3,3,3,3,2,1,2}, \
                              .c={BROWN,BROWN,BROWN,GREEN,GRAY,GREEN,   \
                                  GREEN,GREEN,GREEN,GREEN,GRAY,GREEN,GRAY}})
#define CARC_TILE_GREEN ((struct tile_t) { .t={0,0,0,0,0,0,0,0,0,0,0,0,0}, \
                              .c={GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,   \
                                  GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN}})
#define CARC_TILE_XROAD ((struct tile_t) { .t={0,1,2,2,3,4,4,5,6,6,7,0,8}, \
                              .c={GREEN,GRAY,GREEN,GREEN,GRAY,GREEN,   \
                                  GREEN,GRAY,GREEN,GREEN,GRAY,GREEN,GRAY}})
#define CARC_TILE_TINI ((struct tile_t) { .t={0,0,0,0,1,2,3,3,3,2,1,0,1}, \
                              .c={GREEN,GREEN,GREEN,GREEN,GRAY,GREEN,   \
                                  BROWN,BROWN,BROWN,GREEN,GRAY,GREEN,GRAY}})

// A deck of tiles (abstract)
struct deck_t;

// Creates an empty deck with capacity `capacity`
struct deck_t* deck_empty(size_t capacity);

// Returns the number of tiles in `d`
size_t deck_size(const struct deck_t* d);

// Returns the `idx`-th tile in `d`
// Returns CARC_TILE_EMPTY if the index is incorrect
struct tile_t deck_get(const struct deck_t* d, unsigned int idx);

// Adds the tile `t` into `d`
void deck_add(struct deck_t* d, struct tile_t t);

// Creates an independent/deep copy of `d`
struct deck_t* deck_copy(const struct deck_t* d);

// Frees the resources associated to `d`
void deck_free(struct deck_t* d);

#endif // _CARCASSONNE_DECK_H_
