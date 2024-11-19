#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

/************************************************************
 * @file tetris_backend.h
 * @brief Game logic library header
 ************************************************************/

#include <ncurses.h>
#include <stdlib.h>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIGURES_COUNT 7
#define FIGURES_SIZE 4

/************************************************************
 * @brief Signals for fsm
 *
 * User input, used as signals for finite state machine table
 ************************************************************/

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/************************************************************
 * @brief Game data struct
 *
 * @param field Playing Field
 * @param next Next figure
 * @param score Game score
 * @param high_score Game highest score
 * @param level Game level
 * @param speed Speed of movement of the figures
 * @param pause Pause flag
 ************************************************************/

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/************************************************************
 * @brief Figure datat struct
 *
 * Figures parameters
 *
 * @param blocks Figures blocks
 * @param x Coordinate x
 * @param y Coordinate y
 * ************************************************************/

typedef struct {
  int **blocks;
  int x;
  int y;
} Figure;

/************************************************************
 * @brief States for fsm
 *
 * Game states, used as signals for finite state machine table
 ************************************************************/

typedef enum { START, MOVE, PAUSE, GAME, GAME_OVER, EXIT_STATE } TetrisStates;

/************************************************************
 * @brief User's input processing
 *
 * Function that processes user signals
 * @param action User's action
 * @param hold Parameter whether pressed key hol or not
 ************************************************************/

void userInput(UserAction_t action, bool hold);

/************************************************************
 * @brief Update current game state
 *
 * Update current game state by shifting figure one pixel down
 ************************************************************/

GameInfo_t updateCurrentState();

/************************************************************
 * @brief Gets the address of the static figure
 ************************************************************/

Figure *getFigure();

/************************************************************
 * @brief Gets the address of the static variable GameInfo_t
 ************************************************************/

GameInfo_t *getGameInfo();

/************************************************************
 * @brief Gets the addres of the static variable state
 ************************************************************/

TetrisStates *getTetrisState();

/************************************************************
 * @brief Allocates memory for the field
 ************************************************************/

int **createField();

/************************************************************
 * @brief Clears the allocated memory of the field
 *
 * @param field Pointer to game field
 ************************************************************/

void clearField(int **field);

/************************************************************
 * @brief Allocates memory for the figure
 ************************************************************/

int **createFigure();

/************************************************************
 * @brief Clears the allocated memory of the figure
 *
 * @param figures Pointer to figure
 ************************************************************/

void clearFigure(int **figures);

/************************************************************
 * @brief Set random figure
 *
 * @param figure Pointer to figure
 ************************************************************/

void setFigure(int **figure);

/************************************************************
 * @brief Restart the game
 *
 * Score is assigned zero,  clear game field
 ************************************************************/

void newGame();

/************************************************************
 * @brief Create tetris
 *
 * Memory is allocated for the field, figures,
 * a random figure is set for the current figure and the next,
 * high score is read from the file
 ************************************************************/

GameInfo_t *createTetris();

/************************************************************
 * @brief Clear tetris
 *
 * Clear allocated memory
 ************************************************************/

void clearTetris();

/************************************************************
 * @brief Copy a figure from b to a
 ************************************************************/

void copyFigure(int **a, int **b);

/************************************************************
 * @brief Moves figure down one pixel
 *
 * Increments the y coordinate by 1
 ************************************************************/

void moveDown(Figure *fig);

/************************************************************
 * @brief Moves figure up one pixel
 *
 * Decreases the y coordinate by 1
 ************************************************************/

void moveUp(Figure *fig);

/************************************************************
 * @brief Moves figure right one pixel
 *
 * Increments the x coordinate by 1
 ************************************************************/

void moveRight(Figure *fig);

/************************************************************
 * @brief Moves figure left one pixel
 *
 * Decreases the x coordinate by 1
 ************************************************************/

void moveLeft(Figure *fig);

/************************************************************
 * @brief Rotates the figure
 *
 * Rotates the figure clockwise
 ************************************************************/

void rotateFig(int **a, int **b);

/************************************************************
 * @brief Check if figure collides
 *
 * Check if figure collides during moving, shiftig, rotating.
 ************************************************************/
int collision(GameInfo_t *tetris, Figure *figure);

/************************************************************
 * @brief Puts a figure on the field
 *
 * Puts a figure on the field when figure collides
 ************************************************************/

void plantFigure(GameInfo_t *tetris, Figure *figure);

/************************************************************
 * @brief  Get score
 *
 * @param lines Count of erased line
 ************************************************************/

int getScore(int lines);

/************************************************************
 * @brief  Drop line
 *
 * Deletes the specified line, assigning the line above it in its place
 *
 * @param line a line that needs to be removed
 * @param tetris game struct
 ************************************************************/

void dropLine(int line, GameInfo_t *tetris);

/************************************************************
 * @brief  Erase line when its full
 ************************************************************/

int eraseLine(GameInfo_t *tetris);

/************************************************************
 * @brief  Drop new figure
 ************************************************************/

void dropNewFigure(GameInfo_t *tetris, Figure *fig);

/************************************************************
 * @brief  Calculate score, erase lines and drop new figure
 ************************************************************/

void calculateTetris(GameInfo_t *tetris, Figure *figure, TetrisStates *state);

#endif