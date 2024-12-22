#include "../../includes/tetris.h"

#include "../../includes/backend_tetris.h"

namespace s21 {

void tetris_game_start() {
  srand(time(NULL));
  WIN_INIT(50);
  setlocale(LC_ALL, "");

  s21::game_init();

  endwin();
}

void game_init() {
  GameInfo_t data;
  GameState_t state = GameState_t::START_STATE;
  bool break_flag = TRUE;
  int high = 0;

  init_stats(&data);
  read_file(&high);

  data.high_score = high;
  data.speed = 50;

  while (break_flag) {
    int input = getch();
    if (state == GameState_t::START_STATE) {
      StartGame(input);
    }
    updateCurrentState(&data, &state, input);
    if (state == GameState_t::GAMEOVER_STATE) {
      break_flag = false;
      record_file(data.score);
      clear();
      print_game_over();
      getch();
      sleep(3);
    } else if (state == GameState_t::PAUSE_STATE) {
      print_pause_message();
    } else {
      print_overlay();
      if (state == GameState_t::START_STATE ||
          state == GameState_t::SPAWN_STATE ||
          state == GameState_t::MOVING_STATE)
        draw_next_info(&data);
      draw_field(&data);
    }
  }
}

void init_stats(GameInfo_t *stats) {
  stats->level = 1;
  stats->score = 0;
  stats->speed = 1;
  stats->high_score = 0;
  stats->pause = 0;
  clear_game_field(stats->field);
  clear_game_field(stats->next);
  clear_game_field(stats->figure_m);
}
}  // namespace s21
