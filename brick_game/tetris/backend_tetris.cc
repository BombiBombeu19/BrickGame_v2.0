#include "../../includes/backend_tetris.h"

namespace s21 {
void clear_game_field(int field[10][20]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      field[i][j] = 0;
    }
  }
}

void spawn_new_figure(int matrix[][20]) {
  int value = rand() % 7 + 1;
  int figures[7][4] = {{1, 3, 5, 7}, {2, 4, 5, 7}, {3, 4, 5, 6}, {3, 4, 5, 7},
                       {2, 3, 5, 7}, {3, 5, 6, 7}, {2, 3, 4, 5}};
  for (int c = 0; c < 4; c++) {
    int x = figures[value - 1][c] % 2 + 4;
    int y = figures[value - 1][c] / 2;
    matrix[x][y] = 1;
  }

  if (value != 1) move_up_matrix(matrix);
}

UserAction_t userInput(int action) {
  if (action == KEY_UP)
    return UserAction_t::Up;
  else if (action == KEY_DOWN)
    return UserAction_t::Down;
  else if (action == KEY_LEFT)
    return UserAction_t::Left;
  else if (action == KEY_RIGHT)
    return UserAction_t::Right;
  else if (action == ' ')
    return UserAction_t::Rotate;
  else if (action == ENTER_KEY)
    return UserAction_t::Start;
  else if (action == ESCAPE)
    return UserAction_t::Terminate;
  else if (action == 'p' || action == 'P')
    return UserAction_t::Pause;
  else
    return UserAction_t::Action;
}

void game_controller(GameInfo_t *info, GameState_t *state, int input) {
  switch (userInput(input)) {
    case UserAction_t::Terminate:
      *state = GameState_t::GAMEOVER_STATE;
      break;
    case UserAction_t::Left:
      move_left_matrix(info->figure_m);
      if (check_matrix(info) == 0) move_right_matrix(info->figure_m);
      *state = GameState_t::MOVING_STATE;
      break;
    case UserAction_t::Right:
      move_right_matrix(info->figure_m);
      if (check_matrix(info) == 0) move_left_matrix(info->figure_m);
      *state = GameState_t::MOVING_STATE;
      break;
    case UserAction_t::Down:
      move_down_matrix(info->figure_m);
      if (check_matrix(info) == 0) {
        move_up_matrix(info->figure_m);
        *state = GameState_t::ATTACHING_STATE;
      } else {
        *state = GameState_t::MOVING_STATE;
      }
      break;
    case UserAction_t::Rotate:
      rotate_matrix(info->figure_m);
      break;
    case UserAction_t::Pause:
      *state = GameState_t::PAUSE_STATE;
      break;
    default:
      break;
  }
}

void updateCurrentState(GameInfo_t *data, GameState_t *state, int input) {
  struct timespec ts = {.tv_sec = 0,
                        .tv_nsec = static_cast<long>(0.001 * 1000000000L)};
  int rows_ = 0;
  switch (*state) {
    case GameState_t::START_STATE:
      *state = GameState_t::MOVING_STATE;
      clear_game_field(data->field);
      clear_game_field(data->figure_m);
      clear_game_field(data->next);
      spawn_new_figure(data->figure_m);
      spawn_new_figure(data->next);
      break;
    case GameState_t::SPAWN_STATE:
      *state = GameState_t::MOVING_STATE;
      clear_game_field(data->figure_m);
      copy_matrix(data->figure_m, data->next);
      clear_game_field(data->next);
      spawn_new_figure(data->next);
      break;
    case GameState_t::MOVING_STATE:
      *state = GameState_t::SHIFTING_STATE;
      game_controller(data, state, input);
      break;
    case GameState_t::SHIFTING_STATE:
      if (check_matrix(data) == 1) {
        *state = GameState_t::MOVING_STATE;
        move_down_matrix(data->figure_m);

        ts.tv_nsec = 0.01 * data->speed * 1000000000L;
        nanosleep(&ts, NULL);

        if (data->high_score < data->score) {
          data->high_score = data->score;
        }
      } else {
        move_up_matrix(data->figure_m);
        *state = GameState_t::ATTACHING_STATE;
      }
      break;
    case GameState_t::ATTACHING_STATE:
      *state = GameState_t::SPAWN_STATE;
      add_matrix(data->field, data->field, data->figure_m);
      data->score += 4;
      if (data->score > data->high_score) data->high_score = data->score;
      rows_ = shift_rows(data->field);
      add_points(data, rows_);
      if (data->score > 600 * (data->level + 1) && data->level < 10) {
        data->level++;
        data->speed -= 5;
      }
      if (check_game_over(data->field) == 0)
        *state = GameState_t::GAMEOVER_STATE;
      break;
    case GameState_t::PAUSE_STATE:
      *state = GameState_t::PAUSE_STATE;
      if (userInput(input) == UserAction_t::Pause)
        *state = GameState_t::MOVING_STATE;
      break;
    default:
      break;
  }
}

void shift_matrix_down(int matrix[][20], int start_row) {
  for (int i = 0; i < 10; ++i) {
    for (int k = start_row; k >= 0; --k) {
      if (matrix[i][k] == 1) {
        matrix[i][k] = 0;
        matrix[i][k + 1] = 1;
      }
    }
  }
}

void move_down_matrix(int matrix[][20]) { shift_matrix_down(matrix, 19); }

int check_matrix(const GameInfo_t *data) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 20; ++j) {
      if (data->field[i][j] == 1 && data->figure_m[i][j] == 1) {
        return 0;
      }
      if (data->figure_m[i][j] == 1 && j == 19) {
        return 0;
      }
    }
  }
  return 1;
}

void move_up_matrix(int matrix[][20]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      if (matrix[i][j] == 1) {
        matrix[i][j] = 0;
        matrix[i][j - 1] = 1;
      }
    }
  }
}

void copy_matrix(int res[][20], int dest[][20]) {
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 20; j++) res[i][j] = dest[i][j];
}

void move_left_matrix(int matrix[][20]) {
  int check_left = 1;
  for (int j = 0; j < 20; ++j)
    if (matrix[0][j] == 1) check_left = 0;

  if (check_left == 1) {
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 20; ++j) {
        if (matrix[i][j] == 1) {
          matrix[i][j] = 0;
          matrix[i - 1][j] = 1;
        }
      }
    }
  }
}

void move_right_matrix(int matrix[][20]) {
  int check_left = 1;
  for (int j = 0; j < 20; ++j)
    if (matrix[9][j] == 1) check_left = 0;

  if (check_left == 1) {
    for (int i = 9; i >= 0; --i) {
      for (int j = 0; j < 20; ++j) {
        if (matrix[i][j] == 1) {
          matrix[i][j] = 0;
          matrix[i + 1][j] = 1;
        }
      }
    }
  }
}

void add_matrix(int matrix_res[][20], const int matrix_1[][20],
                const int matrix_2[][20]) {
  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 20; ++j)
      matrix_res[i][j] = matrix_1[i][j] + matrix_2[i][j];
}

int shift_rows(int matrix[][20]) {
  int row_count = 0;
  for (int j = 19; j >= 0; --j) {
    if (is_full_row(matrix, j) == TRUE) {
      for (int i = 0; i < 10; i++) matrix[i][j] = 0;
      shift_matrix_down(matrix, j);
      j++;
      row_count++;
    }
  }
  return row_count;
}

void add_points(GameInfo_t *data, int rows_count) {
  if (rows_count == 1)
    data->score += 100;
  else if (rows_count == 2)
    data->score += 300;
  else if (rows_count == 3)
    data->score += 700;
  else if (rows_count == 4)
    data->score += 1500;
}

int check_game_over(int matrix[][20]) {
  int flag = TRUE;
  for (int i = 0; i < 10; ++i)
    if (matrix[i][0] == 1) flag = 0;

  return flag;
}

int is_full_row(int matrix[][20], int row) {
  int sum = 0;
  int flag = FALSE;
  for (int i = 0; i < 10; ++i) sum += matrix[i][row];

  if (sum == 10) flag = TRUE;

  return flag;
}
}  // namespace s21
