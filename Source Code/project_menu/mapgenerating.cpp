#include "mapgenerating.h"
#include "perlin_noise.h"
#include "widget.h"

using namespace std;

//我可能需要一个类专门表示玩家数据
//以后需要根据每个不同的次元，进入不同的世界
//即mapid不同，调用不同的世界的构造函数
//这里的世界指的是次元
GenerateMap::GenerateMap(short int mapid,QString Name,long int SEED)
{
    MapId = mapid;
    WorldName= Name;
    seed = SEED;

    QString path = SAVES_PATH;//以后在这里修改来加入其它次元 according to Mapid
    DIM0_p = path; DIM0_p.append("/").append(WorldName).append("/DIM0").append("/chunkp.dat");
    DIM0_n = path; DIM0_n.append("/").append(WorldName).append("/DIM0").append("/chunkn.dat");


}//赋给地图标号

GenerateMap::~GenerateMap()
{
    //在玩家那边储存地图
}


int GenerateMap::getMapId()
{
    return MapId;
}   //返回一个地图标号

QString GenerateMap::getWorldName()
{
    return WorldName;
}


void GenerateMap::geneNewChunk(MapChunk ** pId,short int direction)
{
    //pId为玩家所在区块的二次指针，为了减少运算，在区块id变更的时候调用该函数
    //Left:-1,Right:1
    //新的区块被创建的时候，就应该存一次档
    if (direction == 1)
    {
        //如果是向右进入的区块,我们保证每个区块的左右都是加载的状态
        SaveChunkandKill(&((*pId)->LeftChunk->LeftChunk));\
        //我们右进入区块，需要保存最左边的活跃区块，并且清除
        (*pId)->LeftChunk->LeftChunk = NULL;
        //左临边的左边为空指针
        (*pId) = (*pId)->RightChunk;

        //进入了早已经加载的相邻右区块
        (*pId)->RightChunk->RightChunk = new(MapChunk);
        //给现在的这个区块一个新的右右区块

        MapChunk ** p = &((*pId)->RightChunk->RightChunk);//构造一个临时变量p，指向最右边的生成区块

        int Size = sizeof(MapChunk);

        //从存档里面读结构体数据，如果没有，就重新生成
        if ((*pId)->ChunkId + 2 >= 0)
        {

            qDebug() << (*pId)->ChunkId <<"R1"<<(*pId)->ChunkId + 1 <<"R2"<<(*pId)->ChunkId + 2 ;
            FILE* exchunk = fopen(DIM0_p.toStdString().c_str(), "rb"); //打开地图文件-P
            if ((*pId)->ChunkId + 2 >= getChunkNumP())
            {
                //即还没有创建这一个区块
                qDebug() << "新建区块";
                genePureChunk((*pId)->ChunkId + 2, p);
                (*p)->LeftChunk = (*pId)->RightChunk;
            }
            else
            {
                //否则已经有了这个区块
                qDebug() << "读档";
                ReadChunk((*pId)->ChunkId + 2, p);//读了区块
                (*p)->Ifactive = true;
                (*p)->LeftChunk = (*pId)->RightChunk;
            }
        }
        else
        {
            FILE* exchunk = fopen(DIM0_n.toStdString().c_str(), "rb"); //打开地图文件-N
            if (-((*pId)->ChunkId + 2) > getChunkNumN() )
            {
                //即还没有创建这一个区块
                genePureChunk((*pId)->ChunkId + 2, p);
                (*p)->LeftChunk = (*pId)->RightChunk;
            }
            else
            {
                //否则已经有了这个区块
                ReadChunk((*pId)->ChunkId + 2, p);//读了区块
                (*p)->Ifactive = true;
                (*p)->LeftChunk = (*pId)->RightChunk;
            }
        }
    }
    else if (direction == -1)
    {
        //如果是向左进入的区块,我们保证每个区块的左右都是加载的状态
        SaveChunkandKill(&((*pId)->RightChunk->RightChunk));//我们左进入区块，需要保存最右边的活跃区块，并且清除
        (*pId)->RightChunk->RightChunk = NULL;//右临边的右边为空指针
        (*pId) = (*pId)->LeftChunk;//进入了早已经加载的相邻左区块
        (*pId)->LeftChunk->LeftChunk = new(MapChunk);//给现在的这个区块一个新的左左区块

        MapChunk** p = &((*pId)->LeftChunk->LeftChunk);//构造一个临时变量p，指向最左边的生成区块

        int Size = sizeof(MapChunk);

        //从存档里面读结构体数据，如果没有，就重新生成
        if ((*pId)->ChunkId - 2 >= 0)
        {
            FILE* exchunk = fopen(DIM0_p.toStdString().c_str(), "rb"); //打开地图文件-P
            if ((*pId)->ChunkId - 2 >= getChunkNumP())
            {
                //即还没有创建这一个区块
                genePureChunk((*pId)->ChunkId - 2, p);
                (*p)->RightChunk = (*pId)->LeftChunk;

            }
            else
            {
                //否则已经有了这个区块
                ReadChunk((*pId)->ChunkId - 2, p);//读了区块
                (*p)->Ifactive = true;
                (*p)->RightChunk = (*pId)->LeftChunk;
            }
        }
        else
        {
            //小于0的存档
            FILE * exchunk = fopen(DIM0_n.toStdString().c_str(), "rb"); //打开地图文件-N
            if (-((*pId)->ChunkId - 2) > getChunkNumN())
            {
                //即还没有创建这一个区块
                genePureChunk((*pId)->ChunkId - 2, p);
                (*p)->RightChunk = (*pId)->LeftChunk;

            }
            else
            {
                //否则已经有了这个区块
                ReadChunk((*pId)->ChunkId - 2, p);//读了区块
                (*p)->Ifactive = true;
                (*p)->RightChunk = (*pId)->LeftChunk;
            }
        }
    }
    //SaveChunks(pId);
}

void GenerateMap::ReadChunk(int ChunkId,MapChunk ** pId)//默认了有这个区块了哈
{
    //这个只需要赋值数据就可以，先弄单地图其实应该考虑mapid
    //这里应该有一个switch mapid
    //先根据id找文件，然后
    //这里默认了只有一个次元
    if (ChunkId >= 0)
    {
        int Size = sizeof(MapChunk);
        FILE * readchunk = fopen(DIM0_p.toStdString().c_str(),"rb"); //打开了次元的文件
        fseek(readchunk, Size * ChunkId, SEEK_SET);  //移动到第ChunkId个区块
        fread(*pId, Size, 1, readchunk);  //读取存到了传入的位置
        //重新设置指针,我们已经有存储的地图指针都在存储前被设置为空
        fclose(readchunk);
    }
    else
    {
        int Size = sizeof(MapChunk);
        FILE* readchunk = fopen(DIM0_n.toStdString().c_str(), "rb");
        fseek(readchunk, Size * (-1 - ChunkId), SEEK_SET);
        //由于chunkid是负的，一般从-1开始，所以如此变换
        fread(*pId, Size, 1, readchunk);
        //重新设置指针
        fclose(readchunk);
    }
}

void GenerateMap::SaveChunk(MapChunk ** savePtr)
{

    int Size = sizeof(MapChunk);


    if ((*savePtr)->ChunkId >= 0)
    {
        FILE* savechunk = fopen(DIM0_p.toStdString().c_str(), "rb+"); //打开地图文件-P
        if ( (*savePtr)->ChunkId >= getChunkNumP() )
        {
            qDebug() <<"欲存id" << ((*savePtr)->ChunkId) ;
            qDebug() <<"正区块数" << getChunkNumP() ;
            //即还没有创建这一个区块
            fseek(savechunk, 0, SEEK_END);//指针指向最后，开始写新的区块存档
            fwrite(*savePtr, Size, 1, savechunk);
            fclose(savechunk);
        }
        else
        {
            qDebug() <<"欲存id'" << ((*savePtr)->ChunkId) ;
            qDebug() <<"正区块数'" << getChunkNumP() ;
            fseek(savechunk, Size * ((*savePtr)->ChunkId), SEEK_SET);//找到存储的位置
            fwrite(*savePtr, Size, 1, savechunk);//替换中间的某一段
            fclose(savechunk);
        }
    }
    else
    {
        //id小于0
        FILE* savechunk = fopen(DIM0_n.toStdString().c_str(), "rb+"); //打开地图文件-N
        if ( (-(*savePtr)->ChunkId) > getChunkNumN() )
        {
            qDebug() <<"欲存id" << ((*savePtr)->ChunkId) ;
            qDebug() <<"N区块数" << getChunkNumN() ;
            //即还没有创建这一个区块
            fseek(savechunk, 0, SEEK_END);//指针指向最后，开始写新的区块存档
            fwrite(*savePtr, Size, 1, savechunk);
            fclose(savechunk);
        }
        else
        {
            qDebug() <<"欲存id" << ((*savePtr)->ChunkId) ;
            qDebug() <<"N区块数" << getChunkNumN() ;
            fseek(savechunk, Size * (-1 - (*savePtr)->ChunkId), SEEK_SET);//找到存储的位置
            fwrite(*savePtr, Size, 1, savechunk);//替换中间的某一段
            fclose(savechunk);
        }
    }
}

void GenerateMap::SaveChunkandKill(MapChunk ** savePtr)
{
    //这里应该switch 一个 mapid 先弄单世界
    //首先查看地图的id 储存的时候把指针搞掉！！！
    (*savePtr)->LeftChunk = NULL;
    (*savePtr)->RightChunk = NULL;
    (*savePtr)->Ifactive = false;
    SaveChunk(savePtr);
    delete (*savePtr);

}

void GenerateMap::initMap(int PlayerChunkId,MapChunk ** pId,bool ifChunkExist)
{
    QString dim_path = SAVES_PATH;
    ReadSeed();
    qDebug() << seed;
    dim_path.append("/").append(WorldName).append("/DIM0");
    QDir dir(dim_path);
    qDebug()<<dim_path;

    //先在文件夹的地图中检索要初始化哪个地图(待写)，如果没有则新建
    if (0 != access(dim_path.toStdString().c_str(), 0))
    {
        //如果不存在该文件夹，就创建一个
        mkdir(dim_path.toStdString().c_str());
        //这里需要创建两个二进制文件，给赋值ifChunkExist
    }
    //进入新的次元也用这个
    //检查文件是否存在
    if(dir.exists("chunkp.dat") == false){
        QString Edit_path = dim_path;
        QFile f(Edit_path.append("/chunkp.dat"));
        f.open(QIODevice::ReadWrite);
        f.close();
    }
    if(dir.exists("chunkn.dat") == false){
        QString Edit_path = dim_path;
        QFile f(Edit_path.append("/chunkn.dat"));
        f.open(QIODevice::ReadWrite);
        f.close();
    }
    //bool ifChunkExist = true;

    //这里通过一个函数判断是否有过，这里加一个函数判定玩家所在区块是否有
    //

    if (ifChunkExist)
    {
        //如果次元有，现在需要读取5个区块，默认左右各两个都有
        (*pId) = new(MapChunk);
        ReadChunk(PlayerChunkId, pId);
        {
            //注意读取进来的指针要重置
            (*pId)->LeftChunk = new(MapChunk);
            (*pId)->RightChunk = new(MapChunk);
        }
        ReadChunk(PlayerChunkId + 1, &((*pId)->RightChunk));
        {
            (*pId)->RightChunk->LeftChunk = (*pId);
            (*pId)->RightChunk->RightChunk = new(MapChunk);
        }
        ReadChunk(PlayerChunkId - 1, &((*pId)->LeftChunk));
        {
            (*pId)->LeftChunk->RightChunk = (*pId);
            (*pId)->LeftChunk->LeftChunk = new(MapChunk);
        }
        ReadChunk(PlayerChunkId + 2, &((*pId)->RightChunk->RightChunk));
        {
            (*pId)->RightChunk->RightChunk->LeftChunk = (*pId)->RightChunk;
        }
        ReadChunk(PlayerChunkId - 2, &((*pId)->LeftChunk->LeftChunk));
        {
            (*pId)->LeftChunk->LeftChunk->RightChunk = (*pId)->LeftChunk;
        }

        SaveChunks(pId);

    }
    else
    {

        (*pId) = new(MapChunk);
        genePureChunk(0, pId);
        (*pId)->LeftChunk = new(MapChunk);
        {
            genePureChunk(-1, &((*pId)->LeftChunk));
            (*pId)->LeftChunk->RightChunk = (*pId);
            (*pId)->LeftChunk->LeftChunk = new(MapChunk);
        }
        (*pId)->RightChunk = new(MapChunk);
        {
            genePureChunk(1, &((*pId)->RightChunk));
            (*pId)->RightChunk->LeftChunk = (*pId);
            (*pId)->RightChunk->RightChunk = new(MapChunk);
        }
        {
            genePureChunk(2, &((*pId)->RightChunk->RightChunk));
            (*pId)->RightChunk->RightChunk->LeftChunk = (*pId)->RightChunk;
        }
        {
            genePureChunk(-2, &((*pId)->LeftChunk->LeftChunk));
            (*pId)->LeftChunk->LeftChunk->RightChunk = (*pId)->LeftChunk;
        }

        SaveChunks(pId);
        //记得抽离出去单纯的区块构造函数----------和全部保存函数--------------------------------------//
    }
    //new一个struct作为目前的玩家所在区块，并且生成或者读取周围两端
    //我需要一个玩家的指针指向所在区块  也许

}

void GenerateMap::genePureChunk(int ChunkId,MapChunk ** pId)
{

    (*pId)->Ifactive = true;
    (*pId)->Ifexplored = true;
    (*pId)->ChunkId = ChunkId;

    qDebug()<<"新建纯区块"<<ChunkId<<endl;

    (*pId)->LeftChunk = NULL;
    (*pId)->RightChunk = NULL;
    {
        int i, j;
        PerlinNoise m;
        m.SetSeed(seed);
        for (i = 0; i < CHUNK_WIDTH; i++)
        {
            for (j = 0; j < CHUNK_HEIGHT; j++)
            {

                //写柏林函数
                (*pId)->Blocks[i][j] = m.getBlockId(ChunkId,i,j);
                //qDebug() << "生成方块：" << (*pId)->Blocks[i][j];
            }
        }
    }
    GenerateMine(ChunkId,pId);
    GenerateBuildings(ChunkId,pId);
}

void GenerateMap::SaveChunks(MapChunk ** pId)//单纯保存周围的区块
{
    SaveChunk(pId);
    SaveChunk(&((*pId)->LeftChunk));
    SaveChunk(&((*pId)->RightChunk));
    SaveChunk(&((*pId)->LeftChunk->LeftChunk));
    SaveChunk(&((*pId)->RightChunk->RightChunk));
    qDebug() << " 储存成功";
}


void GenerateMap::GenerateMine(int ChunkId,MapChunk ** pId)
{
    int i,j;

    for(j = CHUNK_BASE - 6;j < CHUNK_BASE - 4;j++)
    {
        for(i=0;i<CHUNK_WIDTH;i++)
        {
            if((*pId)->Blocks[i][j]!=0)
            {
                if((ChunkId+i*j+j*j*j)%13==0)
                {
                    //陨落铁矿生成
                    (*pId)->Blocks[i][j]=25;
                }


            }
        }
    }
    for(i=0;i<CHUNK_WIDTH;i++)
    {
        if((*pId)->Blocks[i][CHUNK_HEIGHT-4-5]==0 && ((*pId)->Blocks[i][CHUNK_HEIGHT-3-5]!=0) && ((7*i*i+i*seed)%17 == 3))
        {
            //水晶生成
            (*pId)->Blocks[i][CHUNK_HEIGHT-4-5]=2;
        }
    }


}

void GenerateMap::GenerateBuildings(int ChunkId,MapChunk ** pId)
{
    //  遗迹一号生成     遗迹二号生成     遗迹三号生成
    //      *
    //    *   *         * *               *
    //    *   *         * *             * * *
    //
    int i,j;

    if(abs(ChunkId)%17==1)
    {
        i=abs(ChunkId)%7+5;
        j=0;
        while((*pId)->Blocks[i][j]==0) j++;
        //定位到左下角  铁块
        (*pId)->Blocks[i][j] = 26;
        (*pId)->Blocks[i][j-1] = 26;
        (*pId)->Blocks[i+1][j-2] = 26;
        (*pId)->Blocks[i+2][j] = 26;
        (*pId)->Blocks[i+2][j-1] = 26;
    }
    if(abs(ChunkId)%17==9)
    {
        i=abs(ChunkId)%7+5;
        j=0;
        while((*pId)->Blocks[i][j]==0) j++;
        //定位到左下角  铁块
        (*pId)->Blocks[i][j] = 26;
        (*pId)->Blocks[i][j-1] = 26;
        (*pId)->Blocks[i+1][j] = 26;
        (*pId)->Blocks[i+1][j-1] = 26;
    }
    if(abs(ChunkId)%17==15)
    {
        i=abs(ChunkId)%7+5;
        j=0;
        while((*pId)->Blocks[i][j]==0) j++;
        //定位到左下角  铁块
        (*pId)->Blocks[i][j] = 26;
        (*pId)->Blocks[i+1][j] = 26;
        (*pId)->Blocks[i+2][j] = 26;
        (*pId)->Blocks[i+1][j-1] = 26;
    }
}



//======================多人游戏=============================//
extern short int PLAYERNUM = 1 ;

void GenerateMap::FixChunk(class PLAYER * PlayerPointer[4])
{
    //先给一个指针数组
    MapChunk ** FIX[4];
    int i,j;
    for (i = 0; i < PLAYERNUM; i++)
    {
        //FIX[i] = &(PlayerPointer[i]->pId);
        //给赋值
    }
    //根据CHUNK_X或CHUNK_ID给排序，使用冒泡排序
    MapChunk ** temp;
    for (i = 0; i < PLAYERNUM -1;i++)
    {
        for (j = 0; j < PLAYERNUM - i-1; j++)
        {
            if (((*FIX[j])->ChunkId) > ((*FIX[j+1])->ChunkId))
            {
                temp = FIX[j];
                FIX[j] = FIX[j + 1];
                FIX[j + 1] = temp;
            }
        }
    }
    //现在FIX数组中的chunkid升序

    for (i = 1; i < PLAYERNUM; i++)
    {
        if (((*FIX[i])->ChunkId - (*FIX[i - 1])->ChunkId) <= 4)
        {
            switch ((*FIX[i])->ChunkId- (*FIX[i-1])->ChunkId)
            {
                case 0:
                {
                    //重合
                    if ((*FIX[i]) != (*FIX[i-1]))//如果指向的不是一个内存中的区块
                    {
                        delete ((*FIX[i])->LeftChunk->LeftChunk);
                        delete ((*FIX[i])->LeftChunk);
                        delete ((*FIX[i])->RightChunk);
                        delete ((*FIX[i])->RightChunk->RightChunk);
                        //释放假区块
                        (*FIX[i]) = (*FIX[i-1 ]);
                    }

                    break;
                }
                case 1:
                {
                    //右一
                    if ((*FIX[i]) != ((*FIX[i - 1])->RightChunk))
                    {
                        delete ((*FIX[i])->LeftChunk->LeftChunk);
                        delete ((*FIX[i])->LeftChunk);
                        delete ((*FIX[i])->RightChunk);
                        delete ((*FIX[i])->RightChunk->RightChunk);

                        (*FIX[i]) = (*FIX[i - 1])->RightChunk;
                    }

                    break;
                }
                case 2:
                {
                    if ((*FIX[i]) != ((*FIX[i - 1])->RightChunk->RightChunk))
                    {
                        delete ((*FIX[i])->LeftChunk->LeftChunk);
                        delete ((*FIX[i])->LeftChunk);
                        delete ((*FIX[i])->RightChunk);
                        delete ((*FIX[i])->RightChunk->RightChunk);

                        (*FIX[i]) = (*FIX[i - 1])->RightChunk->RightChunk;
                    }

                    break;
                }
                case 3:
                {
                    if ((*FIX[i])->LeftChunk != (*FIX[i - 1])->RightChunk->RightChunk)
                    {
                        delete ((*FIX[i])->LeftChunk->LeftChunk);
                        delete ((*FIX[i])->LeftChunk);
                        delete ((*FIX[i])->RightChunk);
                        delete ((*FIX[i])->RightChunk->RightChunk);

                        (*FIX[i])->LeftChunk = (*FIX[i - 1])->RightChunk->RightChunk;
                    }


                    break;
                }
                case 4:
                {
                    if ((*FIX[i])->LeftChunk->LeftChunk != (*FIX[i - 1])->RightChunk->RightChunk)
                    {
                        delete ((*FIX[i])->LeftChunk->LeftChunk);
                        delete ((*FIX[i])->LeftChunk);
                        delete ((*FIX[i])->RightChunk);
                        delete ((*FIX[i])->RightChunk->RightChunk);

                        (*FIX[i])->LeftChunk->LeftChunk = (*FIX[i - 1])->RightChunk->RightChunk;
                    }
                    break;
                }
            }
        }
    }
}


int GenerateMap::getChunkNumP()
{
    QFile file(DIM0_p);
    return file.size()/sizeof(MapChunk);
}

int GenerateMap::getChunkNumN()
{
    QFile file(DIM0_n);
    return file.size()/sizeof(MapChunk);
}

void GenerateMap::ReadSeed(){
    QString worldpath = SAVES_PATH;
    worldpath.append("/").append(WorldName).append("/WorldInfo.dat");
    qDebug()<<worldpath<<endl;
    FILE *p = fopen(worldpath.toStdString().c_str(),"rb");
    int Size = sizeof(long int);

    fread(&seed,Size,1,p);

    fclose(p);
}






//=====================旧代码，已经抽离成函数============================================//

//if ((*savePtr)->ChunkId >= 0)
//{
//	FILE* savechunk = fopen(CHUNK_PATH_P, "wb"); //打开地图文件-P
//	if (fseek(savechunk, Size * ((*savePtr)->ChunkId)+1,SEEK_SET) != 0)
//	{
//		//即还没有创建这一个区块
//		fseek(savechunk, 0, SEEK_END);//指针指向最后，开始写新的区块存档
//		fwrite(*savePtr, Size, 1, savechunk);
//		delete(*savePtr);
//		fclose(savechunk);
//	}
//	else
//	{
//		fseek(savechunk, Size * ((*savePtr)->ChunkId), SEEK_SET);//找到存储的位置
//		fwrite(*savePtr, Size, 1, savechunk);//替换中间的某一段
//		delete(*savePtr);
//		fclose(savechunk);
//	}
//}
//else
//{
//	//id小于0
//	FILE* savechunk = fopen(CHUNK_PATH_N, "wb"); //打开地图文件-N
//	if (fseek(savechunk, Size * (-1-(*savePtr)->ChunkId)+1, SEEK_SET) != 0)
//	{
//		//即还没有创建这一个区块
//		fseek(savechunk, 0, SEEK_END);//指针指向最后，开始写新的区块存档
//		fwrite(*savePtr, Size, 1, savechunk);
//		delete(*savePtr);
//		fclose(savechunk);
//	}
//	else
//	{
//		fseek(savechunk, Size * (-1-(*savePtr)->ChunkId), SEEK_SET);//找到存储的位置
//		fwrite(*savePtr, Size, 1, savechunk);//替换中间的某一段
//		delete(*savePtr);
//		fclose(savechunk);
//	}
//}


