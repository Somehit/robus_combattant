# Fonctions disponibles

Liste des fonctions publiques declarees dans les fichiers `.h` du projet.

## Sommaire

| Module | Fichier | Fonctions |
|---|---|---|
| Config | `src/config.h` | `leftOf`, `rightOf` |
| Infrarouge | `src/infrared/infrared.h` | `InfraredInit`, `isObstacle` |
| Moteurs | `src/motor/motor.h` | `MotorsInit`, `moveForward`, `rotate` |
| Matrice | `src/pathfinding/maze/maze.h` | `generateInitMaze`, `blockWall`, `openWall`, `getWall` |
| Navigation | `src/pathfinding/navigation/navigation.h` | `NavigationInit`, `getPosition`, `getMaze`, `hasReachedEnd`, `moveForwardOneSquare`, `turnLeft`, `turnRight`, `scanFront`, `scanPaths`, `getOpenDirections`, `moveToNextSquare` |

## Types utiles

| Type | Fichier | Valeurs / champs |
|---|---|---|
| `Orientation` | `config.h` | `NORTH`, `EAST`, `SOUTH`, `WEST` |
| `CurrentPosition` | `config.h` | `row`, `column`, `direction` |
| `RotationDirection` | `motor.h` | `ROTATE_LEFT`, `ROTATE_RIGHT` |
| `PassageState` | `maze.h` | `UNDETERMINED`, `OPEN`, `BLOCKED` |
| `Square` | `maze.h` | `up`, `down`, `left`, `right` |
| `Maze` | `maze.h` | `squares[ROWS][COLUMNS]` |

---

## Config — `src/config.h`

### `Orientation leftOf(Orientation direction)`
Calcule l'orientation obtenue apres un quart de tour a gauche.
- **@param** `direction` : orientation de depart
- **@return** orientation a gauche de la direction donnee

### `Orientation rightOf(Orientation direction)`
Calcule l'orientation obtenue apres un quart de tour a droite.
- **@param** `direction` : orientation de depart
- **@return** orientation a droite de la direction donnee

---

## Infrarouge — `src/infrared/infrared.h`

### `void InfraredInit()`
Configure les broches des capteurs infrarouges vert et rouge en entree.

### `bool isObstacle()`
Lit les capteurs infrarouges pour detecter un obstacle devant le robot.
- **@return** `true` si au moins un des deux capteurs detecte un obstacle

---

## Moteurs — `src/motor/motor.h`

### `void MotorsInit()`
Remet les encodeurs a zero, arrete les moteurs et choisit les reglages (A ou B) selon la broche d'identification du robot.

### `void moveForward(float distanceMeters)`
Avance en ligne droite avec acceleration, correction PID et deceleration. Bloque jusqu'a ce que la distance soit parcourue.
- **@param** `distanceMeters` : distance a parcourir en metres

### `void rotate(RotationDirection direction)`
Tourne le robot sur place d'un quart de tour. Bloque jusqu'a ce que la rotation soit terminee.
- **@param** `direction` : sens de rotation (`ROTATE_LEFT` ou `ROTATE_RIGHT`)

> `moveForward` et `rotate` ne mettent **pas** la position a jour. Pour se deplacer dans la matrice, utiliser `moveForwardOneSquare`, `turnLeft` et `turnRight` du module Navigation.

---

## Matrice — `src/pathfinding/maze/maze.h`

### `Maze generateInitMaze()`
Cree la matrice de depart : tous les passages sont indetermines, sauf les murs lateraux connus des rangees impaires qui sont bloques.
- **@return** matrice initialisee

### `void blockWall(Maze& maze, const CurrentPosition& position)`
Marque comme bloque le mur devant la position donnee, des deux cotes (case courante et case voisine).
- **@param** `maze` : matrice a modifier
- **@param** `position` : case et orientation qui designent le mur

### `void openWall(Maze& maze, const CurrentPosition& position)`
Marque comme ouvert le passage devant la position donnee, des deux cotes (case courante et case voisine).
- **@param** `maze` : matrice a modifier
- **@param** `position` : case et orientation qui designent le passage

### `PassageState getWall(const Maze& maze, uint8_t row, uint8_t column, Orientation side)`
Lit l'etat d'un mur d'une case. Les bords du parcours sont toujours bloques.
- **@param** `maze` : matrice a consulter
- **@param** `row` : rangee de la case
- **@param** `column` : colonne de la case
- **@param** `side` : cote de la case a lire
- **@return** etat du passage (`UNDETERMINED`, `OPEN` ou `BLOCKED`)

---

## Navigation — `src/pathfinding/navigation/navigation.h`

Ce module garde la matrice et la position du robot. Constantes : `SQUARE_SIZE_METERS` (0.5 m), `OPEN_FORWARD` (`0b100`), `OPEN_LEFT` (`0b010`), `OPEN_RIGHT` (`0b001`).

### `void NavigationInit()`
Initialise la matrice et place le robot a sa position de depart (rangee 0, colonne 1, oriente vers le sud).

### `CurrentPosition getPosition()`
Donne la position actuelle du robot dans la matrice.
- **@return** copie de la position (rangee, colonne, orientation)

### `const Maze& getMaze()`
Donne acces a la matrice construite pendant le parcours.
- **@return** reference constante vers la matrice

### `bool hasReachedEnd()`
Verifie si le robot est arrive sur la derniere rangee.
- **@return** `true` si le robot est sur la derniere rangee

### `void moveForwardOneSquare()`
Avance d'une case dans la direction actuelle et met la position a jour.

### `void turnLeft()`
Tourne d'un quart de tour a gauche et met l'orientation a jour.

### `void turnRight()`
Tourne d'un quart de tour a droite et met l'orientation a jour.

### `bool scanFront()`
Lit le capteur infrarouge et met a jour le mur devant le robot dans la matrice.
- **@return** `true` si un obstacle est detecte devant le robot

### `void scanPaths()`
Scanne devant, a gauche et a droite pour mettre la matrice a jour. Ne tourne pas vers le nord ni vers un mur deja connu, et ne regarde pas les cotes si le sud est devant et ouvert. Le robot revient toujours a son orientation de depart.

### `uint8_t getOpenDirections(const Maze& maze, const CurrentPosition& position)`
Calcule les directions relatives au robot qui le rapprochent de la derniere rangee :
- jamais vers le nord
- si le sud est ouvert, seulement la direction qui mene au sud
- sinon, seulement les cotes ou un passage vers le sud est encore possible

- **@param** `maze` : matrice a consulter
- **@param** `position` : position et orientation du robot
- **@return** masque `0b[forward][left][right]` (`OPEN_FORWARD`, `OPEN_LEFT`, `OPEN_RIGHT`), `0` si le robot est sur la derniere rangee ou si le chemin est derriere lui

### `void moveToNextSquare()`
Choisit un chemin avec la matrice a jour et avance d'une case. Priorite : devant, puis gauche, puis droite.

---

## Exemple d'utilisation

```cpp
void setup() {
    BoardInit();
    MotorsInit();
    InfraredInit();
    NavigationInit();
}

void loop() {
    if (hasReachedEnd()) {
        return;
    }

    scanPaths();
    moveToNextSquare();
}
```
