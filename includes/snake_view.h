#pragma once

#include <iostream>

#include "../includes/objects_snake.h"

namespace s21 {
void snake_print_overlay(void);
void snake_print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void snake_print_stats(SnakeGameInfo_t *stats);
void snake_draw_field(SnakeGameInfo_t *info);
void snake_print_intro_message();
void snake_print_pause_message();
void snake_print_game_over();
}  // namespace s21
