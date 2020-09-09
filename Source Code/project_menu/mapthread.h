#ifndef MAPTHREAD_H
#define MAPTHREAD_H

#include "entity.h"
#include "perlin_noise.h"
#include "mapgenerating.h"
#include "widget.h"
#include "MapInfo.h"

#include <QObject>
#include <QThread>
#include <QString>

class MapThread : public QThread
{
    Q_OBJECT
public:
    explicit MapThread(QObject *parent = nullptr);
    //传入私有属性的函数
    void getexistance(bool e);
    PLAYER *player = NULL;
    GenerateMap *map = NULL;
protected:
    void run();
private:
    bool ifchunkexists;

signals:
    void Finished();

};

#endif // MAPTHREAD_H
