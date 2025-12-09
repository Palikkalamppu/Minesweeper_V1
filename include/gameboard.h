#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define CELLSIDELENGTH 30

typedef struct cell
{
    int x, y;
    int width, height;
    int value;
	int isVisible;
    int isFlag;
    struct cell **neighbors;
	int neighborCount;
} cell;

typedef struct board
{
    int width, height;
    cell **cells;
	cell **mines;
    int mineCount;
    int safeCellsRemaining;
    int isLost;
} board;

board *initGameBoard(int rows, int columns, int mines);

void setCellVisible(board *gameBoard, cell *cell);

#endif
