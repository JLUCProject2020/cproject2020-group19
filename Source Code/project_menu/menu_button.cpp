#include "menu_button.h"

Menu_Button::Menu_Button(QWidget *parent) : QPushButton(parent)
{
//    QPixmap p("../res/images/button.png");
//    QIcon i(p);
//    this->setIcon(i);
//    this->setIconSize(QSize(this->width(),this->height()));
    //this->setStyleSheet("color:white");
    this->setStyleSheet("QPushButton{border-image: url(../res/images/11.png);color : white}");



}

void Menu_Button::enterEvent(QEvent *){
    //QSound q("../res/audios/buttonsound.wav",nullptr);


    this->resize(this->width() + 20 , this->height() + 5 );
    this->move(this->x() - 10 , this->y() - 2 );
}
void Menu_Button::leaveEvent(QEvent *){
    this->resize(this->width() - 20 , this->height() - 5 );
    this->move(this->x() + 10 , this->y() + 2 );
}
//void Menu_Button::mousePressEvent(QMouseEvent *){
//    this->resize(this->width() + 40 , this->height() + 10 );
//    this->move(this->x() - 20 , this->y() - 5 );
//}

//void Menu_Button::mouseReleaseEvent(QMouseEvent *){
//    this->resize(this->width() - 40 , this->height() - 10 );
//    this->move(this->x() + 20 , this->y() + 5 );
//}
