#pragma once

#include <iostream>

namespace s21 {
struct GameInfo_t {
  int field[10][20];
  int next[10][20];
  int figure_m[10][20];
  bool new_figure_spawned;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
};

enum class UserAction_t {
  Start,
  Pause,
  Terminate,
  Left,
  Up,
  Down,
  Right,
  Rotate,
  Action
};

enum class GameState_t {
  START_STATE,
  SPAWN_STATE,
  MOVING_STATE,     // нажатие кнопок
  SHIFTING_STATE,   // перемещение фигуры
  ATTACHING_STATE,  // присоеднение фигуры
  PAUSE_STATE,
  GAMEOVER_STATE
};
} // namespace s21
