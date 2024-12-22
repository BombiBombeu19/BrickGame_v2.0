#include "../../../includes/snake_controller.h"

namespace s21 {
SnakeUserAction_t snakeUserInput(int action) {
  if (action == KEY_LEFT)
    return SnakeUserAction_t::Left;
  else if (action == KEY_RIGHT)
    return SnakeUserAction_t::Right;
  else if (action == ENTER_KEY)
    return SnakeUserAction_t::Start;
  else if (action == ESCAPE)
    return SnakeUserAction_t::Terminate;
  else if (action == 'p' || action == 'P')
    return SnakeUserAction_t::Pause;
  else
    return SnakeUserAction_t::Action;
}
}  // namespace s21
