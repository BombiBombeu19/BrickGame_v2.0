#pragma once

#include <iostream>

#include "tetris.h"

namespace s21 {
void rotate_matrix(int matrix[][20]);
void centr_rotate(int matrix[][20], int *x, int *y);
void sort(int scores[100]);
void read_file(int *high_score);
void record_file(int high_score);
void StartGame(int input);
}  // namespace s21