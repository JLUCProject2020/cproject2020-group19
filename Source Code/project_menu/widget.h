#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMovie>
#include <stdlib.h>
#include <QSound>
#include <QMouseEvent>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QLabel>
#include <QDir>

#include "gameframework.h"
#include "entity.h"
#include "mapthread.h"
#include "gamethread.h"



//**************************************************************************************************
//                                    宏定义
#define BASIS_PX 64


//**************************************************************************************************
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    float translate_Pos_x(float mouse_x,  PLAYER *player);                           //屏幕坐标转化区块坐标
    float translate_Pos_z(float mouse_z , PLAYER *player);
    int translate_ChunkId(float mouse_x, PLAYER *player);

    int translate_Screen_x(int Block_x , int Chunk_Id ,PLAYER *player);
    int translate_Screen_z(int Block_z , PLAYER *player);
    QGraphicsBlurEffect *blureffect;
    //QLabel *pauseframe;

    //=============================游戏状态===============================

    bool synthesis = false;   //进入合成
    bool backpack = false;    //进入背包
    bool box_page = false;    //进入箱子
    int attack_state = 0;

    //==============================物品=================================

    //方块破坏
    void BREAK(int item_num, PLAYER *player,int num);

    //物品切换
    void Switch_Item(Container *firstitem, Container *seconditem);

    //方块放置
    void PUT(int item_num, PLAYER *player,int num);

    //物品合成
    void SEARCH(PLAYER *player);
    bool Synthesis(int line);
    void SYNTHESIS(int line,PLAYER *player);

    int current_synlist = 0;

    //箱子路径,箱子名称,箱子物品
    QString box_path;
    QString box_name;
    //====================================================================================


    //============================== 物块是否可以放置(fjy) =================================
    bool is_place(int CurrentChunk,MapChunk *pId, int x, int z, int depth);
    bool place1 = false;
    bool place2 = false;
    bool place3 = false;
    bool place4 = false;
    void Refreash();                           //重置place1-4
    //====================================================================================

    bool If_placeable(PLAYER *player,int x,int z,int ChunkId);  //物块是否可以放置(tzy)

    //==============================  帧率计算 ===========================================
    int Last_time = 0 , Current_time = 0 ;          //两帧时间
    int frame_state = 0;                       //用在frame_interval数组里面
    int frame_interval[20] = {0};              //帧率间隔
    void get_interval();                       //计算帧间隔的函数
    void frameRate();                          //实时帧率
    //===================================================================================
    QString Select_CubePath(int CubeNum);         //地图贴图路径
    QString Select_IconPath(int IconNum);         //物品贴图路径
public slots:
    void on_MapT_Finished();
    void on_GameT_Finished();
    void on_GameT_Begin();
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *e);
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);

private slots:
    void on_Exit_Button_clicked();

    void on_Single_Button_clicked();

    void on_return_button_clicked();

    void on_creat_world_clicked();

    void on_Cancel_Creating_clicked();

    void on_Begin_Creat_World_clicked();

    void on_enter_world_clicked();

    void on_no_button_clicked();

    void on_delete_world_clicked();

    void on_yes_button_clicked();


    void on_left_map_clicked();

    void on_right_map_clicked();

    void on_return_fromsetting_clicked();

    void on_setting_Button_clicked();

    void on_soundeffect_yes_radioButton_clicked();

    void on_soundeffect_no_radioButton_clicked();

private:
     void startGame();
     void pauseGame();
     void exitGame();
     void resumeGame();
     Ui::Widget *ui;
};

//***************************************************************************************************
//                                      外部函数

#endif // WIDGET_H
