#include "../includes/snake_model.h"
#include "../includes/tetris.h"

int main() {
  int num;
  system("clear");
  cout << "-------- GAME --------" << endl;
  cout << "       1 - tetris\n       2 - snake" << endl;
  cout << "----------------------" << endl;
  cout << "You chose is: ";
  cin >> num;

  if (num == 1) {
    system("clear");
    s21::tetris_game_start();
  } else if (num == 2) {
    system("clear");
    s21::SnakeGame sg = {};
    sg.snake_game_start();
  } else {
    system("clear");
    cout << "!!! 1 OR 2 !!!" << endl;
  }

  return 0;
}
