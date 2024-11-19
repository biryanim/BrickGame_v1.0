/************************************************************
 * @file tetris_front.c
 * @brief CLI GUI source
 ************************************************************/

#include "tetris_front.h"

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);
  int i = left_x + 1;
  for (; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}

void printStats(GameInfo_t *tetris) {
  mvprintw(3, FIELD_WIDTH + 4, "%.6d", tetris->score);
  mvprintw(7, FIELD_WIDTH + 4, "%.6d", tetris->high_score);
  mvprintw(11, FIELD_WIDTH + 4, "%d", tetris->level);
  for (int i = 0; i < FIGURES_SIZE; i++) {
    for (int j = 0; j < FIGURES_SIZE; j++) {
      if (tetris->next[i][j])
        mvaddch(15 + i, FIELD_WIDTH + 4 + j, ACS_BLOCK);
      else
        mvaddch(15 + i, FIELD_WIDTH + 4 + j, ' ');
    }
  }
}

void print_overlay() {
  print_rectangle(0, FIELD_HEIGHT + 1, 0, FIELD_WIDTH + 1);
  print_rectangle(0, FIELD_HEIGHT + 1, FIELD_WIDTH + 2, 2 * FIELD_WIDTH + 5);
  print_rectangle(2, 4, FIELD_WIDTH + 3, 2 * FIELD_WIDTH + 4);
  print_rectangle(6, 8, FIELD_WIDTH + 3, 2 * FIELD_WIDTH + 4);
  print_rectangle(10, 12, FIELD_WIDTH + 3, 2 * FIELD_WIDTH + 4);
  print_rectangle(14, 20, FIELD_WIDTH + 3, 2 * FIELD_WIDTH + 4);
  mvprintw(1, FIELD_WIDTH + 4, "Score");
  mvprintw(5, FIELD_WIDTH + 4, "Best Score");
  mvprintw(9, FIELD_WIDTH + 4, "Level");
  mvprintw(13, FIELD_WIDTH + 4, "Next Figure");
}

void print(GameInfo_t *tetris, Figure *fig) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      int symbol = 0;
      if (tetris->field[i][j] != 0)
        symbol = 1;
      else {
        int x = j - fig->x;
        int y = i - fig->y;
        if (x >= 0 && x < FIGURES_SIZE && y >= 0 && y < FIGURES_SIZE &&
            fig->blocks[y][x])
          symbol = 1;
      }
      mvaddch(i + 1, j + 1, (symbol == 1 ? ACS_BLOCK : ' '));
    }
  }
}

void clearBoard() {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) mvaddch(i + 1, j + 1, ' ');
  mvprintw(3, FIELD_WIDTH + 4, "%.6d", 0);
  // mvprintw(3, FIELD_WIDTH + 4, " ");
  // mvprintw(3, FIELD_WIDTH + 5, " ");
  // mvprintw(3, FIELD_WIDTH + 6, " ");
  // mvprintw(3, FIELD_WIDTH + 7, " ");
  // mvprintw(3, FIELD_WIDTH + 8, " ");
  // mvprintw(3, FIELD_WIDTH + 9, " ");
}