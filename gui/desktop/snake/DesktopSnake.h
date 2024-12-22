#pragma once

#include <QKeyEvent>
#include <QPoint>
#include <QVector>
#include <QWidget>

#include "../../../includes/define_snake.h"
#include "../../../includes/objects_snake.h"
#include "../../../includes/snake_controller.h"
#include "../../../includes/snake_model.h"
#include "../../../includes/snake_view.h"

namespace s21 {
class DesktopSnake : public QWidget {
 public:
  DesktopSnake();

 protected:
  void timerEvent(QTimerEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;
  void paintEvent(QPaintEvent *) override;

 private:
  static const int DOT_WIDTH = 20;
  static const int DOT_HEIGHT = 20;
  static const int FIELD_WIDTH = 20;
  static const int FIELD_HEIGHT = 20;
  static const int FIELD_WIDTH_SNAKE = 10;
  static const int FIELD_HEIGHT_SNAKE = 20;

  void doDrawing();
  void spawnApple();
  void move();
  void checkField();
  void gameOver();
  void checkApple();
  void levelUp();
  void speedUp();
  void win();
  void pause(QPainter &painter);

  int m_timerID;

  QPoint m_apple;

  bool keyUpPressed{};
  bool flagPressed{};
  QVector<QPoint> m_dots;

  void initGame();
  void initSnake();
  void setHighScore();
  void handleHighScore();

  SnakeGameInfo_t m_game_info;
  SnakeGameState_t m_game_state;
  SnakeGame::Direction m_direction;
};
}  // namespace s21
