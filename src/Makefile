CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
LIBS =  -lcheck -lncurses
LINUX = -lrt -lpthread -lm -lsubunit
LFLAGS = -fprofile-arcs -ftest-coverage
OS = $(shell uname -s)

GUI_DIR = gui/cli
BUILD_DIR = build
DOCS_DIR = doxygen
DIST_DIR = dist
REPORT_DIR = report

all: install

install: tetris.a | build_dir
	doxygen
	$(CC) $(CFLAGS) $(GUI_DIR)/tetris.c $(GUI_DIR)/tetris_front.c $(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/tetris $(LIBS)

uninstall:
	rm -rf $(DOCS_DIR) $(BUILD_DIR)

dvi:
	cd doxygen/latex && make all

dist: dist_dir
	tar cf $(DIST_DIR)/tetris-1.0.tar $(BUILD_DIR) $(DOCS_DIR)

tetris.a: build_dir
	$(CC) $(CFLAGS) -c brick_game/tetris/tetris_backend.c -o $(BUILD_DIR)/tetris.o
	ar rc $(BUILD_DIR)/tetris.a $(BUILD_DIR)/tetris.o
	ranlib $(BUILD_DIR)/tetris.a
	rm -rf $(BUILD_DIR)/tetris.o

test: 
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) brick_game/tetris/unit_tests.c brick_game/tetris/tetris_backend.c -o unit_tests $(LIBS)
else
	$(CC) brick_game/tetris/unit_tests.c brick_game/tetris/tetris_backend.c -o unit_tests $(LIBS) $(LINUX)
endif
	./unit_tests

gcov_report:
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(LFLAGS) brick_game/tetris/unit_tests.c brick_game/tetris/tetris_backend.c -o test.out $(LIBS)
else
	$(CC) $(CFLAGS) $(LFLAGS) brick_game/tetris/unit_tests.c brick_game/tetris/tetris_backend.c -o test.out $(LIBS) $(LINUX)
endif
	./test.out
	lcov -t "test" -o test.info --no-external -c -d .
	genhtml -o ./report test.info
	open ./report/index.html

build_dir:
	mkdir -p $(BUILD_DIR)

dist_dir:
	mkdir -p $(DIST_DIR)

clean:
	rm -rf $(DOCS_DIR) $(BUILD_DIR) $(DIST_DIR) $(REPORT_DIR)
	rm -rf unit_tests *.o *.a
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf *.gcov
	rm -rf report
	rm -rf gcov_test
	rm -rf test.out

style:
	cp ../../../materials/linters/.clang-format ./.clang-format
	clang-format -i ./*.c ./*.h
	clang-format -n ./*.c ./*.h