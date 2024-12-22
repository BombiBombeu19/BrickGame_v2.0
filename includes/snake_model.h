#pragma once

#include <ncurses.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <ctime>
#include <iostream>
#include <random>

#include "../includes/define_snake.h"
#include "../includes/objects_snake.h"
#include "../includes/snake_controller.h"
#include "../includes/snake_view.h"

namespace s21 {

class Snake {
 public:
  typedef struct {
    int x;
    int y;
  } Point;

  std::array<Point, 200> snake_body;
  int len;
};

class SnakeGame {
 public:
  enum class Direction {
    DirectionLeft,
    DirectionRight,
    DirectionUp,
    DirectionDown
  };

  SnakeGame();
  ~SnakeGame();

  void snake_game_start();
  void game_init();

  void sort(int scores[100]);
  void read_file(int *high_score);
  void record_file(int high_score);
  void StartGame(int input);

  int check_game_over(int matrix[][20]);
  void game_controller(int input);
  void updateCurrentState(int input);
  void snake_clear_game_field(int field[10][20]);
  void snake_spawn();
  void snake_init();
  void snake_moving();
  void generate_random_apple();
  bool is_correct_coordinate(int x, int y);
  void add_points();

  Snake snake;
  SnakeGameInfo_t game_info;
  Snake::Point apple;
  SnakeGameState_t game_state;
  SnakeUserAction_t user_action;
  Direction direction_state;
};

}  // namespace s21
