#pragma once

#include <iostream>

#include "objects_tetris.h"

namespace s21 {
void print_overlay(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(GameInfo_t *stats);
void draw_field(GameInfo_t *info);
void draw_next_info(GameInfo_t *game_info);
void print_intro_message();
void print_pause_message();
void print_game_over();
}  // namespace s21