#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "gameboard.h"


typedef enum GameScreen { STARTSCREEN = 0, GAMEBOARDSCREEN } GameScreen;

typedef struct  
{
    int rows;
    int columns;
    int mines;
} DifficultyLevel;

DifficultyLevel EASY = {.rows = 8, .columns = 10, .mines = 10};
DifficultyLevel MEDIUM = {.rows = 14, .columns = 18, .mines = 40};
DifficultyLevel HARD = {.rows = 20, .columns = 24, .mines = 99};

int gameWon = 0;
int gameLost = 0;

float seconds = 0;

board *initNewGame(board **gameBoard, DifficultyLevel difficultyLevel)
{
    gameLost = 0;
    gameWon = 0;
    seconds = 0;

    int rows = (*gameBoard)->height / CELLSIDELENGTH;
    int columns = (*gameBoard)->width / CELLSIDELENGTH;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            free((*gameBoard)->cells[i][j].neighbors);
        }
        free((*gameBoard)->cells[i]);
    }

    free((*gameBoard)->cells);
    free((*gameBoard)->mines);
    free(*gameBoard);

    return initGameBoard(difficultyLevel.rows, difficultyLevel.columns, difficultyLevel.mines);
}

void DrawDifficultyLevelButtons(Rectangle easy, Rectangle medium, Rectangle hard, int strokeThickness)
{
    DrawRectangleRec(easy, LIME);
    DrawRectangleLinesEx(easy, strokeThickness, BLACK);
    DrawRectangleRec(medium, YELLOW);
    DrawRectangleLinesEx(medium, strokeThickness, BLACK);
    DrawRectangleRec(hard, RED);
    DrawRectangleLinesEx(hard, strokeThickness, BLACK);
}

int main(void)
{
    board *gameBoard = initGameBoard(MEDIUM.rows, MEDIUM.columns, MEDIUM.mines);

    int screenWidth = 1280;
    int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Mine Sweeper");

    Font font = LoadFontEx("resources/DIN Alternate Bold.ttf", 100, 0, 0);

    GameScreen currentScreen = STARTSCREEN;

    int diffivultyLevelBtnOffsetY = 120;

    int buttonWidth = 150;
    int buttonHeight = 120;

    Rectangle easy = (Rectangle) {screenWidth / 2 - buttonWidth * 1.75, screenHeight / 2 + diffivultyLevelBtnOffsetY, buttonWidth, buttonHeight};
    Rectangle medium = (Rectangle) {screenWidth / 2 - buttonWidth / 2, screenHeight / 2 + diffivultyLevelBtnOffsetY, buttonWidth, buttonHeight};
    Rectangle hard = (Rectangle) {screenWidth / 2 + buttonWidth * 0.75, screenHeight / 2 + diffivultyLevelBtnOffsetY, buttonWidth, buttonHeight};

	SetTargetFPS(30);

	Vector2 mousePosition;

    int minesRemaining = gameBoard->mineCount;

	while (!WindowShouldClose())
	{

        int gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
        int gameBoardRows = gameBoard->height / CELLSIDELENGTH;
        int offsetX = (screenWidth  - gameBoard->width)  / 2;
        int offsetY = (screenHeight - gameBoard->height) / 2;

        switch (currentScreen)
        {
            case STARTSCREEN:
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        mousePosition = GetMousePosition();

                        if (CheckCollisionPointRec(mousePosition, easy))
                        {
                            minesRemaining = EASY.mines;
                            gameBoard = initNewGame(&gameBoard, EASY);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;

                            currentScreen = GAMEBOARDSCREEN;
                        }

                        else if (CheckCollisionPointRec(mousePosition, medium))
                        {
                            minesRemaining = MEDIUM.mines;
                            gameBoard = initNewGame(&gameBoard, MEDIUM);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;

                            currentScreen = GAMEBOARDSCREEN;
                        }

                        else if (CheckCollisionPointRec(mousePosition, hard))
                        {
                            minesRemaining = HARD.mines;
                            gameBoard = initNewGame(&gameBoard, HARD);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;

                            currentScreen = GAMEBOARDSCREEN;
                        }
                    }
            } break;

            case GAMEBOARDSCREEN:
            {
                
                if (gameBoard->safeCellsRemaining == 0)
                {
                    gameWon = 1;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        mousePosition = GetMousePosition();

                        if (CheckCollisionPointRec(mousePosition, easy))
                        {
                            minesRemaining = EASY.mines;
                            gameBoard = initNewGame(&gameBoard, EASY);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;
                        }

                        else if (CheckCollisionPointRec(mousePosition, medium))
                        {
                            minesRemaining = MEDIUM.mines;
                            gameBoard = initNewGame(&gameBoard, MEDIUM);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;
                        }

                        else if (CheckCollisionPointRec(mousePosition, hard))
                        {
                            minesRemaining = HARD.mines;
                            gameBoard = initNewGame(&gameBoard, HARD);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;
                        }
                    }
                }
                else if (gameBoard->isLost)
                {
                    gameLost = 1;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        mousePosition = GetMousePosition();

                        if (CheckCollisionPointRec(mousePosition, easy))
                        {
                            minesRemaining = EASY.mines;
                            gameBoard = initNewGame(&gameBoard, EASY);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;
                        }

                        else if (CheckCollisionPointRec(mousePosition, medium))
                        {
                            minesRemaining = MEDIUM.mines;
                            gameBoard = initNewGame(&gameBoard, MEDIUM);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;
                        }

                        else if (CheckCollisionPointRec(mousePosition, hard))
                        {
                            minesRemaining = HARD.mines;
                            gameBoard = initNewGame(&gameBoard, HARD);
                            
                            gameBoardColumns = gameBoard->width / CELLSIDELENGTH;
                            gameBoardRows = gameBoard->height / CELLSIDELENGTH;
                            offsetX = (screenWidth  - gameBoard->width)  / 2;
                            offsetY = (screenHeight - gameBoard->height) / 2;
                        }
                    }
                }
                else
                {
                    seconds += GetFrameTime();

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        mousePosition = GetMousePosition();
                        
                        for (int i = 0; i < gameBoardRows; i++)
                        {
                            for (int j = 0; j < gameBoardColumns; j++)
                            {
                                cell currentCell = gameBoard->cells[i][j];
                                int currentX = currentCell.x;
                                int currentY = currentCell.y;

                                Rectangle currentRec = (Rectangle) {offsetX + currentX * CELLSIDELENGTH, offsetY + currentY * CELLSIDELENGTH, CELLSIDELENGTH, CELLSIDELENGTH};
                                
                                if (CheckCollisionPointRec(mousePosition, currentRec) && !gameBoard->cells[i][j].isVisible && !currentCell.isFlag)
                                    setCellVisible(gameBoard, &gameBoard->cells[i][j]);
                            }
                        }	
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_SPACE))
                    {
                        mousePosition = GetMousePosition();
                        
                        for (int i = 0; i < gameBoardRows; i++)
                        {
                            for (int j = 0; j < gameBoardColumns; j++)
                            {
                                cell currentCell = gameBoard->cells[i][j];
                                int currentX = currentCell.x;
                                int currentY = currentCell.y;

                                Rectangle currentRec = (Rectangle) {offsetX + currentX * CELLSIDELENGTH, offsetY + currentY * CELLSIDELENGTH, CELLSIDELENGTH, CELLSIDELENGTH};
                                
                                if (CheckCollisionPointRec(mousePosition, currentRec))
                                {
                                    if (currentCell.isFlag)
                                    {
                                        gameBoard->cells[i][j].isFlag = 0;
                                        minesRemaining++;
                                    }
                                    else
                                    {                          
                                        gameBoard->cells[i][j].isFlag = 1;
                                        minesRemaining--;
                                    }                             
                                }
                            }
                        }	
                    }
                }
            } break;
        }
			
		BeginDrawing();
			
			ClearBackground(WHITE);

            switch (currentScreen)
            {
                case STARTSCREEN:
                {
                    const char *startText = "Minesweeper by PALIKKALAMPPU";
                    float fontSize = 50;
                    float textWidth = MeasureTextEx(font, startText, fontSize, 0.0f).x;
                    float textHeight = MeasureTextEx(font, startText, fontSize, 0.0f).y;
                    DrawTextEx(font, startText, (Vector2) {screenWidth / 2 - textWidth / 2, screenHeight / 2 - 150}, fontSize, 0.0f, BLACK);

                    const char *chooseText = "CHOOSE DIFFICULTY";
                    fontSize = 100;
                    textWidth = MeasureTextEx(font, chooseText, fontSize, 0.0f).x;
                    textHeight = MeasureTextEx(font, chooseText, fontSize, 0.0f).y;
                    DrawTextEx(font, chooseText, (Vector2) {screenWidth / 2 - textWidth / 2, screenHeight / 2 - textHeight / 2}, fontSize, 0.0f, BLACK);

                    DrawDifficultyLevelButtons(easy, medium, hard, 4);
                } break;

                case GAMEBOARDSCREEN:
                {
                    for (int i = 0; i < gameBoardRows; i++)
			        {
                        for (int j = 0; j < gameBoardColumns; j++)
                        {
                            cell currentCell = gameBoard->cells[i][j];
                            int currentX = currentCell.x;
                            int currentY = currentCell.y;
                            
                            DrawRectangleLines(offsetX + currentX * CELLSIDELENGTH, offsetY + currentY * CELLSIDELENGTH, CELLSIDELENGTH, CELLSIDELENGTH, BLACK);

                            if (currentCell.isVisible && currentCell.value < 10)
                            {
                                DrawRectangle(offsetX + currentX * CELLSIDELENGTH, offsetY + currentY * CELLSIDELENGTH, CELLSIDELENGTH, CELLSIDELENGTH, GREEN);
                            
                                if (currentCell.value > 0)                 
                                    DrawTextEx(font, TextFormat("%d", currentCell.value), (Vector2) {offsetX + currentX * CELLSIDELENGTH + (CELLSIDELENGTH / 4), offsetY + currentY * CELLSIDELENGTH + (CELLSIDELENGTH / 4)}, CELLSIDELENGTH * 3/4, 0.0f, BLACK);

                            }
                            else if (currentCell.isFlag)
                            {
                                DrawCircle(offsetX + currentX * CELLSIDELENGTH + CELLSIDELENGTH / 2, offsetY + currentY * CELLSIDELENGTH + CELLSIDELENGTH / 2, CELLSIDELENGTH / 3, RED);
                            }

                            else if (currentCell.isVisible)
                            {
                                DrawCircle(offsetX + currentX * CELLSIDELENGTH + CELLSIDELENGTH / 2, offsetY + currentY * CELLSIDELENGTH + CELLSIDELENGTH / 2, CELLSIDELENGTH / 2.4, BLACK);
                                gameBoard->isLost = 1;
                            }
                        }
                    }

                    DrawTextEx(font, TextFormat("%03d", (int)seconds), (Vector2) {10, 0}, 70, 0.0f, BLACK);


                    const char *minesRemainingText = TextFormat("Mines: %d", minesRemaining);
                    int textWidth = MeasureTextEx(font, minesRemainingText, 70, 0.0f).x;
                    DrawTextEx(font, minesRemainingText, (Vector2) {screenWidth / 2 - textWidth / 2, 0}, 70, 0.0f, BLACK);
                    
                    if (gameWon)
                    {
                        const char *endText = "YOU WON!";
                        float textWidth = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).x;
                        float textHeight = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).y;
                        float padding = 5.0f;

                        DrawRectangle(screenWidth / 2 - textWidth / 2 - padding, screenHeight / 2 - textHeight / 2, textWidth + padding * 2, textHeight, WHITE);
                        DrawRectangleLines(screenWidth / 2 - textWidth / 2 - padding, screenHeight / 2 - textHeight / 2, textWidth + padding * 2, textHeight, BLACK);
                        DrawTextEx(font, endText, (Vector2) {screenWidth / 2 - textWidth / 2, screenHeight / 2 - textHeight / 2}, (float)font.baseSize, 0.0f, BLACK);

                        DrawDifficultyLevelButtons(easy, medium, hard, 4);
                    }

                    if (gameLost)
                    {
                        char *endText = "YOU LOST!";
                        float textWidth = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).x;
                        float textHeight = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).y;
                        float padding = 5.0f;

                        DrawRectangle(screenWidth / 2 - textWidth / 2 - padding, screenHeight / 2 - textHeight / 2, textWidth + padding * 2, textHeight, WHITE);
                        DrawRectangleLines(screenWidth / 2 - textWidth / 2 - padding, screenHeight / 2 - textHeight / 2, textWidth + padding * 2, textHeight, BLACK);
                        DrawTextEx(font, endText, (Vector2) {screenWidth / 2 - textWidth / 2, screenHeight / 2 - textHeight / 2}, (float)font.baseSize, 0.0f, BLACK);

                        DrawDifficultyLevelButtons(easy, medium, hard, 4);
                    }
                } break;
            }

				

	
		EndDrawing();
	}

    UnloadFont(font);

    int rows = gameBoard->height / CELLSIDELENGTH;
    int columns = gameBoard->width / CELLSIDELENGTH;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            free(gameBoard->cells[i][j].neighbors);
        }
        free(gameBoard->cells[i]);
    }

    free(gameBoard->cells);
    free(gameBoard->mines);

    free(gameBoard);

	CloseWindow();

    return 0;   
}