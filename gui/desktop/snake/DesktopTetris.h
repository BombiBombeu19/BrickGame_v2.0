#pragma once

#include <QKeyEvent>
#include <QPoint>
#include <QVector>
#include <QWidget>

#include "../../../includes/backend_tetris.h"
#include "../../../includes/helper_tetris.h"
#include "../../../includes/objects_tetris.h"

namespace s21 {
class DesktopTetris : public QWidget {
 public:
  DesktopTetris();

 protected:
  void timerEvent(QTimerEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;
  void paintEvent(QPaintEvent *) override;

 private:
  std::unordered_map<Qt::Key, int> key_map_{
      {Qt::Key_Left, KEY_LEFT}, {Qt::Key_Right, KEY_RIGHT},
      {Qt::Key_Down, KEY_DOWN}, {Qt::Key_P, 'p'},
      {Qt::Key_Escape, 27},     {Qt::Key_Space, ' '},
  };

  static const int DOT_WIDTH = 20;
  static const int DOT_HEIGHT = 20;
  static const int FIELD_WIDTH_TETRIS = 10;
  static const int FIELD_HEIGHT_TETRIS = 20;

  void doDrawing();
  void drawField(QPainter &painter, int field[][20]);
  void drawFieldNext(QPainter &painter, int field[][20]);

  int t_timerID{};
  int t_input{};

  bool keyDownPressed{};
  bool flagPressed{};

  void initGame();
  void initSnake();
  void setHighScore();
  void handleHighScore();
  void speedUp();
  void pause(QPainter &painter);
  int parseKey(Qt::Key key);

  GameInfo_t t_game_info;
  GameState_t t_game_state;
};
}  // namespace s21
