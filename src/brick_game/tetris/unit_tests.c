#include <check.h>

#include "tetris_backend.h"

START_TEST(createFieldTest) {
  int** field = createField();
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) ck_assert_int_eq(field[i][j], 0);
  clearField(field);
}
END_TEST

START_TEST(createFigureTest) {
  int** figure = createFigure();
  for (int i = 0; i < FIGURES_SIZE; i++)
    for (int j = 0; j < FIGURES_SIZE; j++) ck_assert_int_eq(figure[i][j], 0);
  clearFigure(figure);
}
END_TEST

START_TEST(newGameTest) {
  createTetris();
  newGame();
  GameInfo_t* tetris = getGameInfo();
  ck_assert_int_eq(tetris->score, 0);
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++)
      ck_assert_int_eq(tetris->field[i][j], 0);
  TetrisStates* state = getTetrisState();
  ck_assert_int_eq(*state, START);
  clearTetris();
}
END_TEST

START_TEST(createTetrisTest) {
  GameInfo_t* tetris = createTetris();
  ck_assert_ptr_eq(tetris, getGameInfo());
  ck_assert_ptr_nonnull(tetris->field);
  ck_assert_ptr_nonnull(tetris->next);

  Figure* figure = getFigure();
  ck_assert_ptr_nonnull(figure->blocks);

  clearTetris();
}
END_TEST

START_TEST(userInputTest1) {
  createTetris();
  Figure* fig = getFigure();
  TetrisStates* state = getTetrisState();
  *state = MOVE;
  fig->y = 10;
  fig->x = -5;
  int x = fig->x;
  userInput(Left, 0);
  ck_assert_int_eq(x, fig->x);

  fig->x = 5;
  x = fig->x;
  userInput(Left, 0);
  ck_assert_int_eq(x - 1, fig->x);

  clearTetris();
}
END_TEST

START_TEST(userInputTest2) {
  createTetris();
  Figure* fig = getFigure();
  TetrisStates* state = getTetrisState();
  *state = MOVE;

  fig->x = FIELD_WIDTH;
  int x = fig->x;
  userInput(Right, 0);
  ck_assert_int_eq(x, fig->x);

  fig->x = 3;
  x = fig->x;
  userInput(Right, 0);
  ck_assert_int_eq(x + 1, fig->x);
  clearTetris();
}
END_TEST

START_TEST(userInputTest3) {
  createTetris();
  Figure* fig = getFigure();
  TetrisStates* state = getTetrisState();
  *state = MOVE;

  fig->y = FIELD_HEIGHT;
  int y = fig->y;
  userInput(Down, 0);
  ck_assert_int_eq(y, fig->y);

  fig->y = 3;
  y = fig->y;
  userInput(Down, 0);
  ck_assert_int_eq(y + 1, fig->y);
  clearTetris();
}
END_TEST

START_TEST(userInputTest4) {
  createTetris();
  Figure* fig = getFigure();
  TetrisStates* state = getTetrisState();
  *state = MOVE;

  int a[] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0};
  int b[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0};
  for (int i = 0, k = 0; i < FIGURES_SIZE; i++)
    for (int j = 0; j < FIGURES_SIZE; j++) fig->blocks[i][j] = a[k++];

  fig->x = -3;
  userInput(Action, 0);
  for (int i = 0, k = 0; i < FIGURES_SIZE; i++)
    for (int j = 0; j < FIGURES_SIZE; j++)
      ck_assert_int_eq(fig->blocks[i][j], a[k++]);
  fig->x = 5;
  userInput(Action, 0);

  for (int i = 0, k = 0; i < FIGURES_SIZE; i++)
    for (int j = 0; j < FIGURES_SIZE; j++)
      ck_assert_int_eq(fig->blocks[i][j], b[k++]);

  clearTetris();
}
END_TEST

START_TEST(userInputTest5) {
  createTetris();
  TetrisStates* state = getTetrisState();

  userInput(Start, 0);
  ck_assert_int_eq(*state, MOVE);

  userInput(Terminate, 0);
  ck_assert_int_eq(*state, EXIT_STATE);

  clearTetris();
}
END_TEST

START_TEST(userInputTest6) {
  createTetris();
  TetrisStates* state = getTetrisState();
  *state = MOVE;
  userInput(Pause, 0);
  ck_assert_int_eq(*state, PAUSE);

  userInput(Pause, 0);
  ck_assert_int_eq(*state, MOVE);

  clearTetris();
}
END_TEST

START_TEST(userInputTest7) {
  createTetris();
  TetrisStates* state = getTetrisState();
  *state = GAME_OVER;
  userInput(Right, 0);
  ck_assert_int_eq(*state, START);

  clearTetris();
}
END_TEST

START_TEST(eraeLinesTest1) {
  GameInfo_t* tetris = createTetris();
  for (int i = 0; i < FIELD_WIDTH; i++) tetris->field[FIELD_HEIGHT - 1][i] = 1;
  int score = eraseLine(tetris);
  ck_assert_int_eq(score, 100);
  clearTetris();
}
END_TEST

START_TEST(eraeLinesTest2) {
  GameInfo_t* tetris = createTetris();
  for (int j = FIELD_HEIGHT - 1; j >= FIELD_HEIGHT - 2; j--)
    for (int i = 0; i < FIELD_WIDTH; i++) tetris->field[j][i] = 1;
  int score = eraseLine(tetris);
  ck_assert_int_eq(score, 300);
  clearTetris();
}
END_TEST

START_TEST(eraeLinesTest3) {
  GameInfo_t* tetris = createTetris();
  for (int j = FIELD_HEIGHT - 1; j >= FIELD_HEIGHT - 3; j--)
    for (int i = 0; i < FIELD_WIDTH; i++) tetris->field[j][i] = 1;
  int score = eraseLine(tetris);
  ck_assert_int_eq(score, 700);
  clearTetris();
}
END_TEST

START_TEST(eraeLinesTest4) {
  GameInfo_t* tetris = createTetris();
  for (int j = FIELD_HEIGHT - 1; j >= FIELD_HEIGHT - 4; j--)
    for (int i = 0; i < FIELD_WIDTH; i++) tetris->field[j][i] = 1;
  int score = eraseLine(tetris);
  ck_assert_int_eq(score, 1500);
  clearTetris();
}
END_TEST

START_TEST(eraeLinesTest5) {
  GameInfo_t* tetris = createTetris();
  int score = eraseLine(tetris);
  ck_assert_int_eq(score, 0);
  clearTetris();
}
END_TEST

START_TEST(updateCurrentStateTest1) {
  GameInfo_t* tetris = createTetris();
  tetris->score = 800;
  updateCurrentState();
  ck_assert_int_eq(tetris->level, 2);

  tetris->score = 10000;
  for (int i = 0; i < 10; i++) updateCurrentState();
  ck_assert_int_eq(tetris->level, 10);
  clearTetris();
}
END_TEST

START_TEST(updateCurrentStateTest2) {
  GameInfo_t* tetris = createTetris();
  TetrisStates* state = getTetrisState();
  *state = MOVE;
  tetris->score = 800;
  for (int i = 0; i < 1800; i++) updateCurrentState();
  ck_assert_int_eq(*state, GAME_OVER);
  clearTetris();
}
END_TEST

Suite* tests(void) {
  Suite* s = suite_create("\033[45m-=s21_Tetris=-\033[0m");
  TCase* tc11 = tcase_create("log_tc");
  suite_add_tcase(s, tc11);
  tcase_add_test(tc11, createFieldTest);
  tcase_add_test(tc11, createFigureTest);
  tcase_add_test(tc11, newGameTest);
  tcase_add_test(tc11, createTetrisTest);
  tcase_add_test(tc11, userInputTest1);
  tcase_add_test(tc11, userInputTest2);
  tcase_add_test(tc11, userInputTest3);
  tcase_add_test(tc11, userInputTest4);
  tcase_add_test(tc11, userInputTest5);
  tcase_add_test(tc11, userInputTest6);
  tcase_add_test(tc11, userInputTest7);
  tcase_add_test(tc11, eraeLinesTest1);
  tcase_add_test(tc11, eraeLinesTest2);
  tcase_add_test(tc11, eraeLinesTest3);
  tcase_add_test(tc11, eraeLinesTest4);
  tcase_add_test(tc11, eraeLinesTest5);
  tcase_add_test(tc11, updateCurrentStateTest1);
  tcase_add_test(tc11, updateCurrentStateTest2);
  suite_add_tcase(s, tc11);
  return s;
}

int main(void) {
  int failed = 0;
  Suite* s21_math_test[] = {tests(), NULL};
  for (int i = 0; s21_math_test[i] != NULL; i++) {  // (&& failed == 0)
    SRunner* sr = srunner_create(s21_math_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}
