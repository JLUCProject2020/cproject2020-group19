#ifndef BOX_H
#define BOX_H

#include <QDebug>
#include <iostream>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include "entity.h"


//箱子放置
QString Creat_Box_Path(QString worldname,QString boxname);

bool Box_Close(QString BOX_PATH,Container *box);

QString Creat_Box_Name(int x,int y,int n);

bool Box_Open(int x,int y,int n,QString worldname,QString *boxpath, Container *box);

bool box_destroy(QString BOX_PATH, Container *BOX, PLAYER *player);
#endif // BOX_H
