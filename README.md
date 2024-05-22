# Sujet de programmation impérative : Projet Carcassonne

La page du sujet :

https://www.labri.fr/perso/renault/working/teaching/projets/2023-24-S6-C-Carcassonne.php

La page sur thor :

https://thor.enseirb-matmeca.fr/ruby/projects/pr105-carc

---

## Description
Le projet Carcassonne vise à implémenter les règles du jeu de société Carcassonne en utilisant le langage C. Carcassonne est un jeu où les joueurs marquent des points en construisant des édifices tels que des routes, des châteaux, et des monastères. L'objectif est d'optimiser les possibilités en fonction des cartes disponibles, de terminer ses propres constructions et même de perturber celles des adversaires.

## Prérequis
- Le projet est développé en C assurez vous de pouvoir le compiler. 
- Le projet utilise la bibliothèque `igraph`. 

---

## Compilation et exécution

### Compilation
   ```
   make install
   ```
   Cette commande compile le projet et génère les exécutables `server` et `alltests` dans le répertoire `install`.

### Exécution du serveur de jeu
   ```
   ./install/server -m [M] ./install/player1.so ./install/player2.so
   ```
   - L'option `-m` permet de spécifier le mode de jeu du plateau de jeu (comme décrit dans la section Modes de jeu).
   - `[M]` représente le mode de jeu choisi. `NO_MEEPLE`,`INFINITE_MEEPLE`,`FINITE_MEEPLE` sont les modes de jeu disponibles.
   - Les joueurs sont spécifiés en tant que paramètres dans n'importe quel ordre.
   - Le serveur détermine l'ordre de jeu des joueurs.

### Exécution des tests
   ```
   make test
   ```


---

## Auteurs
Ce projet a été développé par Bouyer Kerrian, Genetet Maud, Dufetrelle Arthur et Larragueta César.


