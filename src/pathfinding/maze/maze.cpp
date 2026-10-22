#include <pathfinding/maze/maze.h>

Maze generateInitMaze() {
    Maze maze{};

    for (uint8_t row = 1; row < ROWS; row += 2) {
        maze.squares[row][0].right = BLOCKED;
        maze.squares[row][1].left  = BLOCKED;
        maze.squares[row][1].right = BLOCKED;
        maze.squares[row][2].left  = BLOCKED;
    }

    return maze;
}

void setWall(Maze& maze, const CurrentPosition& position, PassageState state) {
    int row = position.row;
    int column = position.column;

    if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS)
        return;

    switch (position.direction) {
        case NORTH:
            maze.squares[row][column].up = state;

            if (row > 0)
                maze.squares[row - 1][column].down = state;

            break;

        case SOUTH:
            maze.squares[row][column].down = state;

            if (row < ROWS - 1)
                maze.squares[row + 1][column].up = state;

            break;

        case WEST:
            maze.squares[row][column].left = state;

            if (column > 0)
                maze.squares[row][column - 1].right = state;

            break;

        case EAST:
            maze.squares[row][column].right = state;

            if (column < COLUMNS - 1)
                maze.squares[row][column + 1].left = state;

            break;
    }
}

void blockWall(Maze& maze, const CurrentPosition& position) {
    setWall(maze, position, BLOCKED);
}

void openWall(Maze& maze, const CurrentPosition& position) {
    setWall(maze, position, OPEN);
}

PassageState getWall(const Maze& maze, uint8_t row, uint8_t column, Orientation side) {
    const Square& square = maze.squares[row][column];

    // Les bords du parcours sont toujours bloques
    if (side == NORTH) {
        if (row > 0) {
            return square.up;
        }
        return BLOCKED;
    }

    if (side == SOUTH) {
        if (row < ROWS - 1) {
            return square.down;
        }
        return BLOCKED;
    }

    if (side == WEST) {
        if (column > 0) {
            return square.left;
        }
        return BLOCKED;
    }

    if (side == EAST) {
        if (column < COLUMNS - 1) {
            return square.right;
        }
        return BLOCKED;
    }

    return BLOCKED;
}
