#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define CELLSIDELENGTH 30

#define MAX_ROWS 20
#define MAX_COLUMNS 24
#define MAX_MINES 99

typedef struct cell
{
    int x, y;
    int width, height;
    int value;
	int isVisible;
    int isFlag;
    struct cell *neighbors[8];
	int neighborCount;
} cell;

typedef struct board
{
    int width, height;
    cell cells[MAX_ROWS][MAX_COLUMNS];
	cell *mines[MAX_MINES];
    int mineCount;
    int safeCellsRemaining;
    int isLost;
} board;

board *initGameBoard(int rows, int columns, int mines);

void setCellVisible(board *gameBoard, cell *cell);

void setGameBoardMines(board *gameBoard, cell *currentCell);

#endif
