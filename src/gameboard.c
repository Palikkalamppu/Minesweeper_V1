#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameboard.h"



void setCellNeighbors(board *gameBoard, cell *cell)
{
    int x = cell->x;
    int y = cell->y;

	int gameBoardWidth = gameBoard->width / CELLSIDELENGTH;
	int gameBoardHeigth = gameBoard->height / CELLSIDELENGTH;

	cell->neighbors = calloc(8, sizeof(*cell->neighbors));
	if (cell->neighbors == NULL)
	{
		printf("Couldn't calloc cell's neighbors.\n");
		free(cell->neighbors);
		exit(0);
	}


	int index = 0;
	for (int i = y - 1; i <= y + 1; i++)
	{
		for (int j = x - 1; j <= x + 1; j++)
		{
			if (i < 0 || j < 0 || i > gameBoardHeigth - 1 || j > gameBoardWidth - 1 || (i == y && j == x))
				continue;
			else
            {
				cell->neighbors[index] = &gameBoard->cells[i][j];
				index++;
            }
		}
	}

	cell->neighborCount = index;

}

void setGameBoardMines(board *gameBoard)
{
    int rows = gameBoard->height / CELLSIDELENGTH;
    int columns = gameBoard->width / CELLSIDELENGTH;

    int minesRemaining = gameBoard->mineCount;
	while (minesRemaining)
	{
		int randomX = rand() % columns;
		int randomY = rand() % rows;

		if (gameBoard->cells[randomY][randomX].value != 10)
		{	
			gameBoard->cells[randomY][randomX].value = 10;
			gameBoard->mines[gameBoard->mineCount - minesRemaining] = &gameBoard->cells[randomY][randomX];
			minesRemaining--;
		}
	}

    for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (gameBoard->cells[i][j].value != 10)
			{
				int mineNeighborCount = 0;

				for (int k = 0; k < gameBoard->cells[i][j].neighborCount; k++)
				{
					if (gameBoard->cells[i][j].neighbors[k]->value == 10)
						mineNeighborCount++;
				}	

				gameBoard->cells[i][j].value = mineNeighborCount;
			}
		}
	}
}


board *initGameBoard(int rows, int columns, int mines)
{
	srand(time(NULL));

    board *newGameBoard = calloc(1, sizeof(board));
    if (newGameBoard == NULL)
    {
        printf("Couldn't calloc game board.\n");
        free(newGameBoard);
        exit(0);
    }

    newGameBoard->width = CELLSIDELENGTH * columns;
    newGameBoard->height = CELLSIDELENGTH * rows;
    newGameBoard->mineCount = mines;
    newGameBoard->safeCellsRemaining = rows * columns - mines;
    newGameBoard->isLost = 0;

    newGameBoard->cells = calloc(rows, sizeof(cell));
	if (newGameBoard->cells == NULL)
	{
		printf("Couldn't calloc game board's rows.\n");
		free(newGameBoard->cells);
		exit(0);
	}

    for (int i = 0; i < rows; i++)
    {
        newGameBoard->cells[i] = calloc(columns, sizeof(cell));
		if (newGameBoard->cells[i] == NULL)
		{
			printf("Couldn't calloc game board's columns.\n");
			free(newGameBoard->cells[i]);
			exit(0);
		}
    }

	newGameBoard->mines = calloc(mines, sizeof(cell));
	if (newGameBoard->mines == NULL)
	{
		printf("Couldn't calloc mines.\n");
		free(newGameBoard->mines);
		exit(0);
	}

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            newGameBoard->cells[y][x].x = x;
            newGameBoard->cells[y][x].y = y;
            newGameBoard->cells[y][x].width = CELLSIDELENGTH;
            newGameBoard->cells[y][x].height = CELLSIDELENGTH;
            newGameBoard->cells[y][x].value = 0;
			newGameBoard->cells[y][x].isVisible = 0;
            newGameBoard->cells[y][x].isFlag = 0;
            setCellNeighbors(newGameBoard, &newGameBoard->cells[y][x]);
        }
    }

    setGameBoardMines(newGameBoard);

    return newGameBoard;
}


void setCellVisible(board *gameBoard, cell *cell)
{
    if (cell->isFlag)
        return;

	if (cell->value == 10)
	{
        for (int k = 0; k < gameBoard->mineCount; k++)
            gameBoard->mines[k]->isVisible = 1;
	}
	else
	{
		cell->isVisible = 1;
        gameBoard->safeCellsRemaining--;
		
        if (!cell->value)
        {
            for (int k = 0; k < cell->neighborCount; k++)
            {	
                if (!cell->neighbors[k]->isVisible)
                    setCellVisible(gameBoard, cell->neighbors[k]);
            }
        }
	}
}