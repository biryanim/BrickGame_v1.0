/************************************************************
 * @file tetris_backend.c
 * @brief Game logic library source
 ************************************************************/

#include "tetris_backend.h"

Figure *getFigure() {
  static Figure fig;
  return &fig;
}

GameInfo_t *getGameInfo() {
  static GameInfo_t tetris;
  return &tetris;
}

TetrisStates *getTetrisState() {
  static TetrisStates state;
  return &state;
}

int **createField() {
  int **field = (int **)calloc(FIELD_HEIGHT, sizeof(int *));
  for (int i = 0; i < FIELD_HEIGHT; i++)
    field[i] = (int *)calloc(FIELD_WIDTH, sizeof(int));
  return field;
}

void clearField(int **field) {
  if (field) {
    for (int i = 0; i < FIELD_HEIGHT; i++) free(field[i]);
    free(field);
  }
}

int **createFigure() {
  int **figure = (int **)calloc(FIGURES_SIZE, sizeof(int *));
  for (int i = 0; i < FIGURES_SIZE; i++)
    figure[i] = (int *)calloc(FIGURES_SIZE, sizeof(int));
  return figure;
}

void clearFigure(int **figures) {
  if (figures) {
    for (int i = 0; i < FIGURES_SIZE; i++) free(figures[i]);
    free(figures);
  }
}

void setFigure(int **figure) {
  int a[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,

             0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,

             0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0,

             0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,

             0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0,

             0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0,

             0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0};
  int num_of_fig = rand() % FIGURES_COUNT;
  for (int i = 0, k = 0; i < FIGURES_SIZE; i++)
    for (int j = 0; j < FIGURES_SIZE; j++)
      figure[i][j] = a[FIGURES_SIZE * FIGURES_SIZE * num_of_fig + (k++)];
}

void setStartField(int **field) {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) field[i][j] = 0;
}

void newGame() {
  GameInfo_t *tetris = getGameInfo();
  Figure *fig = getFigure();
  TetrisStates *state = getTetrisState();
  tetris->score = 0;
  setStartField(tetris->field);
  setFigure(tetris->next);
  setFigure(fig->blocks);
  *state = START;
}

GameInfo_t *createTetris() {
  GameInfo_t *tetris = getGameInfo();
  tetris->field = createField();
  tetris->next = createFigure();
  setFigure(tetris->next);
  tetris->score = 0;
  tetris->level = 1;
  tetris->speed = 70;
  tetris->pause = 0;
  tetris->high_score = 0;
  FILE *file = fopen("../gui/cli/record.txt", "r");
  if (file) {
    fscanf(file, "%d", &(tetris->high_score));
    fclose(file);
  }

  Figure *curFig = getFigure();
  curFig->blocks = createFigure();
  setFigure(curFig->blocks);
  curFig->x = 3;
  curFig->y = -2;

  TetrisStates *state = getTetrisState();
  *state = START;
  return tetris;
}

void clearTetris() {
  GameInfo_t *tetris = getGameInfo();
  clearField(tetris->field);
  clearFigure(tetris->next);
  Figure *fig = getFigure();
  clearFigure(fig->blocks);
}

void copyFigure(int **a, int **b) {
  for (int i = 0; i < FIGURES_SIZE; i++)
    for (int j = 0; j < FIGURES_SIZE; j++) a[i][j] = b[i][j];
}

void moveDown(Figure *fig) { fig->y++; }

void moveUp(Figure *fig) { fig->y--; }
void moveRight(Figure *fig) { fig->x++; }
void moveLeft(Figure *fig) { fig->x--; }

void rotateFig(int **a, int **b) {
  for (int i = 0, k = FIGURES_SIZE - 1; i < FIGURES_SIZE && k >= 0; i++, k--) {
    for (int j = 0; j < FIGURES_SIZE; j++) {
      a[j][k] = b[i][j];
    }
  }
}

int collision(GameInfo_t *tetris, Figure *figure) {
  int **field = tetris->field;
  Figure *fig = figure;
  // int i = fig->y < 0 ? -(fig->y) : 0;
  // int j = fig->x < 0
  int flag = 0;
  for (int i = 0; i < FIGURES_SIZE && !flag; i++) {
    for (int j = 0; j < FIGURES_SIZE && !flag; j++) {
      if (fig->blocks[i][j] != 0) {
        int fx = fig->x + j;
        int fy = fig->y + i;

        if (fx < 0 || fx >= FIELD_WIDTH || fy >= FIELD_HEIGHT) {
          flag = 1;
        } else if (fy < 0)
          continue;
        else if (field[fy][fx] != 0) {
          flag = 1;
        }
      }
    }
  }
  return flag;
}

void userInput(UserAction_t action, bool hold) {
  GameInfo_t *tetris = getGameInfo();
  Figure *curfig = getFigure();
  TetrisStates *state = getTetrisState();
  (void)hold;  // fix
  if (action == Terminate)
    *state = EXIT_STATE;
  else if (*state == START) {
    if (action == Start) *state = MOVE;
    // if(action == Terminate)
    //     *state = EXIT_STATE;
  } else if (*state == PAUSE) {
    if (action == Pause) *state = MOVE;
  } else if (action == Pause)
    *state = PAUSE;
  else if (*state == GAME_OVER)
    *state = START;
  else if (action == Left) {
    moveLeft(curfig);
    if (collision(tetris, curfig)) moveRight(curfig);
    *state = MOVE;
  } else if (action == Right) {
    moveRight(curfig);
    if (collision(tetris, curfig)) moveLeft(curfig);
    *state = MOVE;
  } else if (action == Down) {
    moveDown(curfig);
    if (collision(tetris, curfig)) moveUp(curfig);
    *state = MOVE;
  } else if (action == Action) {
    Figure old;
    old.blocks = createFigure();
    copyFigure(old.blocks, curfig->blocks);
    rotateFig(curfig->blocks, old.blocks);
    if (collision(tetris, curfig)) copyFigure(curfig->blocks, old.blocks);
    clearFigure(old.blocks);
    *state = MOVE;
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t *tetris = getGameInfo();
  Figure *curfig = getFigure();
  TetrisStates *state = getTetrisState();
  if (*state == EXIT_STATE) {
    FILE *file = fopen("../gui/cli/record.txt", "w");
    if (file) {
      fprintf(file, "%d", tetris->high_score);
    }
    fclose(file);
  }
  if (*state != PAUSE && *state != START)
    calculateTetris(tetris, curfig, state);

  if (tetris->level < 10 && tetris->score >= tetris->level * 600) {
    tetris->level++;
    tetris->speed -= 5;
  }
  if (tetris->high_score < tetris->score) tetris->high_score = tetris->score;
  return *tetris;
}

void plantFigure(GameInfo_t *tetris, Figure *figure) {
  Figure *fig = figure;
  int i = fig->y < 0 ? -(fig->y) : 0;
  for (; i < FIGURES_SIZE; i++) {
    for (int j = 0; j < FIGURES_SIZE; j++) {
      if (fig->blocks[i][j] != 0) {
        int fx = fig->x + j;
        int fy = fig->y + i;
        tetris->field[fy][fx] = fig->blocks[i][j];
      }
    }
  }
}

int getScore(int lines) {
  int res = 0;
  switch (lines) {
    case 1:
      res = 100;
      break;
    case 2:
      res = 300;
      break;
    case 3:
      res = 700;
      break;
    case 4:
      res = 1500;
      break;
    default:
      break;
  }
  return res;
}

void dropLine(int line, GameInfo_t *tetris) {
  for (int i = line; i > 0; i--)
    for (int j = 0; j < FIELD_WIDTH; j++)
      tetris->field[i][j] = tetris->field[i - 1][j];
}

int eraseLine(GameInfo_t *tetris) {
  int full_lines = 0;
  int isFull;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    isFull = 1;
    for (int j = 0; j < FIELD_WIDTH && isFull; j++)
      if (tetris->field[i][j] == 0) isFull = 0;
    if (isFull) {
      dropLine(i, tetris);
      full_lines++;
    }
  }
  return getScore(full_lines);
}

void dropNewFigure(GameInfo_t *tetris, Figure *fig) {
  copyFigure(fig->blocks, tetris->next);
  fig->x = 3;
  fig->y = -2;
  setFigure(tetris->next);
}

void calculateTetris(GameInfo_t *tetris, Figure *figure, TetrisStates *state) {
  static int ticks = 15;
  if (ticks <= 0) {
    ticks = 15;
    moveDown(figure);
    if (collision(tetris, figure)) {
      moveUp(figure);
      plantFigure(tetris, figure);
      tetris->score += eraseLine(tetris);
      dropNewFigure(tetris, figure);
      if (collision(tetris, figure)) *state = GAME_OVER;
    }
  }
  ticks -= 1;
}