#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>
#include <QDebug>
#include "DesktopSnake.h"
#include "DesktopTetris.h"


void setBackground(QWidget* widget, const QColor& color1, const QColor& color2) {
    QPalette palette;
    QLinearGradient gradient(0, 0, widget->width(), widget->height());
    gradient.setColorAt(0, color1);
    gradient.setColorAt(1, color2);

    QBrush brush(gradient);
    palette.setBrush(QPalette::Window, brush);
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    s21::DesktopSnake game;
    s21::DesktopSnake tetris;

    QWidget mainForm;
    mainForm.resize(240, 240);
    mainForm.setWindowTitle("BrickGame");

    setBackground(&mainForm, QColor("#cdb4db"), QColor("#a2d2ff"));

    QVBoxLayout* layout = new QVBoxLayout;

    QPushButton* button1 = new QPushButton("Tetris");
    button1->setStyleSheet(R"(
    QPushButton {
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #ff7f50, stop:1 #ff4500
        );
        color: #f0f8ff;
        border: none;
        border-radius: 10px;
        padding: 10px 20px;
    }
    QPushButton:hover {
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #ff6347, stop:1 #e63900
        );
    }
    QPushButton:pressed {
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #ff4500, stop:1 #cc2900
        );
    }
)");
    button1->setFont(QFont("Helvetica", 25, QFont::Bold));
    button1->setFixedSize(200, 60);

    QPushButton* button2 = new QPushButton("Snake");
    button2->setStyleSheet(R"(
    QPushButton {
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #27d989, stop:1 #1eae6e
        );
        color: #f0f8ff;
        border: none;
        border-radius: 10px;
        padding: 10px 20px;
    }
    QPushButton:hover {
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #20b97b, stop:1 #178d62
        );
    }
    QPushButton:pressed {
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #1eae6e, stop:1 #157951
        );
    }
)");
    button2->setFont(QFont("Helvetica", 25, QFont::Bold));
    button2->setFixedSize(200, 60);


    layout->addWidget(button1);
    layout->addWidget(button2);

    mainForm.setLayout(layout);
    mainForm.show();

    QObject::connect(button1, &QPushButton::clicked, [&]() {
        s21::DesktopTetris* tetris = new s21::DesktopTetris();
        tetris->show();
    });

    QObject::connect(button2, &QPushButton::clicked, [&]() {
        s21::DesktopSnake* game = new s21::DesktopSnake();
        game->show();
    });

    return app.exec();
}
