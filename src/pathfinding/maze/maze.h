#ifndef MAZE_H
#define MAZE_H

#include <config.h>

enum PassageState : uint8_t {
    UNDETERMINED, 
    OPEN, 
    BLOCKED
};

struct Square {
    PassageState up; 
    PassageState down; 
    PassageState left; 
    PassageState right;
};

struct Maze {
    Square squares[ROWS][COLUMNS];
};

/**
 * Cree la matrice de depart: tous les passages sont indetermines,
 * sauf les murs lateraux connus des rangees impaires qui sont bloques.
 *
 * @return Matrice initialisee
 */
Maze generateInitMaze();

/**
 * Marque comme bloque le mur devant la position donnee,
 * des deux cotes (case courante et case voisine).
 *
 * @param maze Matrice a modifier
 * @param position Case et orientation qui designent le mur
 */
void blockWall(Maze& maze, const CurrentPosition& position);

/**
 * Marque comme ouvert le passage devant la position donnee,
 * des deux cotes (case courante et case voisine).
 *
 * @param maze Matrice a modifier
 * @param position Case et orientation qui designent le passage
 */
void openWall(Maze& maze, const CurrentPosition& position);

/**
 * Lit l'etat d'un mur d'une case. Les bords du parcours sont toujours bloques.
 *
 * @param maze Matrice a consulter
 * @param row Rangee de la case
 * @param column Colonne de la case
 * @param side Cote de la case a lire
 * @return Etat du passage (UNDETERMINED, OPEN ou BLOCKED)
 */
PassageState getWall(const Maze& maze, uint8_t row, uint8_t column, Orientation side);

#endif