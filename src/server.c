#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <dlfcn.h>
#include <string.h>

#include "server_utils.h"
#include "graph.h"

// Macros correspondant aux strings du code ANSI pour printer des couleurs dans le termianl : 
#define BOLD_TEXT "\033[1m"
#define UNDERLINE_TEXT "\033[4m"

#define RESET_COLOR "\033[0m"
#define BLACK_COLOR "\033[0;30m"
#define RED_COLOR "\033[0;31m"
#define GREEN_COLOR "\033[0;32m"
#define YELLOW_COLOR "\033[0;33m"
#define BLUE_COLOR "\033[0;34m"

/**
 * Main function
 * 
 * @param argc the number of arguments
 * @param argv the arguments
 * 
 * @return 0 if the program executed successfully
*/
int main(int argc, char* argv[]) {
    // Initialisation de la graine du générateur de nombres aléatoires
    unsigned int seed = time(NULL);
    srand(seed);
    printf("seed: %u \n", seed);

    enum gamemode_t game_mode = get_mode(argc, argv);

    char *player1_path;
    char *player2_path;

    if (get_players_path(argc, argv, &player1_path, &player2_path) != 0) {
        return 0;
    } 

    // Création de la configuration du jeu
    struct gameconfig_t current_gameconfig = make_gameconfig(game_mode, 35);

    // Copie des config pour les joueurs
    struct gameconfig_t gplayer1;
    gplayer1.deck = deck_copy(current_gameconfig.deck);
    gplayer1.meeples = current_gameconfig.meeples;
    gplayer1.mode = current_gameconfig.mode;

    struct gameconfig_t gplayer2;
    gplayer2.deck = deck_copy(current_gameconfig.deck);
    gplayer2.meeples = current_gameconfig.meeples;
    gplayer2.mode = current_gameconfig.mode;

    // Inclusion des joueurs depuis les fichiers partagés
    struct player player1 = player_library_loading(player1_path); 
    struct player player2 = player_library_loading(player2_path);

    // Création du premier mouvement en utilisant la première carte du paquet
    struct move_t first_move = make_first_move(deck_get(current_gameconfig.deck, deck_size(current_gameconfig.deck) -1)); 

    // Détermination du joueur qui commence
    unsigned int starting_player = 0;

    // Initialisation des joueurs 
    player1.initialize(starting_player, first_move, gplayer1);
    player2.initialize((starting_player + 1) % 2, first_move, gplayer2);
    
    // Création du world
    struct world_t * world = createWorld();

    // Affichage des noms des joueurs
    printf(BOLD_TEXT "Nom du joueur 1: " BLUE_COLOR "%s\n" RESET_COLOR, player1.get_player_name()); 
    printf(BOLD_TEXT "Nom du joueur 2: " RED_COLOR "%s\n" RESET_COLOR, player2.get_player_name()); 

    // Tableau des joueurs
    struct player players[NUM_PLAYERS] = {player1, player2};
    int scores[NUM_PLAYERS] = {0};

    // Joueur courant
    int current_player = SERVER;

    // Premier coup jouer par le serveur
    updatingWorld(world, &first_move);

    // Boucle du jeu
    struct move_t current_move = first_move;
    int winner = SERVER;

    int j = 1;

    while (1) { //Je met ici 0 dans le while sinon a lieu un segfault au moment de l'appel à current_player->play(); 
        printf("Tour %d \n", j);
        // Récupérer le prochain joueur
        current_player = compute_next_player(current_player);

        // Obtenir la prochaine tuile à jouer
        struct tile_t next_tile = get_next_tile(current_gameconfig, world); // TODO : faire la fonction get_next_tile();

        // Jouer le mouvement avec le joueur actuel et la tuile actuelle
        current_move = players[current_player].play(current_move, next_tile);

        // Vérifier si le mouvement est invalide
        if (is_invalid(current_move, world) || current_move.player_id != current_player){   // TODO : implémenter is_invalid(), mamis je crois que arthur l'a fait dans player. 
            printf("invalide coup ?\n");
            break;
        }


        printf("Coups valide!\n");

        // Mise à jour du score
        scores[current_player] += compute_score(&current_move, world);

        // Mise à jour du world
        updatingWorld(world, &current_move);

        // Vérifier si le joueur courant a gagné
        int game_over = is_game_over(world, current_gameconfig);
        if (game_over == 2){
            if (current_player == WHITE) {
                winner = WHITE;
            } else {
                winner = BLACK;
            }
            break;
        } else if (game_over == 1){
            winner = scores[WHITE] > scores[BLACK] ? WHITE : scores[WHITE] == scores[BLACK] ? SERVER : BLACK; // who_win(); // A implémenter
            break;
        }

        j++;
    }

    // Affichage du gagnant
    if (winner == SERVER){
        printf("Egalité !\n Score des joueurs: %d \n", scores[WHITE]);
    }else{
        printf("Le gagnant est le joueur %s\n Score %s: %d\n Score %s: %d\n", players[winner].get_player_name(), players[WHITE].get_player_name(), scores[WHITE], players[BLACK].get_player_name(), scores[BLACK]);
    }

    // Libération de la mémoire utilisé par les players, et leurs bibliothèques
    player1.finalize();
    player2.finalize();
    dlclose(player1.library);
    dlclose(player2.library); 

    deck_free(current_gameconfig.deck);

    deleteWorld(world);
    
    return 0;
}