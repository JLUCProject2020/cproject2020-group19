#include "mapthread.h"

MapThread::MapThread(QObject *parent) : QThread(parent)
{

}

void MapThread::run(){
    if (map == NULL) {
        qDebug() << "没有地图指针";
        return;
    }
    if(player == NULL){
        qDebug() << "没有玩家指针";
        return;
    }
    map->initMap(player->Chunk_x,&(player->pId),ifchunkexists);
    map->SaveChunks(&(player->pId));
    qDebug() << "thread生成世界";
    emit Finished();

}

void MapThread::getexistance(bool e){
    ifchunkexists = e;
}
