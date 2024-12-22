#include "DesktopSnake.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QRandomGenerator>

namespace s21 {

DesktopSnake::DesktopSnake() {
  resize(DOT_WIDTH * FIELD_WIDTH, DOT_HEIGHT * FIELD_HEIGHT);

  m_game_info.level = 1;
  m_game_info.score = 0;
  m_game_info.high_score = 0;
  m_game_info.pause = 0;
  m_game_info.speed = 250;

  setWindowTitle("Snake Game");

  m_dots.resize(4);
  m_timerID = -1;

  m_game_state = SnakeGameState_t::MovingState;
  m_direction = SnakeGame::Direction::DirectionRight;

  initGame();
}

void DesktopSnake::initGame() {
  for (int i = 0; i < m_dots.size(); ++i) {
    m_dots[i].rx() = m_dots.size() - i;
    m_dots[i].ry() = 0;
  }

  handleHighScore();

  spawnApple();

  m_timerID = startTimer(m_game_info.speed);
}

void DesktopSnake::timerEvent(QTimerEvent *) {
  if (m_game_state == SnakeGameState_t::GameOverState) {
    if (m_timerID != -1) {
      killTimer(m_timerID);
    }
    setHighScore();
    close();
  } else if (m_game_state != SnakeGameState_t::PauseState) {
    checkApple();
    move();
    checkField();
  }

  this->update();
}

void DesktopSnake::keyPressEvent(QKeyEvent *event) {
  int key = event->key();

  switch (key) {
    case Qt::Key_Up:
      keyUpPressed = false;
      if (flagPressed == false) {
        speedUp();
      }
      break;

    case Qt::Key_Escape:
      m_game_state = SnakeGameState_t::GameOverState;
      break;

    case Qt::Key_Left:
      if (m_direction == SnakeGame::Direction::DirectionLeft)
        m_direction = SnakeGame::Direction::DirectionDown;
      else if (m_direction == SnakeGame::Direction::DirectionRight)
        m_direction = SnakeGame::Direction::DirectionUp;
      else if (m_direction == SnakeGame::Direction::DirectionUp)
        m_direction = SnakeGame::Direction::DirectionLeft;
      else if (m_direction == SnakeGame::Direction::DirectionDown)
        m_direction = SnakeGame::Direction::DirectionRight;

      m_game_state = SnakeGameState_t::MovingState;
      break;

    case Qt::Key_Right:
      if (m_direction == SnakeGame::Direction::DirectionLeft)
        m_direction = SnakeGame::Direction::DirectionUp;
      else if (m_direction == SnakeGame::Direction::DirectionRight)
        m_direction = SnakeGame::Direction::DirectionDown;
      else if (m_direction == SnakeGame::Direction::DirectionUp)
        m_direction = SnakeGame::Direction::DirectionRight;
      else if (m_direction == SnakeGame::Direction::DirectionDown)
        m_direction = SnakeGame::Direction::DirectionLeft;

      m_game_state = SnakeGameState_t::MovingState;
      break;

    case Qt::Key_P:
      if (m_game_state != SnakeGameState_t::PauseState) {
        m_game_state = SnakeGameState_t::PauseState;
      } else {
        m_game_state = SnakeGameState_t::MovingState;
      }
      break;
  }
}

void DesktopSnake::keyReleaseEvent(QKeyEvent *event) {
  int key = event->key();

  if (key == Qt::Key_Up) {
    keyUpPressed = true;
    speedUp();
  }
}

void DesktopSnake::paintEvent(QPaintEvent *) { doDrawing(); }

void DesktopSnake::doDrawing() {
  QPainter qp(this);

  QPixmap background("images/s_background.jpg");
  qp.drawPixmap(rect(), background);

  if (m_game_state == SnakeGameState_t::PauseState) {
    pause(qp);
  } else if (m_game_state != SnakeGameState_t::GameOverState) {
    qp.setPen(Qt::black);
    qp.setBrush(Qt::black);

    QPixmap applePixmap("images/apple_image.jpg");
    qp.drawPixmap(m_apple.x() * DOT_WIDTH, m_apple.y() * DOT_HEIGHT, DOT_WIDTH,
                  DOT_HEIGHT, applePixmap);

    qp.drawLine(FIELD_WIDTH_SNAKE * DOT_WIDTH, 0, FIELD_WIDTH_SNAKE * DOT_WIDTH,
                FIELD_HEIGHT_SNAKE * DOT_HEIGHT);

    QFont font = qp.font();
    font.setPointSize(20);
    font.setBold(true);
    qp.setFont(font);

    qp.drawText(DOT_WIDTH + 200, 80,
                "LEVEL: " + QString::number(m_game_info.level));
    qp.drawText(DOT_WIDTH + 200, 160,
                "SPEED: " + QString::number((260 - m_game_info.speed) / 10));
    qp.drawText(DOT_WIDTH + 200, 240,
                "SCORE: " + QString::number(m_game_info.score));
    qp.drawText(DOT_WIDTH + 200, 320,
                "HIGH SCORE: " + QString::number(m_game_info.high_score));

    for (int i = 0; i < m_dots.size(); ++i) {
      if (i == 0) {
        qp.setBrush(Qt::white);
      } else {
        QColor randomColorFigure(QRandomGenerator::global()->bounded(256),
                                 QRandomGenerator::global()->bounded(256),
                                 QRandomGenerator::global()->bounded(256));
        qp.setBrush(QBrush(randomColorFigure));
      }
      qp.drawRect(m_dots[i].x() * DOT_WIDTH, m_dots[i].y() * DOT_HEIGHT,
                  DOT_WIDTH, DOT_HEIGHT);
    }
  }
}

void DesktopSnake::spawnApple() {
  m_apple.rx() = QRandomGenerator::global()->bounded(FIELD_WIDTH_SNAKE);
  m_apple.ry() = QRandomGenerator::global()->bounded(FIELD_HEIGHT_SNAKE);
}

void DesktopSnake::move() {
  for (int i = m_dots.size() - 1; i > 0; --i) {
    m_dots[i] = m_dots[i - 1];
  }

  switch (m_direction) {
    case SnakeGame::Direction::DirectionLeft: {
      m_dots[0].rx() -= 1;
      break;
    }
    case SnakeGame::Direction::DirectionRight: {
      m_dots[0].rx() += 1;
      break;
    }
    case SnakeGame::Direction::DirectionUp: {
      m_dots[0].ry() -= 1;
      break;
    }
    case SnakeGame::Direction::DirectionDown: {
      m_dots[0].ry() += 1;
      break;
    }
  }
}

void DesktopSnake::checkField() {
  for (int i = 1; i < m_dots.size(); ++i) {
    if (m_dots[0] == m_dots[i]) {
      m_game_state = SnakeGameState_t::GameOverState;
    }
  }
  if (m_dots[0].x() >= FIELD_WIDTH_SNAKE) {
    m_game_state = SnakeGameState_t::GameOverState;
  } else if (m_dots[0].x() < 0) {
    m_game_state = SnakeGameState_t::GameOverState;
  } else if (m_dots[0].y() >= FIELD_HEIGHT_SNAKE) {
    m_game_state = SnakeGameState_t::GameOverState;
  } else if (m_dots[0].y() < 0) {
    m_game_state = SnakeGameState_t::GameOverState;
  }
}

void DesktopSnake::checkApple() {
  if (m_apple == m_dots[0]) {
    m_dots.push_back(QPoint(0, 0));
    spawnApple();
    m_game_info.score++;

    if (m_game_info.score > 0 && m_game_info.score <= 50 &&
        (m_game_info.score % 5) == 0) {
      levelUp();
    }

    if (m_game_info.score == 200) {
      win();
    }
  }
}

void DesktopSnake::levelUp() {
  ++m_game_info.level;
  m_game_info.speed -= 10;
}

void DesktopSnake::speedUp() {
  if (!keyUpPressed) {
    m_game_info.speed -= 50;
    if (m_timerID != -1) killTimer(m_timerID);
    m_timerID = startTimer(m_game_info.speed);
    flagPressed = true;

  } else {
    keyUpPressed = false;
    m_game_info.speed += 50;
    killTimer(m_timerID);
    m_timerID = startTimer(m_game_info.speed);
    flagPressed = false;
  }
}

void DesktopSnake::win() {
  QMessageBox msgb;
  msgb.setText("!!! YOU WIN !!!");
  msgb.setStandardButtons(QMessageBox::Close);
  msgb.setDefaultButton(QMessageBox::Close);
  int ret = msgb.exec();
  m_game_state = SnakeGameState_t::GameOverState;

  switch (ret) {
    case QMessageBox::Close:
      break;
  }
}

void DesktopSnake::pause(QPainter &painter) {
  painter.setPen(Qt::white);
  QFont font = painter.font();
  font.setPointSize(24);
  font.setBold(true);
  painter.setFont(font);
  painter.drawText(DOT_WIDTH * FIELD_WIDTH / 2 - 45, height() / 2, "Paused");
}

void DesktopSnake::handleHighScore() {
  QFile file("high_score_snake.txt");
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file);
    QString line;
    while (!stream.atEnd()) {
      line = stream.readLine();
    }
    if (!line.isEmpty()) {
      m_game_info.high_score = line.toInt();
    }
    file.close();
  } else {
    qDebug() << "File not found!";
  }
}

void DesktopSnake::setHighScore() {
  QFile file("high_score_snake.txt");
  if (file.open(QIODevice::Append)) {
    QTextStream stream(&file);
    if (m_game_info.score > m_game_info.high_score) {
      m_game_info.high_score = m_game_info.score;
      stream << QString::number(m_game_info.high_score) << Qt::endl;
    }
    file.close();
  } else {
    qDebug() << "File not found!";
  }
}
}  // namespace s21
