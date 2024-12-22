#include "../../includes/snake_view.h"

#include "../../includes/define_snake.h"
#include "../../includes/snake_model.h"

namespace s21 {

void snake_print_overlay(void) {
  start_color();
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);

  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);

  snake_print_rectangle(0, SNAKE_BOARD_N + 1, 0, BOARD_M + 1);
  snake_print_rectangle(0, SNAKE_BOARD_N + 1, BOARD_M + 2,
                        BOARD_M + HUD_WIDTH + 11);
  snake_print_rectangle(SNAKE_BOARD_N + 2, SNAKE_BOARD_N + 8, 0,
                        BOARD_M + HUD_WIDTH + 11);

  snake_print_rectangle(1, 4, BOARD_M + 3, BOARD_M + HUD_WIDTH - 1);
  snake_print_rectangle(1, 4, BOARD_M + 14, BOARD_M + HUD_WIDTH + 10);
  snake_print_rectangle(5, 7, BOARD_M + 3, BOARD_M + HUD_WIDTH + 10);
  snake_print_rectangle(8, 10, BOARD_M + 3, BOARD_M + HUD_WIDTH + 10);

  bkgd(COLOR_PAIR(1));

  MVPRINTW(2, BOARD_M + 5, "LEVEL");
  MVPRINTW(2, BOARD_M + 16, "SPEED");
  MVPRINTW(6, BOARD_M + 5, "SCORE");
  MVPRINTW(9, BOARD_M + 5, "HIGH SCORE");

  MVPRINTW(SNAKE_BOARD_N + 3, 1, "Start == ENTER or ESCAPE");
  MVPRINTW(SNAKE_BOARD_N + 4, 1, "Need For Speed == UP");
  MVPRINTW(SNAKE_BOARD_N + 4, 1, "Pause == 'P'");
  MVPRINTW(SNAKE_BOARD_N + 5, 1, "Exit == ESCAPE");
  MVPRINTW(SNAKE_BOARD_N + 6, 1, "Move == arrows");
}

void snake_draw_field(SnakeGameInfo_t *info) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      if (info->field[i][j] == 1) {
        attron(COLOR_PAIR(2));
        mvaddch(j + BEGIN_Y, i + BEGIN_X, ACS_BLOCK);
        attroff(COLOR_PAIR(2));
      } else if (info->field[i][j] == 2) {
        attron(COLOR_PAIR(2));
        mvaddch(j + BEGIN_Y, i + BEGIN_X, 'O');
        attroff(COLOR_PAIR(2));
      } else {
        attron(COLOR_PAIR(2));
        mvaddch(j + BEGIN_Y, i + BEGIN_X, ' ');
        attroff(COLOR_PAIR(2));
      }
    }
  }
  refresh();
  snake_print_stats(info);
}

void snake_print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void snake_print_stats(SnakeGameInfo_t *stats) {
  attron(COLOR_PAIR(2));
  MVPRINTW(3, BOARD_M + 7, "%d", stats->level);
  MVPRINTW(3, BOARD_M + 18, "%d", stats->speed / 1000);
  MVPRINTW(6, BOARD_M + 16, "%d", stats->score);
  MVPRINTW(9, BOARD_M + 16, "%d", stats->high_score);
  attroff(COLOR_PAIR(2));
}

void snake_print_intro_message() {
  attron(COLOR_PAIR(3));
  MVPRINTW(SNAKE_BOARD_N / 2, (BOARD_M - SNAKE_INTRO_MESSAGE_LEN) / 2 + 1,
           INTRO_MESSAGE_S);
  attroff(COLOR_PAIR(3));
  attron(COLOR_PAIR(2));
  MVPRINTW(SNAKE_BOARD_N / 2, (BOARD_M - SNAKE_INTRO_MESSAGE_LEN) / 2 + 2,
           INTRO_MESSAGE_N);
  attroff(COLOR_PAIR(2));
  attron(COLOR_PAIR(4));
  MVPRINTW(SNAKE_BOARD_N / 2, (BOARD_M - SNAKE_INTRO_MESSAGE_LEN) / 2 + 3,
           INTRO_MESSAGE_A);
  attroff(COLOR_PAIR(4));
  attron(COLOR_PAIR(5));
  MVPRINTW(SNAKE_BOARD_N / 2, (BOARD_M - SNAKE_INTRO_MESSAGE_LEN) / 2 + 4,
           INTRO_MESSAGE_K);
  attroff(COLOR_PAIR(5));
  attron(COLOR_PAIR(6));
  MVPRINTW(SNAKE_BOARD_N / 2, (BOARD_M - SNAKE_INTRO_MESSAGE_LEN) / 2 + 5,
           INTRO_MESSAGE_E);
  attroff(COLOR_PAIR(6));
}

void snake_print_pause_message() {
  MVPRINTW(SNAKE_BOARD_N / 2, (BOARD_M - PAUSE_MESSAGE_LEN) / 2 + 1,
           PAUSE_MESSAGE);
}

void snake_print_game_over() {
  attron(COLOR_PAIR(1));
  mvprintw(10, 10, "  _______ ");
  mvprintw(11, 10, " /  _____|");
  mvprintw(12, 10, "|  |  ___ ");
  mvprintw(13, 10, "|  | |_  |");
  mvprintw(14, 10, "|  |__|  |");
  mvprintw(15, 10, " \\_______/");
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(2));
  mvprintw(10, 24, "   _____   ");
  mvprintw(11, 24, "  /     \\ ");
  mvprintw(12, 24, " /  ___  \\");
  mvprintw(13, 24, "|  (___)  |");
  mvprintw(14, 24, "|   ___   |");
  mvprintw(15, 24, "|__|   |__|");
  attroff(COLOR_PAIR(2));

  attron(COLOR_PAIR(3));
  mvprintw(10, 39, "    __   __");
  mvprintw(11, 39, "   /  \\ /  \\");
  mvprintw(12, 39, "  / /\\ V /\\ \\");
  mvprintw(13, 39, " / /  \\_/  \\ \\");
  mvprintw(14, 39, "| |         | |");
  mvprintw(15, 39, "|_|         |_|");
  attroff(COLOR_PAIR(3));

  attron(COLOR_PAIR(4));
  mvprintw(10, 57, " _______ ");
  mvprintw(11, 57, "|   ____|");
  mvprintw(12, 57, "|  |__   ");
  mvprintw(13, 57, "|   __|  ");
  mvprintw(14, 57, "|  |____ ");
  mvprintw(15, 57, "|_______|");
  attroff(COLOR_PAIR(4));

  attron(COLOR_PAIR(5));
  mvprintw(10, 75, "   _______ ");
  mvprintw(11, 75, "  /   _   \\ ");
  mvprintw(12, 75, " /   / \\   \\ ");
  mvprintw(13, 75, "|   (   )   |");
  mvprintw(14, 75, " \\   \\_/   / ");
  mvprintw(15, 75, "  \\_______/  ");
  attroff(COLOR_PAIR(5));

  attron(COLOR_PAIR(6));
  mvprintw(10, 91, "___      ___ ");
  mvprintw(11, 91, "\\  \\    /  / ");
  mvprintw(12, 91, " \\  \\  /  /  ");
  mvprintw(13, 91, "  \\  \\/  /   ");
  mvprintw(14, 91, "   \\    /    ");
  mvprintw(15, 91, "    \\__/     ");
  attroff(COLOR_PAIR(6));

  attron(COLOR_PAIR(1));
  mvprintw(10, 108, " _______ ");
  mvprintw(11, 108, "|   ____|");
  mvprintw(12, 108, "|  |__   ");
  mvprintw(13, 108, "|   __|  ");
  mvprintw(14, 108, "|  |____ ");
  mvprintw(15, 108, "|_______|");
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(2));
  mvprintw(10, 121, " _______");
  mvprintw(11, 121, "|  ___  \\");
  mvprintw(12, 121, "| |   |  |");
  mvprintw(13, 121, "| |___/ /");
  mvprintw(14, 121, "|  __  |");
  mvprintw(15, 121, "|_|  \\__\\");
  snake_print_rectangle(14, 14, 8, 128);
  attroff(COLOR_PAIR(2));
}
}  // namespace s21
