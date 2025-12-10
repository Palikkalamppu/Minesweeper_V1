#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "gameboard.h"

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

typedef enum GameScreen { STARTSCREEN = 0, GAMEBOARDSCREEN } GameScreen;

typedef struct  
{
    int rows;
    int columns;
    int mines;
} DifficultyLevel;

typedef struct
{
    int gameWon;
    int gameLost;
    float seconds;
    int minesRemaining;
    int firstCellOpened;
    DifficultyLevel currentDifficulty;
    GameScreen currentScreen;
    board *gameBoard;
} GameState;

GameState game = {0};

int offsetX;
int offsetY;

void initNewGame(DifficultyLevel difficultyLevel) 
{
    game.gameWon = 0;
    game.gameLost = 0;
    game.seconds = 0;
    game.minesRemaining = difficultyLevel.mines;
    game.firstCellOpened = 0;
    game.currentDifficulty = difficultyLevel;
    game.currentScreen = GAMEBOARDSCREEN;

    free(game.gameBoard);
    game.gameBoard = initGameBoard(difficultyLevel.rows, difficultyLevel.columns, difficultyLevel.mines);

    offsetX = (SCREENWIDTH  - game.gameBoard->width)  / 2;
    offsetY = (SCREENHEIGHT - game.gameBoard->height) / 2;
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

void guaranteeSafeOpening(cell *currentCell)
{
    if (game.firstCellOpened || currentCell->value < 10)
        setCellVisible(game.gameBoard, currentCell);
    else
    {
        int x = currentCell->x;
        int y = currentCell->y;

        free(game.gameBoard);
        game.gameBoard = initGameBoard(game.currentDifficulty.rows, game.currentDifficulty.columns, game.currentDifficulty.mines);
        
        guaranteeSafeOpening(&game.gameBoard->cells[y][x]);

		game.firstCellOpened = 1;
    }
}

int main(void)
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Mine Sweeper");

    DifficultyLevel EASY = {.rows = 8, .columns = 10, .mines = 10};
    DifficultyLevel MEDIUM = {.rows = 14, .columns = 18, .mines = 40};
    DifficultyLevel HARD = {.rows = MAX_ROWS, .columns = MAX_ROWS, .mines = MAX_MINES};

    Font font = LoadFontEx("resources/DIN Alternate Bold.ttf", 100, 0, 0);

    game.currentScreen = STARTSCREEN;

    int diffivultyLevelBtnOffsetY = 120;

    int buttonWidth = 150;
    int buttonHeight = 120;

    Rectangle easy = (Rectangle) {SCREENWIDTH / 2 - buttonWidth * 1.75, SCREENHEIGHT / 2 + diffivultyLevelBtnOffsetY, buttonWidth, buttonHeight};
    Rectangle medium = (Rectangle) {SCREENWIDTH / 2 - buttonWidth / 2, SCREENHEIGHT / 2 + diffivultyLevelBtnOffsetY, buttonWidth, buttonHeight};
    Rectangle hard = (Rectangle) {SCREENWIDTH / 2 + buttonWidth * 0.75, SCREENHEIGHT / 2 + diffivultyLevelBtnOffsetY, buttonWidth, buttonHeight};

	SetTargetFPS(30);

	while (!WindowShouldClose())
	{
        switch (game.currentScreen)
        {
            case STARTSCREEN:
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (CheckCollisionPointRec(GetMousePosition(), easy))
                        initNewGame(EASY);                
                    else if (CheckCollisionPointRec(GetMousePosition(), medium))
                        initNewGame(MEDIUM);
                    else if (CheckCollisionPointRec(GetMousePosition(), hard))
                        initNewGame(HARD);
                }
            } break;

            case GAMEBOARDSCREEN:
            {
                if (game.gameBoard->safeCellsRemaining == 0)
                {
                    game.gameWon = 1;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        if (CheckCollisionPointRec(GetMousePosition(), easy))
                            initNewGame(EASY);
                        else if (CheckCollisionPointRec(GetMousePosition(), medium))
                            initNewGame(MEDIUM);
                        else if (CheckCollisionPointRec(GetMousePosition(), hard))
                            initNewGame(HARD);
                    }
                }
                else if (game.gameBoard->isLost)
                {
                    game.gameLost = 1;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        if (CheckCollisionPointRec(GetMousePosition(), easy))
                            initNewGame(EASY);
                        else if (CheckCollisionPointRec(GetMousePosition(), medium))
                            initNewGame(MEDIUM);
                        else if (CheckCollisionPointRec(GetMousePosition(), hard))
                            initNewGame(HARD);
                    }
                }
                else
                {
                    game.seconds += GetFrameTime();

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        for (int i = 0; i < game.currentDifficulty.rows; i++)
                        {
                            for (int j = 0; j < game.currentDifficulty.columns; j++)
                            {
                                cell *currentCell = &game.gameBoard->cells[i][j];
                                int currentX = currentCell->x;
                                int currentY = currentCell->y;

                                Rectangle currentRec = (Rectangle) {offsetX + currentX * CELLSIDELENGTH, offsetY + currentY * CELLSIDELENGTH, CELLSIDELENGTH, CELLSIDELENGTH};
                                
                                if (CheckCollisionPointRec(GetMousePosition(), currentRec) && !currentCell->isVisible && !currentCell->isFlag)                                    
                                    guaranteeSafeOpening(&game.gameBoard->cells[i][j]);
                            }
                        }	

                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_SPACE))
                        for (int i = 0; i < game.currentDifficulty.rows; i++)
                        {
                            for (int j = 0; j < game.currentDifficulty.columns; j++)
                            {
                                cell *currentCell = &game.gameBoard->cells[i][j];
                                int currentX = currentCell->x;
                                int currentY = currentCell->y;

                                Rectangle currentRec = (Rectangle) {offsetX + currentX * CELLSIDELENGTH, offsetY + currentY * CELLSIDELENGTH, CELLSIDELENGTH, CELLSIDELENGTH};
                                
                                if (CheckCollisionPointRec(GetMousePosition(), currentRec))
                                {
                                    if (currentCell->isFlag)
                                    {
                                        game.gameBoard->cells[i][j].isFlag = 0;
                                        game.minesRemaining++;
                                    }
                                    else
                                    {                          
                                        game.gameBoard->cells[i][j].isFlag = 1;
                                        game.minesRemaining--;
                                    }                             
                                }
                            }
                        }	
                }
            } break;
        }
			
		BeginDrawing();
			
			ClearBackground(WHITE);

            switch (game.currentScreen)
            {
                case STARTSCREEN:
                {
                    const char *startText = "Minesweeper by PALIKKALAMPPU";
                    float fontSize = 50;
                    float textWidth = MeasureTextEx(font, startText, fontSize, 0.0f).x;
                    float textHeight = MeasureTextEx(font, startText, fontSize, 0.0f).y;
                    DrawTextEx(font, startText, (Vector2) {SCREENWIDTH / 2 - textWidth / 2, SCREENHEIGHT / 2 - 150}, fontSize, 0.0f, BLACK);

                    const char *chooseText = "CHOOSE DIFFICULTY";
                    fontSize = 100;
                    textWidth = MeasureTextEx(font, chooseText, fontSize, 0.0f).x;
                    textHeight = MeasureTextEx(font, chooseText, fontSize, 0.0f).y;
                    DrawTextEx(font, chooseText, (Vector2) {SCREENWIDTH / 2 - textWidth / 2, SCREENHEIGHT / 2 - textHeight / 2}, fontSize, 0.0f, BLACK);

                    DrawDifficultyLevelButtons(easy, medium, hard, 4);
                } break;

                case GAMEBOARDSCREEN:
                {
                    for (int i = 0; i < game.currentDifficulty.rows; i++)
			        {
                        for (int j = 0; j < game.currentDifficulty.columns; j++)
                        {
                            cell currentCell = game.gameBoard->cells[i][j];
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
                                game.gameBoard->isLost = 1;
                            }
                        }
                    }

                    DrawTextEx(font, TextFormat("%03d", (int)game.seconds), (Vector2) {10, 0}, 70, 0.0f, BLACK);


                    const char *minesRemainingText = TextFormat("Mines: %d", game.minesRemaining);
                    int textWidth = MeasureTextEx(font, minesRemainingText, 70, 0.0f).x;
                    DrawTextEx(font, minesRemainingText, (Vector2) {SCREENWIDTH / 2 - textWidth / 2, 0}, 70, 0.0f, BLACK);
                    
                    if (game.gameWon)
                    {
                        const char *endText = "YOU WON!";
                        float textWidth = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).x;
                        float textHeight = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).y;
                        float padding = 5.0f;

                        DrawRectangle(SCREENWIDTH / 2 - textWidth / 2 - padding, SCREENHEIGHT / 2 - textHeight / 2, textWidth + padding * 2, textHeight, WHITE);
                        DrawRectangleLines(SCREENWIDTH / 2 - textWidth / 2 - padding, SCREENHEIGHT / 2 - textHeight / 2, textWidth + padding * 2, textHeight, BLACK);
                        DrawTextEx(font, endText, (Vector2) {SCREENWIDTH / 2 - textWidth / 2, SCREENHEIGHT / 2 - textHeight / 2}, (float)font.baseSize, 0.0f, BLACK);

                        DrawDifficultyLevelButtons(easy, medium, hard, 4);
                    }

                    if (game.gameLost)
                    {
                        char *endText = "YOU LOST!";
                        float textWidth = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).x;
                        float textHeight = MeasureTextEx(font, endText, (float)font.baseSize, 0.0f).y;
                        float padding = 5.0f;

                        DrawRectangle(SCREENWIDTH / 2 - textWidth / 2 - padding, SCREENHEIGHT / 2 - textHeight / 2, textWidth + padding * 2, textHeight, WHITE);
                        DrawRectangleLines(SCREENWIDTH / 2 - textWidth / 2 - padding, SCREENHEIGHT / 2 - textHeight / 2, textWidth + padding * 2, textHeight, BLACK);
                        DrawTextEx(font, endText, (Vector2) {SCREENWIDTH / 2 - textWidth / 2, SCREENHEIGHT / 2 - textHeight / 2}, (float)font.baseSize, 0.0f, BLACK);

                        DrawDifficultyLevelButtons(easy, medium, hard, 4);
                    }
                } break;
            }
	
		EndDrawing();
	}

    UnloadFont(font);

    free(game.gameBoard);

	CloseWindow();

    return 0;   
}
