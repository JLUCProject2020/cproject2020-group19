#include "entity.h"
#include "mapgenerating.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QString>
#include <QDir>

using namespace std;

//区块大小宏定义
//#define CHUNK_WIDTH 16
//#define CHUNK_HEIGHT 100


//****************************************************************************
//                             实体函数

//碰撞函数,与地图方面对接
bool ENTITY::Collision_X1(MapChunk *MAP){
    MapChunk *map = MAP;                 //防止函数操作改变原有数据
    int i;
    //先定义边界点
    float sizepoint_x = Position_x - Size_x/2 + Velocity_x , sizepoint_z = Position_z - Size_z/2;
    //判断x是否跨区块
    if(sizepoint_x < 0){
        map = map->LeftChunk;
        sizepoint_x += 16;
    }else if (sizepoint_x >= 16) {
        map = map->RightChunk;
        sizepoint_x -= 16;
    }
    //如果实体的x左向方块不为空气（0号方块为空气方块）（判断实体x负向边界所涉及的所有方块）
    for(i = floor(sizepoint_z + 0.02);i <= (floor(sizepoint_z+Size_z-0.02));i++){
        if(map->Blocks[(int)(sizepoint_x)][i] != 0){
            return true;
        }

    }
    return false;

}

bool ENTITY::Collision_X2(MapChunk *MAP){
    MapChunk *map = MAP;                 //防止函数操作改变原有数据
    int i;
    //先定义边界点
    float sizepoint_x = Position_x + Size_x/2 + Velocity_x, sizepoint_z = Position_z - Size_z/2 ;
    //判断x是否跨区块
    if(sizepoint_x < 0){
        map = map->LeftChunk;
        sizepoint_x += 16;
    }else if (sizepoint_x >= 16) {
        map = map->RightChunk;
        sizepoint_x -= 16;
    }
    //如果实体的x右向方块不为空气（0号方块为空气方块）（判断实体x正向边界所涉及的所有方块）
    for(i = floor(sizepoint_z+0.02);i <= (floor(sizepoint_z+Size_z-0.02) );i++){
        if(map->Blocks[(int)(sizepoint_x)][i] != 0){
            return true;
        }

    }
    return false;
}

bool ENTITY::Collision_Z1(MapChunk *MAP){   //需要改动
    MapChunk *map;                 //防止函数操作改变原有数据
    int i, j;
    //先定义边界点
    float sizepoint_x = Position_x - Size_x/2 + Velocity_x, sizepoint_z = Position_z - Size_z/2 + Velocity_z;
    //判断z是否跨边界
    if(sizepoint_z < 0){
        return true;
    }else if(sizepoint_z > 100){
        return true;
    }

    //如果实体的z上方方块不为空气（0号方块为空气方块）（判断实体x正向边界所涉及的所有方块）
    for(i = floor(sizepoint_x + 0.02);i <= floor(sizepoint_x + Size_x - 0.02);i++){
        if(i < 0){
            map = MAP->LeftChunk;
            j = i + 16;
        }else if (i >= 16) {
            map = MAP->RightChunk;
            j = i - 16;
        }else{
            map = MAP;
            j = i;
        }
        if(map->Blocks[j][(int)(sizepoint_z)] != 0){
            return true;
        }

    }
    return false;
}

bool ENTITY::Collision_Z2(MapChunk *MAP){
    //qDebug() << "开始判断下方碰撞";
    MapChunk *map;                 //防止函数操作改变原有数据
    int i , j;
    int ini_x , final_x;           //下方方块判定的x值起点与终点
    //先定义边界点
    float sizepoint_x = Position_x - Size_x/2 + Velocity_x, sizepoint_z = Position_z + Size_z/2 + Velocity_z ;

    //判断z是否跨边界
    if(sizepoint_z < 0){
        return true;
    }else if(sizepoint_z > 100){
        return true;
    }

    //如果实体的z下方方块不为空气（0号方块为空气方块）（判断实体x正向边界所涉及的所有方块）
    for(i = floor(sizepoint_x + 0.02);i <= floor(sizepoint_x + Size_x - 0.02);i++){
        if(i < 0){
            map = MAP->LeftChunk;
            j = i + 16;
        }else if (i >= 16) {
            map = MAP->RightChunk;
            j = i - 16;
        }else{
            map = MAP;
            j = i;
        }

        //qDebug() << j << (int)(sizepoint_z) << map->Blocks[j][(int)(sizepoint_z)];
        if(map->Blocks[j][(int)(sizepoint_z)] != 0){
            return true;
        }

    }
    return false;
}

//*****************************************************************************
//                             玩家函数

//创建文件路径
void PLAYER::CreatPath(QString worldname){

    //将玩家编号转化为QString                            暂时不需要！！
    //this->SaveName.append(QString::number(Number));

    //先弄文件名
    SaveName = "/Player";
    //加上后缀
    SaveName.append(".txt");

    //现在生成路径
    SavePath = "../saves/" ;
    SavePath.append(worldname);
    SavePath.append(SaveName);

    qDebug() << SavePath;

}

//保存存档
bool PLAYER::SavePlayer(){
    //创建文件对象，同时为文件选择路径
    QFile file(SavePath);
    int i;

    bool is_Open = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(is_Open){
        //创建二进制读写流
        QDataStream stream(&file);
        //写入名字和名字长度
        stream << Player_Name;

        //写入其余属性
        stream << Player_Num << HitPoints << Max_HitPoints;
        stream << Player_Energy;
        for(i=0;i<15;i++){
            stream << Player_Backpack[i].Item_Num << Player_Backpack[i].Quantity << Player_Backpack[i].Item_type;
        }
        for(i=0;i<5;i++){
            stream << Equip[i].Item_Num << Equip[i].Quantity;
        }
        file.close();
        return true;
    }else {
        qDebug() << "文件创建失败";
        return false;
    }

}

//读取玩家存档
bool PLAYER::LoadPlayer(QString worldname){
    //先创建路径
    CreatPath(worldname);

    //创建文件对象，同时为文件选择路径
    QFile file(SavePath);
    int i;

    bool is_Open = file.open(QIODevice::ReadOnly);
    if(is_Open){
        //创建二进制读写流
        QDataStream stream(&file);
        //读名字
        stream >> Player_Name;

        //读入其余属性
        stream >> Player_Num >> HitPoints >> Max_HitPoints;
        stream >> Player_Energy;
        for(i=0;i<15;i++){
            stream >> Player_Backpack[i].Item_Num >> Player_Backpack[i].Quantity >> Player_Backpack[i].Item_type;
        }
        for(i=0;i<5;i++){
            stream >> Equip[i].Item_Num >> Equip[i].Quantity;
        }
        file.close();
        return true;
    }else {
        qDebug() << "玩家读取失败";
        return false;
    }

}

//判断方块是否在玩家碰撞箱内
bool PLAYER::isinbox(int x, int z, int chunk){
    int size_x1 = floor(Position_x - Size_x/2);
    int size_x2 = floor(Position_x + Size_x/2);
    int size_z1 = floor(Position_z - Size_z/2);
    int size_z2 = floor(Position_z + Size_z/2);
    //qDebug() << "x" << size_x1 << size_x2 << "z" <<  size_z1 << size_z2 << "nouse" << x << z ;
    //先检测区块
    if((size_x1 >= 0 && chunk == (Chunk_x - 1))||(size_x2 <= 16 && chunk == (Chunk_x + 1))){
        return false;
    }
    //检测z方向
    if(z < size_z1 || z > size_z2){
        return false;
    }
    //判断玩家在区块边界情况
    if(((size_x1 < 0)&&(chunk == (Chunk_x - 1))) || (size_x2 >= 16 && chunk == (Chunk_x + 1)) ){
        if((size_x1 < 0)&&(chunk == (Chunk_x - 1))){   //区块左边界
            if(x >= size_x1){
                //qDebug() << "case 1 t";
                return true;

            }else{
                //qDebug() << "case 1 f";
                return false;
            }
        }else{   //区块右边界
            if(x <= size_x2){
                //qDebug() << "case 2 t";
                return true;
            }else{
                //qDebug() << "case 2 f";
                return false;
            }
        }
    }
    //判断正常情况
    if(chunk == Chunk_x){
        if(x < size_x1 || x > size_x2){
            //qDebug() << "case n f";
            return false;
        }else{
            //qDebug() << "case n t";
            return true;
        }
    }
    qDebug() << "isinbox错误！！";
    return false;
}

//*****************************************************************************
//                             生物体函数

//生物体移动控制
void CREATURE::Move_GroundWalk(short Direction_x, bool Jump){
    Accelerate_x = Direction_x * Accelerate - Velocity_x * Beta;
    Accelerate_z = Gravity - Velocity_z * Z_Bata;
    Velocity_x = Velocity_x + Accelerate_x;
    Velocity_z = Velocity_z + Accelerate_z;
    if(Collision_X1(pId)/* 如果x负向有障碍 */){
        //重置玩家x坐标
        //Position_x = (int)(Position_x) + Size_x/2;
        //将玩家x负向速度、加速度清零

        if(Velocity_x < 0){
            Velocity_x = 0;
        }
        if(Accelerate_x < 0){
            Accelerate_x = 0;
        }
    }
    if(Collision_X2(pId)/* 如果x正向有障碍 */){
        //重置玩家x坐标
        //Position_x = (int)(Position_x + Size_x/2 + Velocity_x) - Size_x/2;
        //将玩家x正向速度、加速度清零
        if(Velocity_x > 0){
             Velocity_x = 0;
        }
        if(Accelerate_x > 0){
             Accelerate_x = 0;
        }
    }

    if(Collision_Z1(pId)/* 如果z负向有障碍 */){
        //重置玩家z坐标
        if(Position_z > 100){
            Position_z = 100;
        }else if(Position_z < 0){
            Position_z = 0;
        }

        //先将玩家z负向速度、加速度清零
        if(Velocity_z < 0){
             Velocity_z = 0;
        }
        if(Accelerate_z < 0){
             Accelerate_z = 0;
        }

    }

    if(Collision_Z2(pId)/* 如果z正向有障碍 */){
        //重置玩家z坐标
        //Position_z = (int)(Position_z + Size_z/2 + Velocity_z) - Size_z/2;
        //将玩家z正向速度、加速度清零
        if(Velocity_z > 0){
             Velocity_z = 0;
        }
        if(Accelerate_z > 0){
             Accelerate_z = 0;
        }
        //再判断是否进行跳跃(只有在地上才能跳跃)
        if(Jump){
            Velocity_z = - Jump_Velocity;
        }
    }
    else{
        //如果各个方向均无障碍
    }


    Position_x = Position_x + Velocity_x;
    Position_z = Position_z + Velocity_z;

    //检查实体是否超过区块
    if( Position_x > CHUNK_WIDTH ){
        //区块++
        Chunk_x++;
        //区块指针右移
        //pId = pId->RightChunk;
        //坐标转化
        Position_x = Position_x - CHUNK_WIDTH;
        Block_Through = 1;

    }
    else{
        if( Position_x < 0 ){
            //区块--
            Chunk_x--;
            //区块指针右移
            //pId = pId->LeftChunk;
            //坐标转化
            Position_x = Position_x + CHUNK_WIDTH;
            Block_Through = -1;
        }
    }


}

//*****************************************************************************
//                             特殊物体函数
void OBJECT::Shoot(float Sin, float Cos){
    Velocity_x = Max_Speed * Sin;
    Velocity_z = Max_Speed * Cos;
}

void OBJECT::Flying(){
    //先处理加速，再处理速度，再处理位置
    Accelerate_x = 0 - Velocity_x * Beta;
    Accelerate_z = Object_Gravity - Velocity_z * Beta;
    Velocity_x = Velocity_x + Accelerate_x;
    Velocity_z = Velocity_z + Accelerate_z;
    //判断碰撞，碰撞后销毁
    if(Collision_X1(pId) || Collision_X2(pId) || Collision_Z1(pId) || Collision_Z2(pId)){
        Destroy = true;
    }

    Position_x = Position_x + Velocity_x;
    Position_z = Position_z + Velocity_z;

    //检查是否超过区块
    if( Position_x > CHUNK_WIDTH ){
        Chunk_x++;
        Position_x = Position_x - CHUNK_WIDTH;
        Block_Through = 1;
    }
    else{
        if( Position_x < 0 ){
            Chunk_x--;
            Position_x = Position_x + CHUNK_WIDTH;
            Block_Through = -1;
        }
    }

}

//==============================加载物品============================

void Load_Item(Container *current_item, int number, int quantity){
    current_item->Item_Num = number;
    current_item->Quantity = quantity;
    current_item->Item_type = item[number].Item_type;
}

//=================================================================

//*****************************************************************************
//                             外部函数
//元数据读写



//bool LoadItem(int Num, ITEM *item){
//    //合成路径路径串
//    QString savepath = "../Data/Items/";
//    QString savename = "Item_";
//    savename.append(QString::number(Num));
//    savename.append(".txt");
//    savepath.append(savename);

//    //文件操作
//    QFile f(savepath);
//    bool Is_Open = f.open(QIODevice::ReadOnly);
//    if(!Is_Open){
//        qDebug() << "读取失败" ;
//        return false;
//    }
//    else{
//        //创建二进制读写流
//        QDataStream stream(&f);
//        //读取
//        stream >> item->Item_Num >> item->Type >> item->Item_Name >> item->Detail
//               >> item->Add_attack >> item->Add_defance >> item->Add_recover >> item->Add_Energy_recover;
//        return true;
//    }


//}

//void SaveItem(ITEM *item){
//    QString savepath = "../Data/Items/";
//    QString savename = "Item_";
//    //文件夹操作
//    QDir d("../");
//    if(!d.exists("Data")){
//        qDebug() << "无Data，开始创建" ;
//        d.mkdir("Data");
//    }
//    d.cd("Data");
//    if(!d.exists("Items")){
//        qDebug() << "无Items，创建文件夹" ;
//        d.mkdir("Items");
//    }
//    d.cd("Items");

//    //生成路径
//    savename.append(QString::number(item->Item_Num));
//    savename.append(".txt");
//    savepath.append(savename);

//    //文件操作
//    QFile f(savepath);
//    bool Is_Open = f.open(QIODevice::WriteOnly | QIODevice::Truncate);

//    if(Is_Open){
//        //创建二进制读写流
//        QDataStream stream(&f);
//        //写入
//        stream << item->Item_Num << item->Type << item->Item_Name << item->Detail
//               <<item->Add_attack << item->Add_defance << item->Add_recover << item->Add_Energy_recover;
//        qDebug() << "文件创建成功";
//    }else {
//        qDebug() << "文件创建失败";
//        return;
//    }


//}


