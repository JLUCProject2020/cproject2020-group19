#ifndef MAPGENERATING_H
#define MAPGENERATING_H

#include "entity.h"
#include "perlin_noise.h"
//这个类代表某个次元的地图的读取存储
//打算在存储地图的时候根据不同的次元储存不同的地图，留一个传送的函数
//区块暂时和玩家行为无关，加入玩家后，玩家信息中留一个区块信息
//读取存档时，根据玩家的位置先刷新出区块
//解决两个玩家靠近时，同一个区块被同时纳入的情况
//所以应该先检测区块是否已经生成了，再运算
//这样需要每个区块加一个bool值表示是否已经生成 生成后检测周围的是否已经生成
//然后讲已经生成的连接起来
//如果没有生成且被两个人同时刷新此区块会怎样？
//分先后顺序，给player的大小分顺序
//所以区块更新是玩家行为
//我需要一个对照表，把地图id翻译成地图名称 然后储存
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <QDir>
#include <QString>


#define CHUNK_WIDTH 16 //区块的宽度
#define CHUNK_HEIGHT 100 //区块纵向高度
#define CHUNK_BASE 50 //水平线基准
#define BUFFER_NUM 3 //预加载区块数量
#define ACTIVE_NUM 5 //活跃区块数量
#define SAVES_PATH "../saves"


using namespace std;


typedef struct MapChunk
{
    int ChunkId; //区块的id
    unsigned short int Blocks[CHUNK_WIDTH][CHUNK_HEIGHT];
    struct MapChunk* LeftChunk;
    struct MapChunk* RightChunk;
    bool Ifactive;  //区块是否已经在活跃区中
    bool Ifexplored;
} MapChunk;

//==================多人游戏声明====================//

//class PLAYER
//{
//    public:
//        MapChunk * PId;
//        int Chunk_x;
//};

//PLAYER * PlayerPointer[4];

extern short int PLAYERNUM;

//==================多人游戏声明====================//



class GenerateMap
{
    public:
        GenerateMap(short int mapid,QString WorldName,long int SEED);			//生成地图的时候就给赋值！
        ~GenerateMap();											//析构函数
        int getMapId();											//获取地图id
        QString getWorldName();
        void geneNewChunk(MapChunk ** pId,short int dirction);  //生成新的区块,由 Left:-1,Right:1 表示如何进入的新的区块
        void genePureChunk(int ChunkId,MapChunk** pId);			//生成区块，可以放入protected
        void ReadChunk(int ChunkId,MapChunk ** pId);			//读取某一个区块，可以放入protected
        void SaveChunk(MapChunk ** savePtr);					//保存某一个区块，不删除，可以放入protected
        void SaveChunkandKill(MapChunk ** savePtr);				//保存某一个区块,saveptr是要保存的区块的，可以放入protected
        void SaveChunks(MapChunk** pId);						//保存玩家附近5个区块，可以放入protected
        void initMap(int PlayerChunkId,MapChunk**pId, bool a);			//负责刚进入新地图时刷新地图
        void FixChunk(class PLAYER * PlayerPointer[4]);
        int getChunkNumP();
        int getChunkNumN();
        void ReadSeed();


    protected:
        void GenerateMine(int ChunkId,MapChunk ** pId);//矿物生成函数
        void GenerateBuildings(int ChunkId,MapChunk ** pId);
    private:
        long int seed;
        short int MapId;
        QString WorldName;
        QString DIM0_p;
        QString DIM0_n;
};


#endif // MAPGENERATING_H
