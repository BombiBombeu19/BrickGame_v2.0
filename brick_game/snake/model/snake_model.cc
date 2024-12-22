#include "../../../includes/snake_model.h"

namespace s21 {
SnakeGame::SnakeGame() {
  srand(time(NULL));

  Snake new_snake = {};
  snake = new_snake;
  snake.len = 0;
  game_info.high_score = 0;
  game_info.level = 1;
  game_info.score = 0;
  game_info.pause = 0;
  game_info.speed = 350000;

  apple = {0, 0};

  game_state = s21::SnakeGameState_t::StartState;
  user_action = s21::SnakeUserAction_t::Action;
  direction_state = Direction::DirectionLeft;
}

SnakeGame::~SnakeGame() {}

void SnakeGame::snake_game_start() {
  srand(time(NULL));
  WIN_INIT(50);
  setlocale(LC_ALL, "");

  this->game_init();

  endwin();
}

void SnakeGame::game_init() {
  game_state = s21::SnakeGameState_t::StartState;
  bool break_flag = 1;
  int high = 0;

  snake_clear_game_field(game_info.field);
  this->read_file(&high);

  game_info.high_score = high;

  generate_random_apple();

  while (break_flag) {
    int input = getch();
    if (game_state == s21::SnakeGameState_t::StartState) {
      this->StartGame(input);
    }
    this->updateCurrentState(input);
    if (game_state == s21::SnakeGameState_t::GameOverState) {
      break_flag = false;
      this->record_file(game_info.score);
      clear();
      snake_print_game_over();
      getch();
      sleep(3);
    } else if (game_state == s21::SnakeGameState_t::PauseState) {
      snake_print_pause_message();
    } else {
      snake_print_overlay();
      snake_draw_field(&game_info);
    }
  }
}

void SnakeGame::sort(int scores[100]) {
  int i, j, tmp;
  for (i = 0; i < 100; ++i) {
    for (j = i + 1; j < 100; ++j) {
      if (scores[i] < scores[j]) {
        tmp = scores[i];
        scores[i] = scores[j];
        scores[j] = tmp;
      }
    }
  }
}

void SnakeGame::read_file(int *high_score) {
  *high_score = 0;
  int scores[100] = {0};
  FILE *fp;
  if ((fp = fopen("high_score_snake.txt", "r")) == NULL) {
    printw("Cannot open file!\n");
    exit(1);
  }
  char *line = NULL;
  size_t len = 0;
  int i = 0;
  while ((getline(&line, &len, fp) != -1)) {
    if (strlen(line) > 0) {
      scores[i] = atoi(line);
      i++;
    }
  }
  sort(scores);
  *high_score = scores[0];

  free(line);
  fclose(fp);
}

void SnakeGame::record_file(int high_score) {
  FILE *fp;
  if ((fp = fopen("high_score_snake.txt", "a")) == NULL) {
    printw("Cannot open file!\n");
    exit(1);
  }
  fprintf(fp, "%d\n", high_score);

  fclose(fp);
}

void SnakeGame::StartGame(int input) {
  (void)input;
  bool flag = true;
  while (flag) {
    snake_print_overlay();
    snake_print_intro_message();
    input = getch();
    if (input == ENTER_KEY || input == ESCAPE) flag = false;
    refresh();
    getch();
  }
}

int SnakeGame::check_game_over(int matrix[][20]) {
  int flag = TRUE;
  for (int i = 0; i < 10; ++i)
    if (matrix[i][0] == 1) flag = 0;

  return flag;
}

void SnakeGame::game_controller(int input) {
  switch (snakeUserInput(input)) {
    case s21::SnakeUserAction_t::Terminate:
      game_state = s21::SnakeGameState_t::GameOverState;
      break;

    case s21::SnakeUserAction_t::Left:
      if (direction_state == Direction::DirectionLeft)
        direction_state = Direction::DirectionDown;
      else if (direction_state == Direction::DirectionRight)
        direction_state = Direction::DirectionUp;
      else if (direction_state == Direction::DirectionUp)
        direction_state = Direction::DirectionLeft;
      else if (direction_state == Direction::DirectionDown)
        direction_state = Direction::DirectionRight;

      game_state = s21::SnakeGameState_t::MovingState;
      break;

    case s21::SnakeUserAction_t::Right:
      if (direction_state == Direction::DirectionLeft)
        direction_state = Direction::DirectionUp;
      else if (direction_state == Direction::DirectionRight)
        direction_state = Direction::DirectionDown;
      else if (direction_state == Direction::DirectionUp)
        direction_state = Direction::DirectionRight;
      else if (direction_state == Direction::DirectionDown)
        direction_state = Direction::DirectionLeft;

      game_state = s21::SnakeGameState_t::MovingState;
      break;

    case s21::SnakeUserAction_t::Pause:
      game_state = s21::SnakeGameState_t::PauseState;
      break;

    default:
      break;
  }
  game_info.field[apple.x][apple.y] = 2;
}

void SnakeGame::updateCurrentState(int input) {
  switch (game_state) {
    case s21::SnakeGameState_t::StartState:
      snake_spawn();
      game_state = s21::SnakeGameState_t::MovingState;
      break;

    case s21::SnakeGameState_t::MovingState:
      if (input == KEY_UP) {
        game_state = s21::SnakeGameState_t::ShiftingState;
      } else
        usleep(game_info.speed);
      snake_clear_game_field(game_info.field);
      game_controller(input);
      snake_moving();
      snake_init();
      break;

    case s21::SnakeGameState_t::ShiftingState:
      usleep(game_info.speed / 2);
      game_state = s21::SnakeGameState_t::MovingState;
      break;

    case s21::SnakeGameState_t::PauseState:
      game_state = s21::SnakeGameState_t::PauseState;
      if (snakeUserInput(input) == s21::SnakeUserAction_t::Pause)
        game_state = s21::SnakeGameState_t::MovingState;
      break;

    default:
      break;
  }
}

void SnakeGame::snake_clear_game_field(int field[10][20]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      field[i][j] = 0;
    }
  }
}

void SnakeGame::snake_spawn() {
  snake.snake_body[0] = {3, 5};
  snake.snake_body[1] = {4, 5};
  snake.snake_body[2] = {5, 5};
  snake.snake_body[3] = {6, 5};

  snake.len = 4;

  snake_init();
}

void SnakeGame::snake_init() {
  for (int i = 0; i < snake.len; ++i) {
    game_info.field[snake.snake_body[i].x][snake.snake_body[i].y] = 1;
  }
}

void SnakeGame::snake_moving() {
  int new_x = snake.snake_body[0].x;
  int new_y = snake.snake_body[0].y;

  if (direction_state == Direction::DirectionLeft) {
    new_x -= 1;
  } else if (direction_state == Direction::DirectionRight) {
    new_x += 1;
  } else if (direction_state == Direction::DirectionUp) {
    new_y -= 1;
  } else if (direction_state == Direction::DirectionDown) {
    new_y += 1;
  }

  if (new_x < 0 || new_x >= 10 || new_y < 0 || new_y >= 20) {
    game_state = s21::SnakeGameState_t::GameOverState;
    return;
  }

  for (int i = 1; i < snake.len; ++i) {
    if (new_x == snake.snake_body[i].x && new_y == snake.snake_body[i].y) {
      game_state = s21::SnakeGameState_t::GameOverState;
      return;
    }
  }

  if (new_x == apple.x && new_y == apple.y) {
    snake.len += 1;
    game_info.score += 1;
    if (game_info.score > 0 && game_info.score <= 50 &&
        (game_info.score % 5) == 0) {
      add_points();
    }
    generate_random_apple();
  }

  for (int i = snake.len - 1; i > 0; --i) {
    snake.snake_body[i] = snake.snake_body[i - 1];
  }

  snake.snake_body[0] = {new_x, new_y};
}

void SnakeGame::generate_random_apple() {
  do {
    apple.x = rand() % 10;
    apple.y = rand() % 20;
  } while (!is_correct_coordinate(apple.x, apple.y));
}

bool SnakeGame::is_correct_coordinate(int x, int y) {
  if (x < 0 || x >= 10 || y < 0 || y >= 20) {
    return false;
  }

  for (int i = 0; i < snake.len; ++i) {
    if (snake.snake_body[i].x == x && snake.snake_body[i].y == y) {
      return false;
    }
  }
  return true;
}

void SnakeGame::add_points() {
  game_info.speed -= 10000;
  game_info.level += 1;
}
}  // namespace s21
