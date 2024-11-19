/************************************************************
 * @mainpage
 * @author biryanim
 * @version 1.0
 * @file tetris.c
 * @brief Entry point
 ************************************************************/

#include "tetris_front.h"

int main() {
  srand(time(NULL));
  setlocale(LC_ALL, "");
  WIN_INIT(50);
  print_overlay();
  // cbreak();
  // scrollok(stdscr, TRUE);
  // nodelay(stdscr, TRUE);
  game_loop();
  endwin();
  return 0;
}

void game_loop() {
  GameInfo_t* tetris = createTetris();
  TetrisStates* state = getTetrisState();
  Figure* curfig = getFigure();
  while (*state != EXIT_STATE) {
    int act = getch();
    timeout(tetris->speed);
    switch (act) {
      case KEY_DOWN:
        userInput(Down, 0);
        break;
      case KEY_RIGHT:
        userInput(Right, 0);
        break;
      case KEY_LEFT:
        userInput(Left, 0);
        break;
      case KEY_UP:
        userInput(Action, 0);
        break;
      case 'q':
        userInput(Terminate, 0);
        break;
      case '\n':
        userInput(Start, 0);
        break;
      case 'p':
      case 'P':
        userInput(Pause, 0);
        break;
      default:
        break;
    }
    *tetris = updateCurrentState();
    if (*state == START || *state == GAME_OVER) {
      clearBoard();
      newGame();
      mvprintw(10, 1, "Press");
      mvprintw(11, 5, "ENTER");
    } else if (*state == PAUSE) {
      mvprintw(10, 3, "Paused");
    } else {
      print(tetris, curfig);
      printStats(tetris);
    }
    refresh();
  }
  clearTetris();
}