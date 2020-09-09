#ifndef CHANGE_MAP_BUTTON_H
#define CHANGE_MAP_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QPixmap>
#include <QtMultimedia/QSound>

class change_map_button : public QPushButton
{
    Q_OBJECT
public:
    explicit change_map_button(QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
signals:

};

#endif // CHANGE_MAP_BUTTON_H
