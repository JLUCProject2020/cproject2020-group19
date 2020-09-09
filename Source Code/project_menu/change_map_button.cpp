#include "change_map_button.h"
#include <QMediaPlayer>

change_map_button::change_map_button(QWidget *parent) : QPushButton(parent)
{

}

void change_map_button::enterEvent(QEvent *){
    //QSound::play("../res/audios/buttonsound.wav");
    this->resize(this->width() + 10 , this->height() + 10 );
    this->move(this->x() - 5 , this->y() - 5 );
}

void change_map_button::leaveEvent(QEvent *){
    this->resize(this->width() - 10 , this->height() - 10 );
    this->move(this->x() + 5 , this->y() + 5 );
}
