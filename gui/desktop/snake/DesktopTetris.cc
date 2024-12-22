#include "DesktopTetris.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QRandomGenerator>
#include <QRect>

namespace s21 {

DesktopTetris::DesktopTetris() {
  resize(DOT_WIDTH * DOT_WIDTH, DOT_HEIGHT * DOT_HEIGHT);

  t_game_info.level = 1;
  t_game_info.score = 0;
  t_game_info.high_score = 0;
  t_game_info.pause = 0;
  t_game_info.speed = 250;
  t_game_info.pause = 0;
  clear_game_field(t_game_info.field);
  clear_game_field(t_game_info.next);
  clear_game_field(t_game_info.figure_m);

  setWindowTitle("TETRIS");

  t_timerID = -1;

  t_game_state = GameState_t::START_STATE;
  t_input = KEY_DOWN;

  initGame();
}

void DesktopTetris::initGame() {
  handleHighScore();

  t_timerID = startTimer(t_game_info.speed);
}

void DesktopTetris::timerEvent(QTimerEvent *) {
  if (t_game_state == GameState_t::GAMEOVER_STATE) {
    if (t_timerID != -1) {
      killTimer(t_timerID);
    }
    setHighScore();
    close();
  } else if (t_game_state != GameState_t::PAUSE_STATE) {
    updateCurrentState(&t_game_info, &t_game_state, t_input);
    t_input = KEY_DOWN;
  }

  this->update();
}

int DesktopTetris::parseKey(Qt::Key key) {
  auto it = key_map_.find(key);
  if (it != key_map_.end()) {
    return it->second;
  }
  return KEY_DOWN;
}

void DesktopTetris::keyPressEvent(QKeyEvent *event) {
  int key = event->key();

  switch (key) {
    case Qt::Key_Down:
      keyDownPressed = false;
      if (flagPressed == false) {
        speedUp();
      }
      break;

    case Qt::Key_P:
      if (t_game_state != GameState_t::PAUSE_STATE) {
        t_game_state = GameState_t::PAUSE_STATE;
      } else {
        t_game_state = GameState_t::MOVING_STATE;
      }
      return;
  }

  t_input = parseKey(static_cast<Qt::Key>(key));
}

void DesktopTetris::keyReleaseEvent(QKeyEvent *event) {
  int key = event->key();
  if (key == Qt::Key_Down) {
    keyDownPressed = true;
    speedUp();
  }
}

void DesktopTetris::paintEvent(QPaintEvent *) { doDrawing(); }

void DesktopTetris::doDrawing() {
  QPainter qp(this);

  QPixmap background("images/t_background.jpg");
  qp.drawPixmap(rect(), background);

  if (t_game_state == GameState_t::PAUSE_STATE) {
    pause(qp);
  } else if (t_game_state != GameState_t::GAMEOVER_STATE) {
    qp.setPen(Qt::black);
    qp.setBrush(Qt::black);

    qp.drawLine(FIELD_WIDTH_TETRIS * DOT_WIDTH, 0,
                FIELD_WIDTH_TETRIS * DOT_WIDTH,
                FIELD_HEIGHT_TETRIS * DOT_HEIGHT);
    qp.drawLine(
        FIELD_WIDTH_TETRIS * DOT_WIDTH, FIELD_HEIGHT_TETRIS * DOT_HEIGHT / 2,
        FIELD_HEIGHT_TETRIS * DOT_WIDTH, FIELD_HEIGHT_TETRIS * DOT_HEIGHT / 2);

    QFont font = qp.font();
    font.setPointSize(18);
    font.setBold(true);
    qp.setFont(font);

    qp.drawText(DOT_WIDTH + 200, 40,
                "LEVEL: " + QString::number(t_game_info.level));
    qp.drawText(DOT_WIDTH + 200, 80,
                "SPEED: " + QString::number((260 - t_game_info.speed) / 10));
    qp.drawText(DOT_WIDTH + 200, 120,
                "SCORE: " + QString::number(t_game_info.score));
    qp.drawText(DOT_WIDTH + 200, 160,
                "HIGH SCORE: " + QString::number(t_game_info.high_score));

    QColor randomColorFigure(QRandomGenerator::global()->bounded(256),
                             QRandomGenerator::global()->bounded(256),
                             QRandomGenerator::global()->bounded(256));
    qp.setBrush(QBrush(randomColorFigure));
    drawField(qp, t_game_info.figure_m);

    QColor randomColorField(QRandomGenerator::global()->bounded(256),
                            QRandomGenerator::global()->bounded(256),
                            QRandomGenerator::global()->bounded(256));
    qp.setBrush(QBrush(randomColorField));
    drawField(qp, t_game_info.field);

    QColor randomColorNext(QRandomGenerator::global()->bounded(256),
                           QRandomGenerator::global()->bounded(256),
                           QRandomGenerator::global()->bounded(256));
    qp.setBrush(QBrush(randomColorNext));
    drawFieldNext(qp, t_game_info.next);
  }
}

void DesktopTetris::drawField(QPainter &painter, int field[][20]) {
  for (int i = 0; i < FIELD_HEIGHT_TETRIS; i++) {
    for (int j = 0; j < FIELD_WIDTH_TETRIS; j++) {
      if (field[j][i] == 1) {
        QRect rect(j * DOT_WIDTH, 20 + i * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);
        painter.setPen(Qt::black);
        painter.drawRect(rect);
      }
    }
  }
}

void DesktopTetris::drawFieldNext(QPainter &painter, int field[][20]) {
  for (int i = 0; i < FIELD_HEIGHT_TETRIS; i++) {
    for (int j = 0; j < FIELD_WIDTH_TETRIS; j++) {
      if (field[j][i] == 1) {
        QRect rect(190 + j * DOT_WIDTH, 250 + i * DOT_HEIGHT, DOT_WIDTH,
                   DOT_HEIGHT);
        painter.setPen(Qt::black);
        painter.drawRect(rect);
      }
    }
  }
}

void DesktopTetris::speedUp() {
  if (!keyDownPressed) {
    t_game_info.speed -= 50;
    if (t_timerID != -1) killTimer(t_timerID);
    t_timerID = startTimer(t_game_info.speed);
    flagPressed = true;

  } else {
    keyDownPressed = false;
    t_game_info.speed += 50;
    killTimer(t_timerID);
    t_timerID = startTimer(t_game_info.speed);
    flagPressed = false;
  }
}

void DesktopTetris::pause(QPainter &painter) {
  painter.setPen(Qt::white);
  QFont font = painter.font();
  font.setPointSize(24);
  font.setBold(true);
  painter.setFont(font);
  painter.drawText(DOT_WIDTH * DOT_WIDTH / 2 - 45, height() / 2, "Paused");
}

void DesktopTetris::handleHighScore() {
  QFile file("high_score.txt");
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file);
    QString line;
    while (!stream.atEnd()) {
      line = stream.readLine();
    }
    if (!line.isEmpty()) {
      t_game_info.high_score = line.toInt();
    }
    file.close();
  } else {
    qDebug() << "File not found!";
  }
}

void DesktopTetris::setHighScore() {
  QFile file("high_score.txt");
  if (file.open(QIODevice::Append)) {
    QTextStream stream(&file);
    if (t_game_info.score >= t_game_info.high_score) {
      t_game_info.high_score = t_game_info.score;
      stream << QString::number(t_game_info.high_score) << Qt::endl;
    }
    file.close();
  } else {
    qDebug() << "File not found!";
  }
}

}  // namespace s21
