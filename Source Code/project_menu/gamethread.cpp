//#include "gamethread.h"
//#include <QDateTime>

//GameThread::GameThread(QObject *parent) : QThread(parent)
//{

//}

//void GameThread::run(){
//    while (execute) {
//        emit Begin();
//        qDebug() << "开始计算" << i << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd");
//        if(((a_status == false)&&(d_status == false))||(a_status && d_status)){
//            Player->Move_GroundWalk(0, space_status);

//        }
//        else{
//            if(a_status || d_status){
//                //qDebug() << "走";
//                if(a_status){
//                    Player->Move_GroundWalk(-1, space_status);
//                    Player->Facing = Back;
////                    delete player_m;
////                    player_m = new QMovie(PlayerWL_Path);
////                    ui->astronaut_player->setMovie(player_m);
////                    player_m->start();

//                }
//                else{
//                    Player->Move_GroundWalk(1, space_status);
//                    Player->Facing = Front;
////                    delete player_m;
////                    player_m = new QMovie(PlayerWR_Path);
////                    ui->astronaut_player->setMovie(player_m);
////                    player_m->start();

//                }
//            }
//        }
//        //判断是否穿过区块
//        if(Player->Block_Through != 0){
//            MAP->geneNewChunk(&(Player->pId),Player->Block_Through);
//            Player->Block_Through = 0;
////            qDebug() << "-2区块" << current_player->pId->LeftChunk->LeftChunk <<
////                        "-1区块" << current_player->pId->LeftChunk <<
////                        "0区块" << current_player->pId <<
////                        "1区块" << current_player->pId->RightChunk <<
////                        "2区块" << current_player->pId->RightChunk->RightChunk;
////            qDebug() << current_player->Chunk_x << current_player->pId->ChunkId;
//        }
//        emit Finished();
//        qDebug() << "开始绘图" << i << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd");
//        QThread::msleep(10);
//        i++;
//    }
//}

//void GameThread::setKeyStatus(bool A_status, bool D_status, bool Space_status){
//    a_status = A_status;
//    d_status = D_status;
//    space_status = Space_status;
//}

//void GameThread::iniThread(GenerateMap *map, PLAYER *player){
//    MAP = map;
//    Player = player;
//}
