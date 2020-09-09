#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <QWidget>
#include <QPainter>

class Main_Game : public QWidget
{
    Q_OBJECT
public:
    explicit Main_Game(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

signals:

};

#endif // MAIN_GAME_H
