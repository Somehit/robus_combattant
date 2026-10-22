#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <config.h>
#include <pathfinding/maze/maze.h>
#include <infrared/infrared.h>
#include <motor/motor.h>


constexpr float SQUARE_SIZE_METERS = 0.5f;

constexpr uint8_t OPEN_FORWARD = 0b100;
constexpr uint8_t OPEN_LEFT    = 0b010;
constexpr uint8_t OPEN_RIGHT   = 0b001;

// Nombre maximal de mouvements gardes dans l'historique
constexpr int MAX_HISTORY = 128;

/**
 * Initialise la matrice et place le robot a sa position de depart
 * (rangee 0, colonne 1, oriente vers le sud).
 */
void NavigationInit();

/**
 * Donne la position actuelle du robot dans la matrice.
 *
 * @return Copie de la position (rangee, colonne, orientation)
 */
CurrentPosition getPosition();

/**
 * Donne acces a la matrice construite pendant le parcours.
 *
 * @return Reference constante vers la matrice
 */
const Maze& getMaze();

/**
 * Verifie si le robot est arrive sur la derniere rangee.
 *
 * @return true si le robot est sur la derniere rangee
 */
bool hasReachedEnd();

/**
 * Avance d'une case dans la direction actuelle et met la position a jour.
 */
int moveForwardOneSquare();

/**
 * Tourne d'un quart de tour a gauche et met l'orientation a jour.
 */
void turnLeft();

/**
 * Tourne d'un quart de tour a droite et met l'orientation a jour.
 */
void turnRight();

/**
 * Lit le capteur infrarouge et met a jour le mur devant le robot dans la matrice.
 *
 * @return true si un obstacle est detecte devant le robot
 */
bool scanFront();

/**
 * Calcule les directions relatives au robot qui le rapprochent de la derniere rangee:
 * - jamais vers le nord
 * - si le sud est ouvert, seulement la direction qui mene au sud
 * - sinon, seulement les cotes ou un passage vers le sud est encore possible
 *
 * @param maze Matrice a consulter
 * @param position Position et orientation du robot
 * @return Masque 0b[forward][left][right] (OPEN_FORWARD, OPEN_LEFT, OPEN_RIGHT),
 *         0 si le robot est sur la derniere rangee ou si le chemin est derriere lui
 */
uint8_t getOpenDirections(const Maze& maze, const CurrentPosition& position);

/**
 * Scanne devant, a gauche et a droite pour mettre la matrice a jour.
 * Ne tourne pas vers le nord ni vers un mur deja connu, et ne regarde pas
 * les cotes si le sud est devant et ouvert. Le robot revient toujours
 * a son orientation de depart.
 */
void scanSurroundingAndUpdateMatrice();

/**
 * Choisit un chemin avec la matrice a jour et avance d'une case.
 * Priorite: devant, puis gauche, puis droite.
 */
void moveToNextSquare();

void moveFromPosRobot(int column);

/**
 * Ajoute un mouvement a l'historique (sauf pendant le retour).
 * Un virage suivi du virage inverse s'annule et n'est pas garde.
 *
 * @param action Mouvement effectue
 */
void setHistory(Action action);

/**
 * Donne l'historique des mouvements, du plus ancien au plus recent.
 *
 * @return Tableau de getHistoryCount() mouvements
 */
Action* getHistory();

int getHistoryCount();

/**
 * Fait demi-tour puis rejoue l'historique a l'envers pour revenir
 * au point de depart. La position et l'orientation restent a jour.
 * L'historique est vide a la fin.
 */
void reverseAllMovesInHistory();

#endif
