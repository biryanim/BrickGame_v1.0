#ifndef FRONT_TETRIS_H
#define FRONT_TETRIS_H

/************************************************************
 * @file tetris_front.h
 * @brief CLI GUI header
 ************************************************************/

#include <locale.h>
#include <ncurses.h>
#include <time.h>

#include "../../brick_game/tetris/tetris_backend.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

/************************************************************
 * @brief GUI initialization
 *
 * Initialize ncurses cli window, ncurses settings
 ************************************************************/

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

/************************************************************
 * @brief Draw game board
 ************************************************************/

void print_overlay();

/************************************************************
 * @brief Draw ractangles
 ************************************************************/

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/************************************************************
 * @brief Draw game stats
 ************************************************************/

void printStats(GameInfo_t *tetris);

/************************************************************
 * @brief Draw game field
 ************************************************************/

void print(GameInfo_t *tetris, Figure *fig);

/************************************************************
 * @brief Game maine loop
 ************************************************************/

void game_loop();

/************************************************************
 * @brief Clear game field
 ************************************************************/

void clearBoard();

#endif