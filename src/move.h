#ifndef _CARCASSONNE_MOVE_H_
#define _CARCASSONNE_MOVE_H_

#include <stddef.h>

#include "deck.h"

// The player ids
#define NUM_PLAYERS 2
enum player_color_t { BLACK=0, WHITE=1, SERVER=-1 };

// The game modes
enum gamemode_t {
  NO_MEEPLE,                      // The game is played without meeples
                                  // First player to end a structure earns his points
  INFINITE_MEEPLE,                // The game is played with meeples (that must be placed)
                                  // Each player has an inifinite supply of meeples
                                  // When a structure is at end, the player with the most
                                  // meeples inside earns the points. If equality, nobody wins.
  FINITE_MEEPLE,                  // Same as previous, but each player has a finite number
                                  // of meeples
};

// The configuration of a game
struct gameconfig_t {
  enum gamemode_t mode;           // The game mode (cf. above)
  size_t meeples;                 // The number of meeples if mode == FINITE_MEEPLE
  struct deck_t* deck;            // The deck with which the game is played
};

// The colors of the sides of the tiles
enum color_t {
  GREEN, GRAY, BLUE, BROWN,
  PURPLE, GOLD, AZURE, MAHOGANY,
};

// The vertices in a tile (given by their positions)
enum conn_t {
  NW=0, NN=1, NE=2,
  EN=3, EE=4, ES=5,
  SE=6, SS=7, SW=8,
  WS=9, WW=10, WN=11,
  CENTER=12, MAX_CONNECTIONS=13,
  NO_CONNECTION=14,
};

// A tile in the game
struct tile_t {
  unsigned int t[MAX_CONNECTIONS]; // The connected components of the tile
                                   // Each vertex has one color (unsigned int)
                                   // Each color represents a component
  enum color_t c[MAX_CONNECTIONS]; // The colors of the vertices of the tile
};

// A move in the game
struct move_t {
  enum player_color_t player_id; // The id of the player doing the move
  int x, y;               // The central position of the tile
  struct tile_t tile;     // The tile itself
  enum conn_t meeple;     // The position of the meeple (or NO_CONNECTION)
};

#endif // _CARCASSONNE_MOVE_H_
