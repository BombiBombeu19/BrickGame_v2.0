#pragma once

#include <ncurses.h>

#include <iostream>

#include "backend_tetris.h"
#include "cli_tetris.h"
#include "define_tetris.h"
#include "helper_tetris.h"
#include "objects_tetris.h"

using namespace std;

namespace s21 {
void tetris_game_start();
void game_init();
void init_stats(GameInfo_t *stats);
void read_file(int *high_score);
void sort(int scores[100]);
void record_file(int high_score);
}  // namespace s21