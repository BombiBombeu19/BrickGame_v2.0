CC := g++
CFLAGS := -I includes -std=c++17 -Wall -Werror -Wextra
TETRIS_SRC_DIR := brick_game/tetris
SNAKE_SRC_DIR_MODEL := brick_game/snake/model
SNAKE_SRC_DIR_CONTROLLER := brick_game/snake/controller
CLI_SRC_DIR := gui/cli
OUT_DIR := src/out
CHECK_LIBS = -L/opt/homebrew/lib -lgtest -lgtest_main -pthread
CFLAGS += -I/opt/homebrew/Cellar/googletest/1.15.2/include
LFLAGS_COVERAGE= --coverage

SRC_TEST_TETRIS = gui/cli/cli_tetris.cc brick_game/tetris/*.cc
SRC_TEST_SNAKE = gui/cli/snake_view.cc brick_game/snake/controller/snake_controller.cc brick_game/snake/model/snake_model.cc

OBJS := $(patsubst %.cc, %.o, $(SRCS))

OS := $(shell uname -s)

ifeq ($(OS), Linux)
    CHECK_LIBS += -lm -lrt -pthread -lsubunit
endif

ifeq ($(OS), Darwin)
    CHECK_LIBS += -lncurses
endif

all: install

test: $(SRC_TEST)
	@$(CC) $(CFLAGS) test.cc $(SRC_TEST_TETRIS) $(SRC_TEST_SNAKE) -o test $(CHECK_LIBS)
	@./test

leaks:
	leaks -atExit -- ./test

install: clean cli desktop

cli: clean
	@touch high_score.txt
	@touch high_score_snake.txt
	@mkdir ../build
	@cp -rf images ../build
	@cd ../build && cmake ../ && make consoleBrickGame
	
run_cli:
	@../build/consoleBrickGame

desktop: clean
	@touch high_score.txt
	@touch high_score_snake.txt
	@mkdir gui/desktop/snake/build
	@cp -rf images gui/desktop/snake/build
	@cd gui/desktop/snake/build && cmake ../ && make snake

run_desktop:
	@gui/desktop/snake/build/snake

uninstall: clean

dvi:
	open readme.md
	
gcov_report:
	@$(CC) $(CFLAGS) $(SRC_TEST_TETRIS) $(SRC_TEST_SNAKE) -o test $(CHECK_LIBS) $(LFLAGS_COVERAGE)
	@./test
	@lcov --capture --directory . --output-file gcov.info
	@genhtml gcov.info --output-directory report
	@open report/index.html

dist: clean
	@mkdir -p ./bin
	@tar -cvzf ./brick_game_v2.tgz makefile ./brick_game/snake/*  ./brick_game/tetris/* ./gui/cli/* ./gui/desktop/* test.cc ../CMakeLists.txt
	@mv brick_game_v2.tgz ./bin/

clang:
	@clang-format -i -style=Google $(TETRIS_SRC_DIR)/*.cc brick_game/snake/controller/*.cc brick_game/snake/model/*.cc gui/desktop/snake/*.cc gui/desktop/snake/*.h

clang-check:
	@clang-format -n -style=Google $(TETRIS_SRC_DIR)/*.cc brick_game/snake/controller/*.cc brick_game/snake/model/*.cc gui/desktop/snake/*.cc gui/desktop/snake/*.h

clean:
	@rm -rf $(SNAKE_SRC_DIR)/*.o
	@rm -rf $(SNAKE_SRC_DIR_MODEL)/*.o
	@rm -rf $(SNAKE_SRC_DIR_CONTROLLER)/*.o
	@rm -rf $(TETRIS_SRC_DIR)/*.o
	@rm -rf gui/cli/*.o
	@rm -rf gcov.info
	@rm -rf report
	@rm -rf test
	@rm -rf ./bin
	@rm -rf ./dvi
	@rm -rf cli
	@rm -rf brick_game/*.o
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf output.aux
	@rm -rf output.log
	@rm -rf ../build
	@rm -rf gui/desktop/snake/build
	@rm -rf high_score.txt
	@rm -rf high_score_snake.txt
	@rm -rf tetris_high_score.txt


.PHONY: all dvi gcov_report test clean install uninstall dist run_desktop run_cli