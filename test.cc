#include <gtest/gtest.h>
#include <time.h>
#include "./includes/tetris.h"
#include "./includes/backend_tetris.h"
#include "./includes/cli_tetris.h"
#include "./includes/objects_tetris.h"
#include "./includes/define_tetris.h"

#include "./includes/snake_controller.h"
#include "./includes/snake_model.h"
#include "./includes/snake_view.h"
#include "./includes/objects_snake.h"
#include "./includes/define_snake.h"
#include <ncurses.h>

TEST(TetrisTest, MatrCopy) {
  int matr_1[10][20];
  int matr_2[10][20];
  s21::clear_game_field(matr_1);
  s21::clear_game_field(matr_2);
  matr_1[3][10] = 1;
  s21::copy_matrix(matr_2, matr_1);

  EXPECT_EQ(matr_1[3][10], matr_2[3][10]);
}

TEST(TetrisTest, GameOver) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 0;

  while (state != s21::GameState_t::GAMEOVER_STATE) {
    s21::updateCurrentState(&data, &state, 0);
  }

  EXPECT_EQ(state, s21::GameState_t::GAMEOVER_STATE);
}

TEST(TetrisTest, Terminate) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 0;
  int input = 0550;
  s21::updateCurrentState(&data, &state, input);

  EXPECT_NE(state, s21::GameState_t::GAMEOVER_STATE);
}

TEST(TetrisTest, MoveDown) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 0;
  int input = KEY_DOWN;
  s21::updateCurrentState(&data, &state, input);
  input = KEY_DOWN;
  s21::updateCurrentState(&data, &state, input);

  EXPECT_EQ(state, s21::GameState_t::MOVING_STATE);
}

TEST(TetrisTest, MoveLeft) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 0;
  int input = KEY_LEFT;
  s21::updateCurrentState(&data, &state, input);
  s21::updateCurrentState(&data, &state, input);

  EXPECT_EQ(state, s21::GameState_t::MOVING_STATE);
}

TEST(TetrisTest, MoveRight) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 1;
  int input = KEY_RIGHT;
  s21::updateCurrentState(&data, &state, input);
  s21::updateCurrentState(&data, &state, input);

  EXPECT_EQ(state, s21::GameState_t::MOVING_STATE);
}

TEST(TetrisTest, Pause) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 1;
  int input = 'p';
  s21::updateCurrentState(&data, &state, input);

  EXPECT_NE(state, s21::GameState_t::PAUSE_STATE);
}

TEST(TetrisTest, MoveRotate) {
  srand(time(NULL));
  s21::GameInfo_t data;
  s21::GameState_t state = s21::GameState_t::START_STATE;
  data.speed = 1;
  int input = ' ';
  s21::updateCurrentState(&data, &state, input);

  EXPECT_EQ(state, s21::GameState_t::MOVING_STATE);
}

#include "./includes/snake_model.h"

TEST(SnakeGameTest, SetGameDataDefault) {
  s21::SnakeGame snake_game;
  EXPECT_EQ(snake_game.game_info.score, 0);
  EXPECT_EQ(snake_game.game_info.high_score, 0);
  EXPECT_EQ(snake_game.game_info.level, 1);
  EXPECT_EQ(snake_game.game_info.speed, 350000);
}

TEST(SnakeGameTest, ChangeDirectionRight) {
  s21::SnakeGame snake_game;
  snake_game.direction_state = s21::SnakeGame::Direction::DirectionLeft;
  int input = KEY_RIGHT;
  snake_game.game_controller(input);
  EXPECT_EQ(snake_game.direction_state, s21::SnakeGame::Direction::DirectionUp);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::MovingState);
}

TEST(SnakeGameTest, ChangeDirectionLeft) {
  s21::SnakeGame snake_game;
  snake_game.direction_state = s21::SnakeGame::Direction::DirectionRight;
  int input = KEY_LEFT;
  snake_game.game_controller(input);
  EXPECT_EQ(snake_game.direction_state, s21::SnakeGame::Direction::DirectionUp);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::MovingState);
}

TEST(SnakeGameTest, ChangeDirectionRight_2) {
  s21::SnakeGame snake_game;
  snake_game.direction_state = s21::SnakeGame::Direction::DirectionDown;
  int input = KEY_RIGHT;
  snake_game.game_controller(input);
  EXPECT_EQ(snake_game.direction_state, s21::SnakeGame::Direction::DirectionLeft);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::MovingState);
}

TEST(SnakeGameTest, ChangeDirectionLeft_2) {
  s21::SnakeGame snake_game;
  snake_game.direction_state = s21::SnakeGame::Direction::DirectionUp;
  int input = KEY_LEFT;
  snake_game.game_controller(input);
  EXPECT_EQ(snake_game.direction_state, s21::SnakeGame::Direction::DirectionLeft);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::MovingState);
}

TEST(SnakeGameTest, PauseGame) {
  s21::SnakeGame snake_game;
  int input = 'p';
  snake_game.game_controller(input);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::PauseState);
}

TEST(SnakeGameTest, ResumeGame) {
  s21::SnakeGame snake_game;
  int input = 'p';
  snake_game.game_controller(input);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::PauseState);
  snake_game.updateCurrentState(input);
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::MovingState);
}

TEST(SnakeGameTest, GameOver) {
  s21::SnakeGame snake_game;
  snake_game.snake.len = 1;
  snake_game.snake.snake_body[0].x = 0;
  snake_game.snake.snake_body[0].y = 0;
  snake_game.snake_moving();
  EXPECT_EQ(snake_game.game_state, s21::SnakeGameState_t::GameOverState);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
