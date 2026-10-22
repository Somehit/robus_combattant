#include <pathfinding/navigation/navigation.h>

Maze maze;
CurrentPosition currentPosition;

Action history[MAX_HISTORY];
int historyCount = 0;

// Faux pendant le retour pour que les mouvements rejoues ne soient pas enregistres
bool isRecordingHistory = true;

void setPosition(int row, int column)
{
    currentPosition.row = constrain(row, 0, ROWS - 1);
    currentPosition.column = constrain(column, 0, COLUMNS - 1);
}

void NavigationInit()
{
    maze = generateInitMaze();
    setPosition(0, 1);
    currentPosition.direction = SOUTH;
    historyCount = 0;
    isRecordingHistory = true;
}

CurrentPosition getPosition()
{
    return currentPosition;
}

const Maze &getMaze()
{
    return maze;
}

bool hasReachedEnd()
{
    return currentPosition.row >= ROWS - 1;
}

int moveForwardOneSquare()
{
    moveForward(SQUARE_SIZE_METERS);
    setHistory(MOVE_FORWARD);

    switch (currentPosition.direction)
    {
    case NORTH:
        setPosition(currentPosition.row - 1, currentPosition.column);
        break;

    case EAST:
        setPosition(currentPosition.row, currentPosition.column + 1);
        break;

    case SOUTH:
        setPosition(currentPosition.row + 1, currentPosition.column);
        break;

    case WEST:
        setPosition(currentPosition.row, currentPosition.column - 1);
        break;
    }

    return 0;
}

void turnRight()
{
    rotate(ROTATE_RIGHT);
    currentPosition.direction = rightOf(currentPosition.direction);
    setHistory(TURN_RIGHT);
}

void turnLeft()
{
    rotate(ROTATE_LEFT);
    currentPosition.direction = leftOf(currentPosition.direction);
    setHistory(TURN_LEFT);
}

bool scanFront()
{
    delay(100);
    bool obstacleDetected = isObstacle();

    if (obstacleDetected)
    {
        blockWall(maze, currentPosition);
    }
    else
    {
        openWall(maze, currentPosition);
    }

    return obstacleDetected;
}

bool isPassageOpen(const Maze &maze, uint8_t row, uint8_t column, Orientation side)
{
    return getWall(maze, row, column, side) != BLOCKED;
}

// Verifie si un passage vers le sud est encore possible en se deplacant vers un cote de la rangee
bool canReachSouth(const Maze &maze, uint8_t row, uint8_t column, Orientation side)
{
    int step;

    if (side == EAST)
    {
        step = 1;
    }
    else
    {
        step = -1;
    }

    while (isPassageOpen(maze, row, column, side))
    {
        column += step;

        if (isPassageOpen(maze, row, column, SOUTH))
        {
            return true;
        }
    }

    return false;
}

// Convertit une orientation absolue en direction relative au robot
uint8_t toRelativeDirection(Orientation facing, Orientation target)
{
    if (target == facing)
    {
        return OPEN_FORWARD;
    }

    if (target == leftOf(facing))
    {
        return OPEN_LEFT;
    }

    if (target == rightOf(facing))
    {
        return OPEN_RIGHT;
    }

    return 0;
}

uint8_t getOpenDirections(const Maze &maze, const CurrentPosition &position)
{
    uint8_t openDirections = 0;

    if (position.row >= ROWS - 1)
    {
        return 0;
    }

    if (isPassageOpen(maze, position.row, position.column, SOUTH))
    {
        return toRelativeDirection(position.direction, SOUTH);
    }

    if (canReachSouth(maze, position.row, position.column, EAST))
    {
        openDirections |= toRelativeDirection(position.direction, EAST);
    }

    if (canReachSouth(maze, position.row, position.column, WEST))
    {
        openDirections |= toRelativeDirection(position.direction, WEST);
    }

    return openDirections;
}

void scanSurroundingAndUpdateMatrice()
{
    bool frontBlocked = scanFront();

    // Le sud est devant et ouvert: pas besoin de regarder les cotes
    if (currentPosition.direction == SOUTH && !frontBlocked)
    {
        return;
    }

    Orientation left = leftOf(currentPosition.direction);
    Orientation right = rightOf(currentPosition.direction);

    // Inutile de tourner vers le nord ou vers un mur deja connu
    if (left != NORTH && getWall(maze, currentPosition.row, currentPosition.column, left) == UNDETERMINED)
    {
        turnLeft();
        scanFront();
        turnRight();
    }

    if (right != NORTH && getWall(maze, currentPosition.row, currentPosition.column, right) == UNDETERMINED)
    {
        turnRight();
        scanFront();
        turnLeft();
    }
}

void moveToNextSquare()
{
    uint8_t openPaths = getOpenDirections(maze, currentPosition);

    if (openPaths & OPEN_FORWARD)
    {
        moveForwardOneSquare();
    }
    else if (openPaths & OPEN_LEFT)
    {
        turnLeft();
        moveForwardOneSquare();
    }
    else if (openPaths & OPEN_RIGHT)
    {
        turnRight();
        moveForwardOneSquare();
    }
    else
    {
        // Aucun chemin devant ou sur les cotes: demi-tour
        turnRight();
        turnRight();
    }
}

void moveFromLeft()
{
    if (scanFront())
    { // si la case en avant est bloqué
        turnRight();
        if (!scanFront())
        { // si on peut aller a la case à droite (au milieu)
            moveForwardOneSquare();
            turnLeft(); // le robot regarde en avant
            if (!scanFront())
            {
                moveForwardOneSquare(); // si la case au milieu peut avancer
            }
            else
            { // on va a droite au complet
                turnRight();
                if (!scanFront())
                { // si on peut aller a la case à droite au complet
                    moveForwardOneSquare();
                    turnLeft(); // on regarde en avant
                    if (!scanFront())
                    {
                        moveForwardOneSquare(); // si la case au milieu peut avancer
                    }
                }
            }
        }
        else
        {
            turnLeft(); // la case à droite est bloquée, on regarde en avant
        }
    }
    else
    {
        moveForwardOneSquare();
    }
}

void moveFromCenter()
{
    if (scanFront())
    { // si la canse en avant est bloqué
        turnRight();

        if (!scanFront())
        { // si on peut aller a la case de gauche
            moveForwardOneSquare();
            turnLeft();
            if (!scanFront())
            {
                moveForwardOneSquare(); // si la case à droite on peut avance
            }
            else
            { // si le centre et gauche ne sont pas bon
                turnLeft();
                if (!scanFront())
                {
                    moveForwardOneSquare(); // retour au centre
                    if (!scanFront())
                    {
                        moveForwardOneSquare(); // si on peut avancer à gaucher
                        turnRight();
                        if (!scanFront())
                        {
                            moveForwardOneSquare(); // si on peut avancer tout droit
                        }
                    }
                }
            }
        }
        else
        {
            turnLeft(); // la case à droite est bloquée, on regarde en avant
            turnLeft();
            moveForwardOneSquare();
            turnRight();
        }
    }
    else
    {
        moveForwardOneSquare();
    }
}

void moveFromRight()
{
    if (scanFront())
    { // si la canse en avant est bloqué
        turnLeft();

        if (!scanFront())
        { // si on peut aller a la case de gauche
            moveForwardOneSquare();
            turnRight();

            if (!scanFront())
            {
                moveForwardOneSquare(); // si la case à droite on peut avance
            }
            else
            { // si le centre et gauche ne sont pas bon
                turnLeft();

                if (!scanFront())
                {
                    moveForwardOneSquare(); // si on peut avancer à gaucher
                    turnRight();

                    if (!scanFront())
                    {
                        moveForwardOneSquare(); // si on peut avancer tout droit
                    }
                }
            }
        }
        else
        {
            turnRight(); // la case à gauche est bloquée, on regarde en avant
        }
    }

    else
    {
        moveForwardOneSquare();
    }
}

void moveFromPosRobot(int column)
{
    bool versLeSud = (getPosition().direction == SOUTH);

    switch (column)
    {
    case 0: // colonne 0 : à droite du robot quand il va vers le sud
        if (versLeSud)
            moveFromRight();
        else
            moveFromLeft();
        break;
    case 1: // si on commence au milieu
        moveFromCenter();
        break;
    case 2: // colonne 2 : à gauche du robot quand il va vers le sud
        if (versLeSud)
            moveFromLeft();
        else
            moveFromRight();
        break;
    default:
        break;
    }
}

// Retourne le virage inverse (gauche <-> droite)
Action oppositeTurn(Action action)
{
    if (action == TURN_LEFT)
    {
        return TURN_RIGHT;
    }

    return TURN_LEFT;
}

void setHistory(Action action)
{
    if (!isRecordingHistory)
    {
        return;
    }

    // Un virage suivi du virage inverse (ex: tourner pour scanner puis revenir)
    // s'annule: on retire le dernier virage au lieu d'enregistrer les deux
    bool isTurn = (action == TURN_LEFT || action == TURN_RIGHT);

    if (isTurn && historyCount > 0 && history[historyCount - 1] == oppositeTurn(action))
    {
        historyCount--;
        return;
    }

    if (historyCount >= MAX_HISTORY)
    {
        return;
    }

    history[historyCount] = action;
    historyCount++;
}

Action *getHistory()
{
    return history;
}

int getHistoryCount()
{
    return historyCount;
}

void reverseAllMovesInHistory()
{
    // Ne pas enregistrer les mouvements du retour
    isRecordingHistory = false;

    // Demi-tour pour faire face au chemin deja parcouru
    turnRight();
    turnRight();

    // Rejouer les mouvements du plus recent au plus ancien,
    // en inversant le sens des virages
    for (int i = historyCount - 1; i >= 0; i--)
    {
        if (history[i] == MOVE_FORWARD)
        {
            moveForwardOneSquare();
        }
        else if (history[i] == TURN_LEFT)
        {
            turnRight();
        }
        else if (history[i] == TURN_RIGHT)
        {
            turnLeft();
        }
        else if (currentPosition.column == 1 && currentPosition.row == 0) {
            return;
        }
    }

    historyCount = 0;
    isRecordingHistory = true;
}
