#ifndef ENTITY_H
#define ENTITY_H
#include "mapgenerating.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <QString>
#include <fstream>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

//******************************************************************
//                    数字宏定义:目前先弄个值，等后期测试时再具体改

#define Beta 0.05           //阻尼系数
#define Z_Bata 0.02         //跳跃阻尼
#define Gravity 0.005       //重力


//******************************************************************

//                     注意事项

//对于带有方向属性的变量命名：
//           1统一代表负向 2统一代表正向
//         如：Collision_X1代表x负向碰撞

//坐标规定:
//                区块的原点位于     最左上
//                正负方向         右正下正→↓（重力沿着坐标的正方向）
//                实体的坐标点      实体中心
//
//物品：
//                空物品序号为1000

//******************************************************************

//                      枚举定义

//                       实体
//实体种类
typedef enum ENTITY_TYPE { Players, Monster , NPC /* 以后可以加更多...... */} Entity_Type;

//是不是友好生物
typedef enum ENTITY_FRIEND { Friendly , Agressive } Entity_Friend;

//人物朝向
typedef enum FACE { Front /* 正x方向 */ , Back /* 负x方向 */} Face;

//                       物品
//物品种类
typedef enum ITEM_TYPE {
    Sword,
    Gun,
    Equip,
    Material,
    Cube /*可放置方块*/,
    Null /* 空物品 */
}Item_Type;

//******************************************************************
//                    结构体定义

//物品结构体
typedef struct ITEM
{
    int Item_Num;                  //物品编号
    QString Item_Name;             //物品名称
    QString Detail;                //物品描述
    bool put_cube;                 //判断是否为可放置方块
    int Add_attack;            //物品附加攻击（默认为0）
    int Add_defance;           //物品附加防御（默认为0）
    int Add_recover;           //物品附加生命恢复
    int Add_Energy_recover;    //物品附加能量恢复,正为恢复，负为消耗
    Item_Type Item_type;   //定义物品种类
}Item;


//容器结构体，用于背包、箱子、怪的掉落
typedef struct CONTAINER{
    int Quantity ;         //数量,默认值为0
    int Item_Num ;         //物品编号，1000代表空物品
    Item_Type Item_type;   //定义物品种类
}Container;


//实体结构体，用于存储各种实体初始数据，在使用时调用             掉落物在实体子类里面写
typedef struct ENTITY_STRUCT
{
    //实体序号
    int Entity_Num;
    //实体种类
    Entity_Type Type;
    //是不是友好生物 可不加
    //Entity_Friend Friend;
    //碰撞箱
    float Size_x , Size_z;
    //战斗属性
    int Max_HitPoints , Attack , Defance;
    float Attack_Range;         //攻击范围
    //移动能力
    float Accelerate;       //水平加速能力
    float Jump_Velocity;    //跳跃能力


}Entity_Struct;

//实体数据库

static Entity_Struct Entity_Data[10] = {
//    0号实体：          玩家
    {
        0, Players , 0.9 , 1.9 , 100 , 0 , 0 , 1.5 , 0.003 , 0.3   //这里暂时放一个用于存档测试，具体的后期调
    }
//    1号实体：
};

//物品数据库

static Item item[50]={
    {
        0,"","",false,0,0,0,0,Null
    },
    {
        1,"行星土","遍布于行星各地的棕红色块状物，没什么利用价值。",true,0,0,0,0,Cube
    },
    {
        2,"行星结晶","一种位于地底的矿石，蕴藏着巨大的能量。",false,0,0,0,0,Material
    },
    {
        3,"机床","你可以用它创造各种工具哟！",true,0,0,0,0,Cube
    },
    {
        4,"损坏机床","这是一个破损的机床。",false,0,0,0,0,Material
    },
    {
        5,"辐射能收集器","收集能量，创造未来！",true,0,0,0,0,Cube
    },
    {
        6,"自动熔炼机","快用它去熔炼你想要的金属吧！",true,0,0,0,0,Cube
    },
    {
        7,"箱子","你想在箱子里藏些什么宝贝呢？",true,0,0,0,0,Cube
    },
    {
        8,"芯片","机械智慧的结晶。",false,0,0,0,0,Material
    },
    {
        9,"零件","这些小玩意可是制造机械的必需品！",false,0,0,0,0,Material
    },
    {
        10,"陨铁锭","陨铁的精华所在。",false,0,0,0,0,Material
    },
    {
        11,"能源核心","",false,0,0,0,0,Material
    },
    {
        12,"激光枪","威力无穷！",false,0,0,0,0,Gun
    },
    {
        13,"激光剑","听说你能看到剑影？",false,0,0,0,0,Sword
    },
    {
        14,"铁棍","吃俺老孙一棒！",false,0,0,0,0,Sword
    },
    {
        15,"水晶剑","好剑！",false,0,0,0,0,Sword
    },
    {
        16,"水晶能爆枪","迎接来自水晶能量的轰击吧！",false,0,0,0,0,Gun
    },
    {
        17,"平台","可供穿过站立。",false,0,0,0,0,Cube
    },
    {
        18,"能量电池","快去补充能量吧！",false,0,0,0,0,Equip
    },
    {
        19,"终极电池","You've got infinity power!",false,0,0,0,0,Equip
    },
    {
        20,"强化护甲","Stronger!",false,0,0,0,0,Equip
    },
    {
        21,"机械手臂","强壮有力。",false,0,0,0,0,Equip
    },
    {
        22,"纳米机器人","纳米机器人准备就绪！",false,0,0,0,0,Equip
    },
    {
        23,"超频系统","longer，better，faster，stronger!",false,0,0,0,0,Equip
    },
    {
        24,"高能钻头","嗡嗡嗡~~~",false,0,0,0,0,Equip
    },
    {
        25,"陨铁矿","行星上的常见矿物",true,0,0,0,0,Cube
    },
    {
        26,"陨铁块","",true,0,0,0,0,Cube
    }
};

//加载物品
void Load_Item(Container *current_item, int number, int quantity);

//物品合成部分

//合成表结构体定义
typedef struct Synthesis_list{
    Container synthesis_item[4];
    int product;
}synthesis_list;

//合成表结构体数组（利用container结构体）
static synthesis_list Synthesis_List[50]={
    {
        {{1,4,Material},{1,8,Material},{10,9,Material},{10,10,Material}},3
    },
    {
        {{0,0,Material},{3,8,Material},{20,9,Material},{20,10,Material}},5
    },
    {
        {{1,2,Material},{1,8,Material},{10,9,Material},{10,10,Material}},6
    },
    {
        {{10,10,Material},{0,0,Material},{0,0,Material},{0,0,Material}},7
    },
    {
        {{1,11,Material},{5,9,Material},{5,10,Material},{0,0,Material}},12
    },
    {
        {{1,11,Material},{3,9,Material},{7,10,Material},{0,0,Material}},13
    },
    {
        {{10,10,Material},{0,0,Material},{0,0,Material},{0,0,Material}},14
    },
    {
        {{3,2,Material},{2,10,Material},{0,0,Material},{0,0,Material}},15
    },
    {
        {{3,5,Material},{5,9,Material},{5,10,Material},{0,0,Material}},16
    },
//    {
//        {{2,10,Material},{0,0,Material},{0,0,Material},{0,0,Material}},17
//    },
//    {
//        {{3,10,Material},{1,2,Material},{0,0,Material},{0,0,Material}},18
//    },
//    {
//        {{1,11,Material},{10,8,Material},{10,9,Material},{3,10,Material}},19
//    },
//    {
//        {{20,10,Material},{10,9,Material},{0,0,Material},{0,0,Material}},20
//    },
//    {
//        {{3,8,Material},{20,9,Material},{20,10,Material},{0,0,Material}},21
//    },
//    {
//        {{20,8,Material},{20,9,Material},{0,0,Material},{0,0,Material}},22
//    },
//    {
//        {{1,1,Material},{10,8,Material},{20,9,Material},{0,0,Material}},23
//    },
//    {
//        {{15,10,Material},{10,9,Material},{0,0,Material},{0,0,Material}},24
//    }
};
//定义一个材料数组在合成时方便使用
static CONTAINER material[5];





//******************************************************************

//                      类的定义

//实体类
class ENTITY
{
public:
    //                        属性
    //实体序号
    int Entity_Num;
    //实体种类
    Entity_Type Type;
    //碰撞箱
    float Size_x , Size_z;

    //实体位置及移动状态
    float Velocity_x , Velocity_z;
    int Chunk_x;          //区块坐标
    float Position_x , Position_z;  //相对坐标
    float Accelerate_x , Accelerate_z;
    //穿过区块的信号(给tzy用)
    short int Block_Through = 0; //-1代表从x负方向穿过边界，1代表从x正方向穿过边界，0代表未穿过边界

    //当前区块指针
    struct MapChunk *pId = NULL;

    //前后实体
    ENTITY *Prev_Entity = NULL;
    ENTITY *Next_Entity = NULL;

    //                    函数
    //判断是否碰撞
    bool Collision_X1(MapChunk *map);
    bool Collision_X2(MapChunk *map);
    bool Collision_Z1(MapChunk *map);
    bool Collision_Z2(MapChunk *map);

    //构造函数
    ENTITY(){
        //初始化全部为零
        Velocity_x = 0;
        Velocity_z = 0;
        Accelerate_x = 0;
        Accelerate_z = 0;
        Position_x = 0;
        Position_z = 0;
    }

    //析构函数
    ~ENTITY(){
        delete pId;
        pId = NULL;
    }

    //实体生成
    void Spawn(float position_x, float position_z, int block_x){
        Position_x = position_x;
        Position_z = position_z;
        Chunk_x = block_x;
    }

    //被攻击
    void Attacked();


};


class CREATURE : public ENTITY
{
public:
    //是不是友好生物
    Entity_Friend Friend;
    //移动能力
    float Accelerate;       //水平加速能力
    float Jump_Velocity;    //跳跃能力
    //战斗属性
    int Attack;             //攻击
    int Max_HitPoints;      //最大生命及生命
    int HitPoints;
    int Attack_Range;       //攻击范围
    int Defance;            //防御
    float Push_Back;        //击退

    //实体状态(就是buff)
//    bool OnFire = false;
//    bool Posioning = false;
//    bool Frozen = false;

    //朝向
    Face Facing;

    //                           函数
    //构造
    CREATURE(Entity_Struct *Data){

        //将属性传入
        Entity_Num = Data->Entity_Num;               //实体编号
        Type = Data->Type;                           //实体种类
        Size_x = Data->Size_x;                       //实体大小
        Size_z = Data->Size_z;

        HitPoints = Data->Max_HitPoints;             //最大生命及生命值
        Max_HitPoints = Data->Max_HitPoints;
        Attack = Data->Attack;                       //实体攻击
        Defance = Data->Defance;                     //实体防御
        Accelerate = Data->Accelerate;               //实体加速能力
        Attack_Range = Data->Attack_Range;           //实体攻击范围
        Jump_Velocity = Data->Jump_Velocity;         //实体跳跃能力


    }
    //析构
    ~CREATURE(){

    }

    //实体移动控制
    void Move_GroundWalk(        short int Direction_x       ,          bool Jump );
                         /*这个值只能是0和正负1，向前走，向后走和停*/ /*这个值代表是否发生跳跃*/
    //判断死亡
    bool is_Dead(){
        if(HitPoints <= 0){
            return true;
        }
        else{
            return false;
        }
    }


};

//玩家类
class PLAYER : public CREATURE
{
public:

    //玩家名称、背包
    QString Player_Name;
    Container Player_Backpack[15];
    Container Equip[5];

    //玩家能量
    float Player_Energy;
    float Energy_Consumption = 0; //能源消耗 正值为减少能量，负值为增加能量

    //玩家序号
    int Player_Num;

    //保存、加载存档
    bool SavePlayer();
    bool LoadPlayer(QString worldname);

    //存档地址、存档文件名
    QString SavePath;
    QString SaveName;
    void CreatPath(QString worldname);

    //判断方块是否在玩家碰撞箱内
    bool isinbox(int x,int z, int chunk);

    //玩家手中物品
    Item *current_item = NULL;

    PLAYER() : CREATURE(&Entity_Data[0]){
        /* 写入初始数值 */
        int i;
        Player_Num = 0;
        Player_Name = "";
        for(i=0;i<15;i++){
            Player_Backpack[i].Item_Num = 0;
            Player_Backpack[i].Quantity = 0;
            Player_Backpack[i].Item_type = Null;
        }
        for(i=0;i<5;i++){
            Equip[i].Item_Num = 0;
            Equip[i].Quantity = 0;
        }

        //检查存档目录，若没有，则创建
//        QDir* dir = new QDir();
//        if(dir->exists("saves") == false){
//            dir->mkdir("saves");
//        }
//        dir->cd("saves");
//        if(dir->exists("players") == false){
//            dir->mkdir("players");
//        }
    }

};

//怪物类
class MONSTER : public CREATURE
{
public:
    //构造与析构函数
    MONSTER(int Num) : CREATURE(&Entity_Data[Num]){}
    ~MONSTER();



};

//特殊实体object
class OBJECT : public ENTITY
{
public:
    int Damage;                //伤害
    float Object_Gravity;      //物品重力值
    float Max_Speed;           //最大飞行速度

    bool Destroy = false;              //自毁信号
    //
    //                           函数
    //构造析构
    OBJECT(){

    }
    ~OBJECT();

    void Shoot(float Sin,float Cos);          //输入方向，以sin cos值的方式
    void Flying();                            //飞行
    void Deal_Damage();                       //造成伤害

};

//******************************************************************

//                      外部函数

//bool LoadItem(int Num, Item *item);        //加载物品

//void SaveItem(Item *item);                 //写入元文件，真正程序不用


#endif // ENTITY_H
