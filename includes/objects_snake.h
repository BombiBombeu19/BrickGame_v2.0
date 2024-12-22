#pragma once

#include <iostream>

namespace s21 {

struct SnakeGameInfo_t {
  int field[10][20];
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
};

enum class SnakeUserAction_t { Start, Pause, Terminate, Left, Right, Action };

enum class SnakeGameState_t {
  StartState,
  MovingState,
  ShiftingState,
  PauseState,
  GameOverState
};
}  // namespace s21
