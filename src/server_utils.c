#include "server_utils.h"
#include "board.h"
#include <string.h>

/**
 * Function to check if the library has been loaded correctly
*/
void check_lib_load() {
    if (dlerror() != NULL) {
        fprintf(stderr, "dlsym() failed: %s\n", dlerror());
    }
}

/**
 * Function to load the player library
 * 
 * @param player_path the path to the player library
 * 
 * @return the player structure
*/
struct player player_library_loading(char * player_path) {
    
    struct player my_player;

    dlerror();
    my_player.library = dlopen(player_path, RTLD_LAZY);
    if (!my_player.library){
        fprintf(stderr, "dlopen() failed: %s\n", dlerror());
        exit(1);
    }

    my_player.get_player_name = dlsym(my_player.library, "get_player_name");
    check_lib_load();

    my_player.initialize = dlsym(my_player.library, "initialize");
    check_lib_load();

    my_player.play = dlsym(my_player.library, "play");
    check_lib_load();

    my_player.finalize = dlsym(my_player.library, "finalize");
    check_lib_load();
    
    return my_player;
}

/**
 * Function to choose a random player at the beginning of the game
 * 
 * @param seed the seed to choose the player
 * 
 * @return the player to play
*/
unsigned int random_or_config(unsigned int seed) {
    switch (seed)
    {
    case 0:
        return WHITE; 
        break;
    case 1: 
        return BLACK; 
        break;
    default:
        return rand()%2; 
        break;
    }
}

/**
 * Function to make the first move of the game
 * 
 * @param first_card the first card to play
 * 
 * @return the first move
*/
struct move_t make_first_move(struct tile_t first_card) { // depend du type de la game ? / comment avoir player_id? (forcer que le premier est id = -1)
    struct move_t first_move;
    first_move.player_id = SERVER;
    first_move.x = 0;
    first_move.y = 0;
    first_move.tile = first_card;
    first_move.meeple = NO_CONNECTION;
    return first_move;
}

/**
 * Function to compute the next player
 * 
 * @param current_player the current player
 * 
 * @return the next player
*/
int compute_next_player(int current_player) {
    if (current_player == WHITE) {
        return BLACK;
    } else {
        return WHITE;
    }
}

/**
 * Function to get the next tile to play
 * 
 * @param current_gameconfig the current game configuration
 * @param world the world to play in
 * 
 * @return the next tile to play
*/
struct tile_t get_next_tile(struct gameconfig_t current_gameconfig, struct world_t *world) { // pour l'instant on considere que l'on retire 1 du size à chaque pioche, et que la next est celle de size   
    return deck_get(current_gameconfig.deck, deck_size(current_gameconfig.deck) - 1 - (world->num_placed_tiles)); // on recupere le nombre de tiles posé pour recuperer la bonne tile à  poser du deck
}

/**
 * Function to check if the next tile has an available position
 * 
 * @param next_tile the next tile to play
 * @param world the world to play in
 * 
 * @return 1 if the next tile has an available position, 0 otherwise
*/
int next_tile_has_available_position(struct tile_t next_tile, struct world_t * world){

    struct tile_t next_tile_90_right = rotation_90_right(next_tile);
    struct tile_t next_tile_180_right = rotation_90_right(next_tile_90_right);
    struct tile_t next_tile_270_right = rotation_90_right(next_tile_180_right);

    int boundbox[4];
    set_boundbox(boundbox, world);

    int available_move_size = 4*((boundbox[0]+1)-boundbox[1])*((boundbox[2]+1)-boundbox[3]);
    struct move_t available_move[available_move_size];
    for (int i = 0; i < available_move_size; i++){
        available_move[i].player_id = NUM_PLAYERS;
        available_move[i].x = MAX_TILES-3;
        available_move[i].y = MAX_TILES-3;
        available_move[i].tile = TILE_EMPTY;
        available_move[i].meeple = NO_CONNECTION;
    }

    int nb_available_move = get_available_move(available_move, available_move_size, next_tile, world, SERVER, 0);
    nb_available_move = get_available_move(available_move, available_move_size, next_tile_90_right, world, SERVER, nb_available_move);
    nb_available_move = get_available_move(available_move, available_move_size, next_tile_180_right, world, SERVER, nb_available_move);
    nb_available_move = get_available_move(available_move, available_move_size, next_tile_270_right, world, SERVER, nb_available_move);

    if (nb_available_move == 0){
        return 0;
    }

    return 1;
}

/**
 * Function to check if the game is over
 * 
 * @param world the world to play in
 * @param gameconfig the game configuration
 * 
 * @return 1 if the deck is empty, 2 if the next tile has no available position, 0 otherwise
*/
int is_game_over(struct world_t * world, struct gameconfig_t gameconfig) {
    if (world->num_placed_tiles == deck_size(gameconfig.deck)){
        return 1;
    } else if (!(next_tile_has_available_position(get_next_tile(gameconfig, world), world))){
        return 2;     
    }
    return 0; 
}

/**
 * Returns a tile in order to build a deck later
 * 
 * @param seed the seed to choose the type of tile
 * 
 * @return the tile
*/
struct tile_t make_tile(unsigned int seed) {
    // Allocation de mémoire pour la tile 
    struct tile_t tile;

    switch (seed)
    {
        case 0 : // - Celle avec uniquement de l'herbe 
            tile = CARC_TILE_GREEN;
            break;
        
        case 1 : // - Celle avec le carrefour de 4 routes qui se croisent au centre 
            tile = CARC_TILE_XROAD;
            break;
        
        case 2 :  // - Celle avec 1/4 de chateau fort sur un coté et une route qui va des 2 cotés adjacents au coté du chateau
        //Chateau en Haut (marron)
            tile = CARC_TILE_INIT;
            break;

        case 4 :  //  Celle avec 1/4 de chateau fort sur un coté et une route qui va des 2 cotés adjacents au coté du chateau
        //Chateau en bas (marron)
            tile = CARC_TILE_TINI;
            break;
        default:
            tile = CARC_TILE_EMPTY;
            break;
    }
    return tile; 
}

/**
 * Returns a deck fullfilled with capacity cards
 * 
 * @param capacity the capacity of the deck
 * 
 * @return the deck
*/
struct deck_t *make_deck(size_t capacity) {
    struct deck_t * deck = deck_empty(capacity);
    for(size_t i = 0; i < capacity-1; i++) {
        struct tile_t current__tile = make_tile(rand()%3); 
        deck_add(deck, current__tile); 
    }
    deck_add(deck, CARC_TILE_INIT); 
    return deck; 
}

/**
 * Returns a gameconfig, choose game mode with the seed.
 * 
 * @param seed the seed to choose the game mode
 * 
 * @return the gameconfig
*/
struct gameconfig_t make_gameconfig(unsigned int seed, size_t deck_capacity) {
    struct gameconfig_t game_config = {0};
    struct deck_t *deck = make_deck(deck_capacity); 
    game_config.deck = deck;  
    switch (seed)
    {
    case 0 : //NO_MEEPLE
        game_config.mode = 0; 
        break;
    case 1 : //INFINITE_MEEPLE
        game_config.mode = 1; 
        break;

    case 2 : //FINITE_MEEPLE
        game_config.mode = 1; 
        game_config.meeples = 7; 
        break;
    default:
        break;
    }
    return game_config; 
} 

/**
 * Return the game mode
 * 
 * @param argc the number of arguments
 * @param argv the arguments
 * 
 * @return the game mode
*/
enum gamemode_t get_mode(int argc, char *argv[]) {
    int opt;
    enum gamemode_t gamemode_int = 0;
    while ((opt = getopt (argc, argv, "m:")) != -1){
        switch(opt){
        case 'm':
            gamemode_int = atoi(optarg);
            break;
        case '?':
            break;
        }
    }
    return gamemode_int;
}

/**
 * Function to get the players path
 * 
 * @param argc the number of arguments
 * @param argv the arguments
 * @param player1_path the path of the first player
 * @param player2_path the path of the second player
 * 
 * @return 0 if the function is successful, 1 otherwise
*/
int get_players_path(int argc, char *argv[], char **player1_path, char **player2_path) {
    int mode_index;
    if (argc < 4){
        mode_index = 0;
    }else if (argc == 4){
        fprintf(stderr, "Usage: %s [-m [0:NO_MEEPLE | 1:INFINITE_MEEPLE | 2:FINITE_MEEPLE]] [player1_path] [player2_path]\n", argv[0]);
        return 1;
    }else{
        mode_index = 2;
    }

    if (argc < 3 || argc > 5) {
        fprintf(stderr, "Usage: %s [-m [0:NO_MEEPLE | 1:INFINITE_MEEPLE | 2:FINITE_MEEPLE]] [player1_path] [player2_path]\n", argv[0]);
        return 1;
    }

    *player1_path = argv[mode_index + 1];
    *player2_path = argv[mode_index + 2];

    return 0;
}

/* --------------------------- PLAY MULTIPLE GAME --------------------------- */

/**
 * Function to play multiple games
 * 
 * @param player1_path the path of the first player
 * @param player2_path the path of the second player
 * @param seed the seed to choose the player
 * 
 * @return the result of the game 
*/
struct game_result_t play_game(char* player1_path, char* player2_path, int seed) {
    printf("seed : %u \n", seed);
    srand(seed);
    int nb_card = 50;
    enum gamemode_t game_mode = NO_MEEPLE;

    struct gameconfig_t current_gameconfig = make_gameconfig(game_mode, nb_card);
    /* initialize player*/
    struct gameconfig_t gplayer1;
    gplayer1.deck = deck_copy(current_gameconfig.deck);
    gplayer1.meeples = current_gameconfig.meeples;
    gplayer1.mode = current_gameconfig.mode;

    struct gameconfig_t gplayer2;
    gplayer2.deck = deck_copy(current_gameconfig.deck);
    gplayer2.meeples = current_gameconfig.meeples;
    gplayer2.mode = current_gameconfig.mode;

    struct player player1 = player_library_loading(player1_path); 
    struct player player2 = player_library_loading(player2_path);

    /* first move by server */

    struct move_t first_move = make_first_move(deck_get(current_gameconfig.deck, deck_size(current_gameconfig.deck) -1)); 

    unsigned int starting_player = random_or_config(15); // seed random pour choisir le starting player 

    player1.initialize(0, first_move, gplayer1); // le premier path passé en argument (bilal) id = 0
    int id_bilal = 0;
    player2.initialize(1, first_move, gplayer2); // le deuxieme (darkiki) id = 1
    int id_darkiki = 1;

    struct world_t * world = createWorld();

    struct player players[NUM_PLAYERS] = {player1, player2};
    int scores[NUM_PLAYERS] = {0};

    int current_player = SERVER;
    updatingWorld(world, &first_move);
    struct move_t current_move = first_move;
    int j = 1;

    current_player = (starting_player == 0) ? 1 : 0; // si le premier joueur est celui d'id 0, alors current_player_id = 1 pr compute next player
    int winner;
    while(1) 
    {
        //printf("tour numéro : %d", j);
        current_player = compute_next_player(current_player);
        struct tile_t next_tile = get_next_tile(current_gameconfig, world);
        current_move = players[current_player].play(current_move, next_tile);

        if (is_invalid(current_move, world)){   // TODO : implémenter is_invalid(), mamis je crois que arthur l'a fait dans player. 
            printf("invalide coup ?\n");
            break;
        }

        scores[current_player] += compute_score(&current_move, world);
        updatingWorld(world, &current_move);
        if (is_game_over(world, current_gameconfig) == 2){
            if (current_player == 0) {
                winner = 0;
            } else {
                winner = 1;
            }
            break;
        } else if (is_game_over(world, current_gameconfig) == 1){
            winner = scores[WHITE] > scores[BLACK] ? WHITE : scores[WHITE] == scores[BLACK] ? SERVER : BLACK; // who_win(); // A implémenter
            break;
        }

        j++;

    }

    int winner_id = winner;

    if(winner == id_bilal) {
        winner_id = 0; // par defaut, bilal est l'id 0
    }
    if(winner == id_darkiki) {
        winner_id = 1; // par defaut darkiki est l'id 1
    }
    if(winner == SERVER) {
        winner_id = 2;
    }

    struct game_result_t result = {
        .bilal_score = scores[id_bilal],
        .darkiki_score = scores[id_darkiki],
        .winner_id = winner_id,
    };

    player1.finalize();
    player2.finalize();
    dlclose(player1.library);
    dlclose(player2.library); 

    deck_free(current_gameconfig.deck);

    deleteWorld(world);
    
    return result;

}

/**
 * Function to play multiple games and compare the results
 * 
 * @param nb_game the number of games to play
 * @param player1_path the path of the first player
 * @param player2_path the path of the second player
 * @param seed the seed to choose the player
*/
void play_game_and_compare(int nb_game, char* player1_path, char* player2_path, int seed) {
    int total_scores[NUM_PLAYERS] = {0};
    int games_won[NUM_PLAYERS+1] = {0}; // id = 2 -> server donc égalité

    int id_bilal = 0;
    int id_darkiki = 1;


    for (int i = 0; i < nb_game; i++) {
        int real_seed = seed * time(NULL) * i;
        struct game_result_t result = play_game(player1_path, player2_path, real_seed);
        total_scores[id_bilal]+=result.bilal_score;
        total_scores[id_darkiki]+=result.darkiki_score;

        games_won[result.winner_id]+=1;
    }

    printf("Résultats après %d parties:\n", nb_game);
    printf("Score moyen de darkiki : %d\n", total_scores[id_darkiki] / nb_game);
    printf("Score moyen de bilal : %d\n", total_scores[id_bilal] / nb_game);
    printf("Nombre de parties gagnées par darkiki : %d\n", games_won[id_darkiki]);
    printf("Nombre de parties gagnées par bilal: %d\n", games_won[id_bilal]);
    printf("Nombre de parties égalités : %d\n", games_won[2]);
}