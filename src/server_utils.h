#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <dlfcn.h>

#include "player.h"
#include "deck.h"
#include "move.h"
#include "board.h"

struct player {
    char const* (*get_player_name)();
    void (*initialize)(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config);
    struct move_t (*play)(struct move_t, const struct tile_t);
    void (*finalize)();
    void *library;
};

struct game_result_t {
    int bilal_score;
    int darkiki_score;
    int winner_id; 
};


void check_lib_load();

struct player player_library_loading(char * player_path);

unsigned int random_or_config(unsigned int seed);

struct move_t make_first_move(struct tile_t first_card);

int compute_next_player(int current_player);

struct tile_t get_next_tile(struct gameconfig_t current_gameconfig, struct world_t *world);

int is_winning(struct player *current_player);

struct tile_t make_tile(unsigned int seed);

struct deck_t *make_deck(size_t capacity);

struct gameconfig_t make_gameconfig(unsigned int seed, size_t deck_capacity);

enum gamemode_t get_mode(int argc, char *argv[]);

int get_players_path(int argc, char *argv[], char **player1_path, char **player2_path);

int next_tile_has_available_position(struct tile_t next_tile, struct world_t * world);

int is_game_over(struct world_t * world, struct gameconfig_t gameconfig);

struct game_result_t play_game(char* player1_path, char* player2_path, int seed);

void play_game_and_compare(int nb_game, char* player1_path, char* player2_path, int seed);


