//#ifndef GAMETHREAD_H
//#define GAMETHREAD_H

//#include <QObject>
//#include <QThread>

//#include "mapgenerating.h"
//#include "entity.h"
//#include "MapInfo.h"

//class GameThread : public QThread
//{
//    Q_OBJECT
//public:
//    explicit GameThread(QObject *parent = nullptr);
//    void iniThread(GenerateMap *map,PLAYER *player);
//    void setKeyStatus(bool a_status,bool d_status,bool space_status);
//    GenerateMap *MAP = NULL;
//    PLAYER *Player = NULL;
//    bool a_status;
//    bool d_status;
//    bool space_status;
//    int i = 0;

//    bool execute = true;
//protected:
//    void run();

//signals:
//    void Finished();
//    void Begin();

//};

//#endif // GAMETHREAD_H
