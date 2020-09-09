#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H
#include <QPushButton>
#include <QMouseEvent>
#include <QPixmap>
#include <QtMultimedia/QSound>


class Menu_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Menu_Button(QWidget *parent = nullptr);
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
//    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
signals:

};

#endif // MENU_BUTTON_H
