#include "widget.h"
#include "ui_widget.h"
#include "MapInfo.h"
#include <QSizePolicy>
#include <QDebug>
#include <qvalidator.h>
#include <QTime>
#include <QElapsedTimer>
#include "entity.h"
#include <QPainter>
#include <QPen>
#include <QCheckBox>
#include <QSound>
#include <QDateTime>
#include "box.h"

//************************************************************************************************
//                                              宏定义
//游戏名称
#define Game_Name "Tangraria"
//玩家不同方向的gif路径
#define PlayerR_Path ":/new/player_gif/res/videos/PlayerR.gif"
#define PlayerL_Path ":/new/player_gif/res/videos/PlayerL.gif"
#define PlayerWL_Path ":/new/player_gif/res/videos/WalkingLeft.gif"
#define PlayerWR_Path ":/new/player_gif/res/videos/WalkingRight.gif"
//方块材质包路径
#define Earth_cube_image ":/new/background_images/res/Icon_Images/Earth.png"                  //行星土
#define Meterorite_cube_image ":/new/background_images/res/Icon_Images/areo_siderite.png"     //陨铁矿
#define Crystal_cube_image ":/new/background_images/res/Icon_Images/crystal.png"              //水晶
#define Iron_cube_image ":/new/background_images/res/images/IRON.png"                         //铁
#define Inventory_image ":/new/background_images/res/Icon_Images/Inventory.png"               //物品栏
#define Collecting_mach ":/new/background_images/res/Icon_Images/Energy.png"                  //辐射能收集器
#define Box_cube_image ":/new/background_images/res/Icon_Images/chest.png"                    //箱子
#define Lathe_image  ":/new/background_images/res/Icon_Images/Lathe.png"                    //机床
//物品材质包
#define Crystal_Icon_image ":/new/icon_image/res/Icon_Images/crystal_icon.png"              //水晶物品

#define Damage_lathe  ":/new/background_images/res/Icon_Images/damaged_lathe.png"        //损坏机床

#define Smelting_mach  ":/new/background_images/res/Icon_Images/furnace.png"       //自动熔炼机
#define Core_Image ":/new/background_images/res/Icon_Images/coretaq.png"                 //芯片
#define Part  ":/new/background_images/res/Icon_Images/part.png"                //零件
#define Iron_ingot ":/new/background_images/res/Icon_Images/iron_icon.png"           //铁锭
#define Energy_core ":/new/background_images/res/Icon_Images/core_2.png"          //能源核心
#define Laser_gun    ":/new/background_images/res/Icon_Images/gun.png"         //镭射枪
#define laser_blade  ":/new/background_images/res/Icon_Images/light_sword.png"         //激光剑
#define Iron_stick   ":/new/background_images/res/Icon_Images/iron_stick.png"         //铁棍
#define Crystal_blade ":/new/background_images/res/Icon_Images/crystal_sword.png"        //水晶剑
#define Crystal_gun   ":/new/background_images/res/Icon_Images/gun.png"        //水晶能爆枪
#define Platform              //平台
#define Battery               //电池
#define Final_Battery         //终极电池
#define Armor                 //化装甲
#define Mechanical_arm        //机械手臂
#define Nanorobot             //纳米机器人
#define Overclock_system      //超频系统
#define High_energy_bit       //高能钻头

//背景图片
#define Gamebgi_Path ":/new/background_images/res/images/game_bgi.png"
#define Undergroundbgi_Path ":/new/background_images/res/images/underground_bgi.png"
#define Synbgi_Path ":/new/background_images/res/images/syn_bgi.png"
#define Synarrow_Image ":/new/background_images/res/images/syn_arrow.png"
#define Syncross_Image ":/new/background_images/res/images/Syn_cross.png"
//屏幕大小
#define Screen_Width 1440
#define Screen_Height 810
#define Proportion 1.5



//************************************************************************************************
//                                              全局变量

static int Nth_Map = 0;          //世界序号
static QString WorldName;        //世界名
static long int Seed;            //世界种子
static QMovie *m = NULL;         //背景gif
static int Dialog_status;        //对话框状态 0为是否删除世界，1为内容为空提示
static QString player_name = "";      //玩家名称
static QMovie *player_m = NULL;  //玩家背景gif

//程序状态
static bool isGame = false;
static int timer_ID;

//主游戏内容
static PLAYER *current_player = NULL;        //玩家
static OBJECT *bullet[50] = {NULL};          //非生物实体（子弹）
static MONSTER *monster[10] = {NULL};        //敌怪
static Item *current_item = NULL;            //当前物品
static short int handitem = 0;               //物品在背包中的位置
static GenerateMap *Map = NULL;              //地图
static MapThread *MapT = new MapThread;      //地图线程
//static GameThread *GameT = new GameThread;   //游戏线程
static bool Accessible = false;
static CONTAINER BOX[15] = {{0,0,Null},{0,0,Null},{0,0,Null},{0,0,Null},{0,0,Null},
                            {0,0,Null},{0,0,Null},{0,0,Null},{0,0,Null},{0,0,Null},
                            {0,0,Null},{0,0,Null},{0,0,Null},{0,0,Null},{0,0,Null}};

//按键状态
static bool A_Status = false;
static bool D_Status = false;
static bool Space_Status = false;
static bool E_Status = false;              //test only

static int mouse_x;
static int mouse_z;
//====================test only=======================//
static int bright = 0;
//====================test only=======================//
//设置选项
static bool soundeffect = true;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //*******************************************************************************************
    //                                    主widget设置
    ui->setupUi(this);
    this->setWindowTitle(Game_Name);                        //调整窗口标题
    this->setFixedSize(Screen_Width,Screen_Height);                              //调整窗口大小，同时使窗口大小不可调



    ui->stackedWidget->setCurrentWidget(ui->lobby);           //初始化页面为lobby
    ui->stackedWidget->move(0,0);
    ui->stackedWidget->resize(this->width(),this->height());  //让stackedwidget跟主Widget一样大小
    ui->bgi->move(0,0);
    ui->bgi->resize(this->width(),this->height());            //让bgi(背景动图)跟主Widget一样大小
    ui->bgi->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose,false);
    this->setMouseTracking(true);                             //鼠标追踪默认开启

    //玩家label调整
    ui->astronaut_player->move((Screen_Width - BASIS_PX)/2,(Screen_Height - 2 * BASIS_PX)/2);
    ui->astronaut_player->hide();                             //玩家的label隐藏
    ui->astronaut_player->setScaledContents(true);
    ui->astronaut_player->resize(BASIS_PX,BASIS_PX*2);
    ui->player_name->move((Screen_Width - BASIS_PX)/2,(Screen_Height - 2 * BASIS_PX)/2 - ui->player_name->height());
    ui->player_name->hide();
    //加入动图背景
    m = new QMovie("../res/videos/PlanetEarth.gif");
    m->start();
    ui->bgi->setScaledContents(true);
    ui->bgi->setMovie(m);


    //***********************************************************************************
    //                                    确认窗口格式设置
    //按比例缩放===============================================================

    {
        ui->sure_label->resize((int)(ui->sure_label->width() * Proportion)
                               ,(int)(ui->sure_label->height() * Proportion));
        ui->sure_label->move((int)(ui->sure_label->x() * Proportion)
                             ,(int)(ui->sure_label->y() * Proportion));
        ui->yes_button->resize((int)(ui->yes_button->width() * Proportion)
                               ,(int)(ui->yes_button->height() * Proportion));
        ui->yes_button->move((int)(ui->yes_button->x() * Proportion)
                             ,(int)(ui->yes_button->y() * Proportion));
        ui->no_button->resize((int)(ui->no_button->width() * Proportion)
                              ,(int)(ui->no_button->height() * Proportion));
        ui->no_button->move((int)(ui->no_button->x() * Proportion)
                            ,(int)(ui->no_button->y() * Proportion));
    }

    //========================================================================
    ui->sure_widget->hide();                     //刚开始时隐藏
    ui->sure_widget->move(0,0);
    ui->sure_widget->resize(Screen_Width,Screen_Height);
    ui->sure_frame->setStyleSheet("QWidget{background:rgba(0,169,231,70);"
                                  "border:4px groove gray;"
                                  "border-radius:20px;"
                                  "padding:2px 4px;"
                                  "color:white}");
    ui->sure_frame->move((int)(209 * Proportion),(int)(109 * Proportion));
    ui->sure_frame->resize((int)(ui->sure_frame->width() * Proportion)
                                  ,(int)(ui->sure_frame->height() * Proportion));
    ui->yes_button->setStyleSheet("QPushButton{color:white;"
                                  "background:transparent;"
                                  "border:0px}");                    //鼠标不停留时的按钮
    ui->yes_button->setStyleSheet("QPushButton:hover{color:white;"
                                  "background:transparent;"
                                  "border-color : #808080;"
                                  "border-width : 4px;"
                                  "border-style : outset;}");        //鼠标停留时的按钮
    ui->no_button->setStyleSheet("QPushButton:!hover{color:white;"
                                 "background:transparent;"
                                 "border:0px}");
    ui->no_button->setStyleSheet("QPushButton:hover{color:white;"
                                 "background:transparent;"
                                 "border-color : #808080;"
                                 "border-width : 4px;"
                                 "border-style : outset;}");

    //*******************************************************************************************
    //                                    lobby页设置
    //lobby页中的标题
    ui->Caption_lobby->resize(315,86);
    ui->Caption_lobby->setScaledContents(true);
    ui->Caption_lobby->setPixmap(QPixmap("../res/images/caption.jpg"));

    //*************************************************************************************************
    //                                     单人游戏界面设置
    //左右地图按钮
    //按比例缩放
    {
        ui->left_map->resize((int)(ui->left_map->width() * Proportion),(int)(ui->left_map->height() * Proportion));
        ui->right_map->resize((int)(ui->right_map->width() * Proportion),(int)(ui->right_map->height() * Proportion));
        ui->left_map->move((int)(ui->left_map->x() * Proportion),(int)(ui->left_map->y() * Proportion));
        ui->right_map->move((int)(ui->right_map->x() * Proportion),(int)(ui->right_map->y() * Proportion));

        ui->caption_choosemap->resize((int)(ui->caption_choosemap->width() * Proportion)
                                      ,(int)(ui->caption_choosemap->height() * Proportion));
        ui->caption_choosemap->move((int)(ui->caption_choosemap->x() * Proportion)
                                    ,(int)(ui->caption_choosemap->y() * Proportion));
        ui->showmap_label->resize((int)(ui->showmap_label->width() * Proportion)
                                  ,(int)(ui->showmap_label->height() * Proportion));
        ui->showmap_label->move((int)(ui->showmap_label->x() * Proportion)
                                ,(int)(ui->showmap_label->y() * Proportion));
        ui->enter_world->resize((int)(ui->enter_world->width() * Proportion)
                                ,(int)(ui->enter_world->height() * Proportion));
        ui->enter_world->move((int)(ui->enter_world->x() * Proportion)
                              ,(int)(ui->enter_world->y() * Proportion));
        ui->creat_world->resize((int)(ui->creat_world->width() * Proportion)
                                ,(int)(ui->creat_world->height() * Proportion));
        ui->creat_world->move((int)(ui->creat_world->x() * Proportion)
                              ,(int)(ui->creat_world->y() * Proportion));
        ui->delete_world->resize((int)(ui->delete_world->width() * Proportion)
                                 ,(int)(ui->delete_world->height() * Proportion));
        ui->delete_world->move((int)(ui->delete_world->x() * Proportion)
                               ,(int)(ui->delete_world->y() * Proportion));
        ui->return_button->resize((int)(ui->return_button->width() * Proportion)
                                  ,(int)(ui->return_button->height() * Proportion));
        ui->return_button->move((int)(ui->return_button->x() * Proportion)
                                ,(int)(ui->return_button->y() * Proportion));
        ui->button_widget->resize((int)(ui->button_widget->width() * Proportion)
                                  ,(int)(ui->button_widget->height() * Proportion));
        ui->button_widget->move((int)(ui->button_widget->x() * Proportion)
                                ,(int)(ui->button_widget->y() * Proportion));
    }

    //=========================================================================================
    //ui->caption_choosemap
    //设置样式表
    ui->caption_choosemap->setStyleSheet("QLabel{background:transparent;border:px;color:white}");
    ui->left_map->setStyleSheet("QPushButton{border-image: url(../res/images/left_map_button.png);}");
    //ui->left_map->setStyleSheet("QPushButton:hover{width:101px;height : 101px;}");
    ui->right_map->setStyleSheet("QPushButton{border-image: url(../res/images/right_map_button.png);}");
    ui->showmap_label->setStyleSheet("QLabel{border-image: url(../res/images/world_bgi1.jpg);"
                                     "border-radius : 25px;"
                                     "border-style : inset;"
                                     "border-width : 4px;"
                                     "border-color : #97b0b8;"
                                     "color:white}");
    ui->showmap_label->setScaledContents(true);
    //ui->showmap_label->setPixmap(QPixmap("../res/images/world_bgi.jpg"));

    //***********************************************************************************************
    //                                          创建世界页面设置
    //按比例缩放
    {
        ui->Begin_Creat_World->resize((int)(ui->Begin_Creat_World->width() * Proportion)
                                      ,(int)(ui->Begin_Creat_World->height() * Proportion));
        ui->Begin_Creat_World->move((int)(ui->Begin_Creat_World->x() * Proportion)
                                    ,(int)(ui->Begin_Creat_World->y() * Proportion));
        ui->Cancel_Creating->resize((int)(ui->Cancel_Creating->width() * Proportion)
                                    ,(int)(ui->Cancel_Creating->height() * Proportion));
        ui->Cancel_Creating->move((int)(ui->Cancel_Creating->x() * Proportion)
                                  ,(int)(ui->Cancel_Creating->y() * Proportion));
        ui->caption_createmap->resize((int)(ui->caption_createmap->width() * Proportion)
                                      ,(int)(ui->caption_createmap->height() * Proportion));
        ui->caption_createmap->move((int)(ui->caption_createmap->x() * Proportion)
                                    ,(int)(ui->caption_createmap->y() * Proportion));
        ui->creatingmap_frame->resize((int)(ui->creatingmap_frame->width() * Proportion)
                                      ,(int)(ui->creatingmap_frame->height() * Proportion));
        ui->creatingmap_frame->move((int)(ui->creatingmap_frame->x() * Proportion)
                                    ,(int)(ui->creatingmap_frame->y() * Proportion));
        ui->Seed_Widget->resize((int)(ui->Seed_Widget->width() * Proportion)
                                ,(int)(ui->Seed_Widget->height() * Proportion));
        ui->Seed_Widget->move((int)(ui->Seed_Widget->x() * Proportion)
                              ,(int)(ui->Seed_Widget->y() * Proportion));
        ui->emptyseed->resize((int)(ui->emptyseed->width() * Proportion)
                              ,(int)(ui->emptyseed->height() * Proportion));
        ui->emptyseed->move((int)(ui->emptyseed->x() * Proportion)
                            ,(int)(ui->emptyseed->y() * Proportion));
        ui->seed_label->resize((int)(ui->seed_label->width() * Proportion)
                               ,(int)(ui->seed_label->height() * Proportion));
        ui->seed_label->move((int)(ui->seed_label->x() * Proportion)
                             ,(int)(ui->seed_label->y() * Proportion));
        ui->seed_lineEdit->resize((int)(ui->seed_lineEdit->width() * Proportion)
                                  ,(int)(ui->seed_lineEdit->height() * Proportion));
        ui->seed_lineEdit->move((int)(ui->seed_lineEdit->x() * Proportion)
                                ,(int)(ui->seed_lineEdit->y() * Proportion));
        ui->Worldname_Widget->resize((int)(ui->Worldname_Widget->width() * Proportion)
                                     ,(int)(ui->Worldname_Widget->height() * Proportion));
        ui->Worldname_Widget->move((int)(ui->Worldname_Widget->x() * Proportion)
                                   ,(int)(ui->Worldname_Widget->y() * Proportion));
        ui->emptyworldname->resize((int)(ui->emptyworldname->width() * Proportion)
                                   ,(int)(ui->emptyworldname->height() * Proportion));
        ui->emptyworldname->move((int)(ui->emptyworldname->x() * Proportion)
                                 ,(int)(ui->emptyworldname->y() * Proportion));
        ui->worldname_label->resize((int)(ui->worldname_label->width() * Proportion)
                                    ,(int)(ui->worldname_label->height() * Proportion));
        ui->worldname_label->move((int)(ui->worldname_label->x() * Proportion)
                                  ,(int)(ui->worldname_label->y() * Proportion));
        ui->worldname_lineEdit->resize((int)(ui->worldname_lineEdit->width() * Proportion)
                                       ,(int)(ui->worldname_lineEdit->height() * Proportion));
        ui->worldname_lineEdit->move((int)(ui->worldname_lineEdit->x() * Proportion)
                                     ,(int)(ui->worldname_lineEdit->y() * Proportion));
    }



    //调整lineedit样式
    ui->seed_lineEdit->setStyleSheet("QLineEdit{background:transparent;"
                                     "border-style : inset;"
                                     "border-width : 4px;"
                                     "border-color : #97b0b8;"
                                     "border-radius:10px;"
                                     "color:white}");
    ui->worldname_lineEdit->setStyleSheet("QLineEdit{background:transparent;"
                                          "border-style : inset;"
                                          "border-width : 4px;"
                                          "border-color : #97b0b8;"
                                          "border-radius:10px;"
                                          "color:white}");
    //设置seedlineedit只能输数字
    ui->seed_lineEdit->setValidator(new QIntValidator(-2147483648,2147483647,this));
    //调整label样式
    ui->seed_label->setStyleSheet("QLabel{background:transparent;"
                                  "border-style : inset;"
                                  "border-width : 4px;"
                                  "border-color : #97b0b8;"
                                  "border-radius:10px;"
                                  "color:white}");
    ui->worldname_label->setStyleSheet("QLabel{background:transparent;"
                                       "border-style : inset;"
                                       "border-width : 4px;"
                                       "border-color : #97b0b8;"
                                       "border-radius:10px;"
                                       "color:white}");


    ui->caption_createmap->setStyleSheet("QLabel{background:transparent;border:0px;color:white}");
    ui->creatingmap_frame->setStyleSheet("QLabel{background:rgba(0,169,231,70);"
                                         "border:4px groove gray;"
                                         "border-radius:20px;"
                                         "padding:2px 4px;}");
    //ui->label->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    //设置空白输入提示栏
    ui->emptyseed->setStyleSheet("QLabel{background:transparent;"
                                 "color:red}");
    ui->emptyworldname->setStyleSheet("QLabel{background:transparent;"
                                 "color:red}");

    //*********************************************************************************************
    //                                       加载页面设置
    ui->loading_label->setStyleSheet("QLabel{color:white}");

    //***********************************************************************************************
    //                                         游戏界面设置
    //设置玩家label,主游戏画面格式

    //*****************************************************************************************************
    //                                           线程槽函数connect
    connect(MapT,&MapThread::Finished,this,&Widget::on_MapT_Finished);
    //connect(GameT, &GameThread::Finished,this,&Widget::on_GameT_Finished);
    //connect(GameT,&GameThread::Begin,this,&Widget::on_GameT_Begin);
    //退出时关闭线程
    connect(this,&QWidget::destroyed,
            []()
            {

                MapT->quit();
                MapT->wait();
                delete MapT;
//                GameT->quit();
//                GameT->wait();
//                delete GameT;
            });


    //*****************************************************************************************************
    //                                           设置页面设置
    ui->playername_lineEdit->setStyleSheet("QLineEdit{background:rgba(0,169,231,70);"
                                     "border-style : inset;"
                                     "border-width : 4px;"
                                     "border-color : #97b0b8;"
                                     "border-radius: 10px;"
                                     "color:white}");
    ui->playername_label->setStyleSheet("QLabel{background:rgba(0,169,231,70);"
                                     "border-style : inset;"
                                     "border-width : 4px;"
                                     "border-color : #97b0b8;"
                                     "border-radius: 10px;"
                                     "color:white}");
    ui->soundeffect_label->setStyleSheet("QLabel{background:rgba(0,169,231,70);"
                                            "border-style : inset;"
                                            "border-width : 4px;"
                                            "border-color : #97b0b8;"
                                            "border-radius: 10px;"
                                            "color:white}");
    ui->soundeffect_yes_radioButton->setChecked(true);
//    ui->soundeffect_yes_radioButton->setStyleSheet("QRadioButton{background:rgba(0,169,231,70);"
//                                                   "border-style : inset;"
//                                                   "border-width : 4px;"
//                                                   "border-color : #97b0b8;"
//                                                   "border-radius: 10px;"
//                                                   "color:white}"
//                                                   );

//    ui->soundeffect_no_radioButton->setStyleSheet("QRadioButton{background:rgba(0,169,231,70);"
//                                                   "border-style : inset;"
//                                                   "border-width : 4px;"
//                                                   "border-color : #97b0b8;"
//                                                   "border-radius: 10px;"
//                                                   "color:white;}"
//                                                   );



}

Widget::~Widget()
{
    delete ui;

}

//***************************************************************************************************
//                                       事件函数

void Widget::paintEvent(QPaintEvent *event){
    if(!isGame){
        //qDebug() << "不是游戏";
        return;

    }
    //qDebug() << "绘图开始" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd");
    //qDebug() << "进入绘图";
    //画地图========================================================================================
    QPainter painter;
    painter.begin(this);
    int block_x, block_z;
    int screen_x, screen_z;
    int ini_block_z , final_block_z;
    //画背景
    if(current_player->Position_z < 45){
        painter.drawPixmap(rect(),QPixmap(Gamebgi_Path));
    }else{
        painter.drawPixmap(0,0,720,405,QPixmap(Undergroundbgi_Path));
        painter.drawPixmap(720,0,720,405,QPixmap(Undergroundbgi_Path));
        painter.drawPixmap(0,405,720,405,QPixmap(Undergroundbgi_Path));
        painter.drawPixmap(720,405,720,405,QPixmap(Undergroundbgi_Path));
    }

    if(current_player->Position_z <= 8){
        ini_block_z = 0;
        final_block_z = (int)(current_player->Position_z + 8);
    }else if(current_player->Position_z >= (CHUNK_HEIGHT - 8)){
        ini_block_z = (int)(current_player->Position_z - 8);
        final_block_z = 100;
    }else{
        ini_block_z = (int)(current_player->Position_z - 8);
        final_block_z = (int)(current_player->Position_z + 8);
    }
    QString Path;
    //打印玩家左方区块
    for (block_x = 0; block_x < CHUNK_WIDTH ; block_x++) {
        for (block_z = ini_block_z;
             block_z < final_block_z; block_z++) {
            screen_x = translate_Screen_x(block_x, current_player->Chunk_x - 1 ,current_player);
            screen_z = translate_Screen_z(block_z, current_player);

            if((screen_x >= -64)&&(screen_x <= Screen_Width + 64)
                    &&(screen_z >= -64)&&(screen_z <= Screen_Height + 64)){
                //qDebug() << "画";
                Path = Select_CubePath(current_player->pId->LeftChunk->Blocks[block_x][block_z]);
                painter.drawPixmap(screen_x , screen_z , BASIS_PX , BASIS_PX,
                                   QPixmap(Path));

            }

        }
    }
    //打印玩家自身区块

    for (block_x = 0; block_x < CHUNK_WIDTH ; block_x ++) {
        for (block_z = ini_block_z;
             block_z < final_block_z; block_z++) {
            screen_x = translate_Screen_x(block_x, current_player->Chunk_x ,current_player);
            screen_z = translate_Screen_z(block_z, current_player);
            if((screen_x >= -64)&&(screen_x <= Screen_Width + 64)&&
                    (screen_z >= -64)&&(screen_z <= Screen_Height + 64)){
                Path = Select_CubePath(current_player->pId->Blocks[block_x][block_z]);
                painter.drawPixmap(screen_x , screen_z , BASIS_PX , BASIS_PX,
                                   QPixmap(Path));

            }

        }
    }
    //打印玩家右边区块
    for (block_x = 0;block_x < CHUNK_WIDTH ; block_x ++) {
        for (block_z = ini_block_z;
             block_z < final_block_z; block_z++) {
            screen_x = translate_Screen_x(block_x, current_player->Chunk_x + 1 ,current_player);
            screen_z = translate_Screen_z(block_z, current_player);
            if((screen_x >= -64)&&(screen_x <= Screen_Width + 64)&&
                    (screen_z >= -64)&&(screen_z <= Screen_Height + 64)){
                Path = Select_CubePath(current_player->pId->RightChunk->Blocks[block_x][block_z]);
                painter.drawPixmap(screen_x , screen_z , BASIS_PX , BASIS_PX,
                                   QPixmap(Path));

            }

        }
    }

    QPen pen;

    //画鼠标框框=================================================================================
    if(!backpack && !synthesis){

        pen.setWidth(5);

        int paint_x = floor(translate_Pos_x(mouse_x,current_player));
        int paint_chunk = translate_ChunkId(mouse_x,current_player);
        int paint_z = floor(translate_Pos_z(mouse_z,current_player));
        int dis_x = translate_Screen_x(paint_x,paint_chunk,current_player) - Screen_Width/2;
        int dis_z = translate_Screen_z(paint_z,current_player) - Screen_Height/2;
        float R = sqrt(dis_x * dis_x + dis_z * dis_z);
        //qDebug() << dis_x << dis_z << R;
        if(R <= (5 * BASIS_PX)){
            pen.setColor(QColor(0,255,127,95));
            Accessible = true;
        }else {
            pen.setColor(QColor(255,128,128,95));
            Accessible = false;
        }
        paint_x = translate_Screen_x(paint_x,paint_chunk,current_player);
        paint_z = translate_Screen_z(paint_z,current_player);
        painter.setPen(pen);
        painter.drawRect(paint_x,paint_z,BASIS_PX,BASIS_PX);
        if(!Accessible){
            pen.setStyle(Qt::PenStyle::DashLine);
            pen.setColor(QColor(255,128,128,150));
            painter.setPen(pen);
            painter.drawEllipse(QPoint(Screen_Width/2,Screen_Height/2),5 * BASIS_PX,5 * BASIS_PX);
        }

    }

    //画物品栏============================================================================
    int i,j;
    QString Icon_Path;
    if(!synthesis){
        for(i=0;i<15;i++){

            if(i >= 5 && !backpack){
                break;
            }
            //画物品框
            painter.drawPixmap(40 + 110 * (i%5) ,40 + 110*(i/5),90,90,QPixmap(Inventory_image));

            //物品贴图
            Icon_Path = Select_IconPath(current_player->Player_Backpack[i].Item_Num);
            painter.drawPixmap(40 + 110 * (i%5) + 13,40 + 110*(i/5) + 13,BASIS_PX,BASIS_PX,QPixmap(Icon_Path));

            //物品数量
            if(current_player->Player_Backpack[i].Quantity != 0){
                QFont font = painter.font();
                font.setPixelSize(30);
                painter.setFont(font);
                pen.setColor(Qt::GlobalColor::black);
                painter.setPen(pen);
                painter.drawText(QRect(50 + 110*(i%5) , 100 + 110*(i/5),80,30),QString::number(current_player->Player_Backpack[i].Quantity));
            }


            //画当前物品选择框
            if(handitem == i){
                pen.setColor(QColor(0,255,255,120));
                pen.setStyle(Qt::PenStyle::SolidLine);
                pen.setWidth(8);
                painter.setPen(pen);
                painter.drawRect(40 + 110*(i%5),40 + 110*(i/5),90,90);
            }
        }

        //箱子界面
        if(box_page){
            for (i=15;i<30;i++) {
                //画物品框
                painter.drawPixmap(700 + 110 * ((i-15)%5) ,40 + 110*((i-15)/5),90,90,QPixmap(Inventory_image));

                //物品贴图
                QString Icon_Path = Select_IconPath(BOX[i-15].Item_Num);
                painter.drawPixmap(700 + 110 * ((i-15)%5) + 13,40 + 110*((i-15)/5) + 13,BASIS_PX,BASIS_PX,QPixmap(Icon_Path));

                //物品数量
                if(BOX[i-15].Quantity != 0){
                    QFont font = painter.font();
                    font.setPixelSize(30);
                    painter.setFont(font);
                    pen.setColor(Qt::GlobalColor::black);
                    painter.setPen(pen);
                    painter.drawText(QRect(760 + 110*((i-15)%5) , 100 + 110*((i-15)/5),30,30),
                                     QString::number(BOX[i-15].Quantity));
                }

                //画当前物品选择框
                if(handitem == i){
                    pen.setColor(QColor(0,255,255,120));
                    pen.setStyle(Qt::PenStyle::SolidLine);
                    pen.setWidth(8);
                    painter.setPen(pen);
                    painter.drawRect(700 + 110 * ((i-15)%5) ,40 + 110*((i-15)/5),90,90);
                }
            }
        }
    }


    //===========================攻击==========================//


    //===============================================================

    //===========================合成页面============================//
    if(synthesis){
        //合成表贴图
        for (i=0;i<4;i++) {
            //画框框
            painter.drawPixmap(100 + 180 * (i%5) , 80 + 180 *(i/5),150,150,QPixmap(Inventory_image));

            //画合成材料
            Icon_Path = Select_IconPath(Synthesis_List[current_synlist].synthesis_item[i].Item_Num);
            painter.drawPixmap(100 + 180 * (i%5) + 11 , 80 + 180*(i/5) + 11,BASIS_PX*2,BASIS_PX*2,QPixmap(Icon_Path));

            //画合成数量
            if(Synthesis_List[current_synlist].synthesis_item[i].Quantity != 0){
                QFont font = painter.font();
                font.setPixelSize(40);
                painter.setFont(font);
                pen.setColor(Qt::GlobalColor::black);
                painter.setPen(pen);
                painter.drawText(QRect(100 + 180 * (i%5) + 90 , 80 + 180 *(i/5) + 90,60,60),
                                 QString::number(Synthesis_List[current_synlist].synthesis_item[i].Quantity));
            }
        }
        //画箭头
        painter.drawPixmap(840 , 80 , 300 , 150, QPixmap(Synarrow_Image));
        //如果不能合成，画叉叉
        if(!Synthesis(current_synlist)){
            painter.drawPixmap(915 , 80 , 150 , 150, QPixmap(Syncross_Image));
        }

        //画合成物品
        painter.drawPixmap(1170,80,150,150,QPixmap(Inventory_image));
        Icon_Path = Select_IconPath(Synthesis_List[current_synlist].product);
        painter.drawPixmap(1170 + 11,80 + 11 ,128,128,QPixmap(Icon_Path));

    }
    //===============================================================

    //====================test only=======================//
    //此处仅测试用，将来用于死亡效果（屏幕逐渐变黑）
    if(E_Status == true){

        bright++;
        if(bright > 255){
            bright = 255;
        }
        QBrush brush;
        brush.setColor(QColor(0,0,0,bright));
        brush.setStyle(Qt::SolidPattern);
        pen.setWidth(0);
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(0,0,Screen_Width,Screen_Height);
    }else{
        bright = 0;
    }
    //===================================================//

    painter.end();

}

void Widget::timerEvent(QTimerEvent *e){
    //qDebug() << A_Status << D_Status;

    if(isGame){
        //qDebug() << "开始计算走路" << QDateTime::currentDateTime().toString("zzz");


        //先计算走路=================================================================================
        //qDebug() << current_player->Position_x << current_player->Position_z;
        //qDebug() << "gif name:" << player_m->fileName();
        if(((A_Status == false)&&(D_Status == false))||(A_Status && D_Status)){
            current_player->Move_GroundWalk(0, Space_Status);
            if(current_player->Facing == Back){
                delete player_m;
                player_m = new QMovie(PlayerL_Path);
                player_m->setSpeed(500);
                ui->astronaut_player->setMovie(player_m);
                player_m->start();
            }else{
                delete player_m;
                player_m->setSpeed(500);
                player_m = new QMovie(PlayerR_Path);
                ui->astronaut_player->setMovie(player_m);
                player_m->start();
            }
        }
        else{
            if(A_Status || D_Status){
                //qDebug() << "走";
                if(A_Status){
                    current_player->Move_GroundWalk(-1, Space_Status);
                    current_player->Facing = Back;
                    if(player_m->fileName() != PlayerWL_Path){
                        qDebug() << "executed left";
                        delete player_m;
                        player_m = new QMovie(PlayerWL_Path);
                        player_m->setSpeed(500);
                        ui->astronaut_player->setMovie(player_m);
                        player_m->start();
                    }

                }
                else{
                    current_player->Move_GroundWalk(1, Space_Status);
                    current_player->Facing = Front;
                    if(player_m->fileName() != PlayerWR_Path){
                        qDebug() << "executed right";
                        delete player_m;
                        player_m = new QMovie(PlayerWR_Path);
                        player_m->setSpeed(500);
                        ui->astronaut_player->setMovie(player_m);
                        player_m->start();
                    }

                }
            }
        }
        //判断是否穿过区块=========================================================================

        if(current_player->Block_Through != 0){
            Map->geneNewChunk(&(current_player->pId),current_player->Block_Through);
            current_player->Block_Through = 0;
//            qDebug() << "-2区块" << current_player->pId->LeftChunk->LeftChunk <<
//                        "-1区块" << current_player->pId->LeftChunk <<
//                        "0区块" << current_player->pId <<
//                        "1区块" << current_player->pId->RightChunk <<
//                        "2区块" << current_player->pId->RightChunk->RightChunk;
//            qDebug() << current_player->Chunk_x << current_player->pId->ChunkId;
        }

        //=====================================================================

        repaint();
        frameRate();//显示帧率

        //清除数量为0,以及空气的物品=============================================
        int i;
        for(i=0;i<15;i++){
            if(current_player->Player_Backpack[i].Quantity <= 0){
                current_player->Player_Backpack[i].Item_Num = 0;
                current_player->Player_Backpack[i].Item_type = Null;
            }
            if(current_player->Player_Backpack[i].Item_Num == 0){
                current_player->Player_Backpack[i].Quantity = 0;
                current_player->Player_Backpack[i].Item_type = Null;
            }
        }
        //============================================================
    }

}

void Widget::keyPressEvent(QKeyEvent *e){
    qDebug() << "按键被按下" << e->key();
    if(e->key() == Qt::Key_A){
        A_Status = true;

    }
    if(e->key() == Qt::Key_S){
        //S_status = true;
    //debug 显示material
        int n;
        for (n=0;n<5;n++) {
            qDebug() << "材料" << material[n].Item_Num << material[n].Quantity;
        }
    }
    if(e->key() == Qt::Key_D){
        D_Status = true;
    }
    if(e->key() == Qt::Key_W){
        //W_status = true;
    }
    if(e->key() == Qt::Key_Space){
        Space_Status = true;
        qDebug() << "space被按下" << Space_Status;
    }
    //背包键位B
    if(e->key() == Qt::Key_B){
        if(!synthesis){
            backpack = !backpack;
            if(!backpack){
                int i;
                handitem = 0;
                if(box_page){
                    Box_Close(box_path,BOX);
                    for (i=0;i<15;i++) {
                        BOX[i].Item_Num=0;
                        BOX[i].Quantity=0;
                        BOX[i].Item_type=Null;
                    }
                    box_name = "";
                    box_path = "";
                }
                box_page = false;

            }
        }else{
            synthesis = false;
            current_synlist = 0;
        }


    }

    //退出游戏
    if(e->key() == Qt::Key::Key_Escape){
        //pauseGame();
        exitGame();
    }

    //合成键位，O为向前翻，P为向后翻,I确认合成
    if(e->key() == Qt::Key::Key_O){
        current_synlist--;
        if(current_synlist < 0){
            current_synlist = 8;
        }
    }
    if(e->key() == Qt::Key::Key_P){
        current_synlist = (current_synlist+1) % 8;
    }
    if(e->key() == Qt::Key::Key_I){
        qDebug() << "I键被按下";
        if(Synthesis(current_synlist)){
            SYNTHESIS(current_synlist,current_player);
            SEARCH(current_player);
        }
    }

    if(e->key() == Qt::Key_E){
        E_Status = true;
        //resumeGame();
    }
    if(e->key() == Qt::Key::Key_1){
        handitem = 0;
    }
    if(e->key() == Qt::Key::Key_2){
        handitem = 1;
    }
    if(e->key() == Qt::Key::Key_3){
        handitem = 2;
    }
    if(e->key() == Qt::Key::Key_4){
        handitem = 3;
    }
    if(e->key() == Qt::Key::Key_5){
        handitem = 4;
    }
}

void Widget::keyReleaseEvent(QKeyEvent *e){
    if(!isGame){
        return;
    }
    if(e->key() == Qt::Key_A){
        A_Status = false;
    }
    if(e->key() == Qt::Key_S){
        //S_status = false;
    }
    if(e->key() == Qt::Key_D){
        D_Status = false;
    }
    if(e->key() == Qt::Key_W){
        //W_status = false;
    }
    if(e->key() == Qt::Key_Space){
        Space_Status = false;
    }
    if(e->key() == Qt::Key_Q){
        //PushBack();
    }
    if(e->key() == Qt::Key_E){
        E_Status = false;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    mouse_x = event->x();
    mouse_z = event->y();
}

void Widget::mousePressEvent(QMouseEvent *event){
    if(!isGame){
        return;
    }
    int last_handitem = handitem;
    int line, row;

    //物品操作
    if(backpack){
        if(event->button() == Qt::LeftButton){//左键点击移动物品

            if( (mouse_x >= 40) && (mouse_x <= 570) && (mouse_z >= 40) && (mouse_z <= 350)){
                line = (mouse_x - 40)/100;
                row = (mouse_z - 40)/100;
                handitem = line + row * 5;

                if((handitem > 14) || (last_handitem > 14)){
                    if(handitem <= 14){//箱子转移到背包
                        Switch_Item(&BOX[last_handitem - 15],
                                    &current_player->Player_Backpack[handitem]);
                    }
                    else if(last_handitem <= 14){//背包转移到箱子
                        Switch_Item(&current_player->Player_Backpack[last_handitem],
                                    &BOX[handitem - 15]);
                    }else{//箱子转移到箱子
                        Switch_Item(&BOX[last_handitem - 15],
                                    &BOX[handitem - 15]);
                    }
                }
                else{//背包转移到背包
                    Switch_Item(&current_player->Player_Backpack[last_handitem],
                                &current_player->Player_Backpack[handitem]);
                }
                qDebug() << handitem;
            }
            //若打开了箱子
            if(box_page && ((mouse_x >= 700) && (mouse_x <= 1230) && (mouse_z >= 40) && (mouse_z <= 350))){
                line = (mouse_x - 700)/100;
                row = (mouse_z - 40)/100;
                handitem = line + row * 5 + 15;

                if((handitem > 14) || (last_handitem > 14)){
                    if(handitem <= 14){//箱子转移到背包
                        Switch_Item(&BOX[last_handitem - 15],
                                    &current_player->Player_Backpack[handitem]);
                    }
                    else if(last_handitem <= 14){//背包转移到箱子
                        Switch_Item(&current_player->Player_Backpack[last_handitem],
                                    &BOX[handitem - 15]);
                    }else{//箱子转移到箱子
                        Switch_Item(&BOX[last_handitem - 15],
                                    &BOX[handitem - 15]);
                    }
                }
                else{//背包转移到背包
                    Switch_Item(&current_player->Player_Backpack[last_handitem],
                                &current_player->Player_Backpack[handitem]);
                }

            }
        }
        //右键点击选定物品
        else{
            if( (mouse_x >= 40) && (mouse_x <= 570) && (mouse_z >= 40) && (mouse_z <= 350)){
                line = (mouse_x - 40)/100;
                row = (mouse_z - 40)/100;
                handitem = line + row * 5;
                qDebug() << handitem;
            }
            //若打开了箱子
            if(box_page && ((mouse_x >= 700) && (mouse_x <= 1230) && (mouse_z >= 40) && (mouse_z <= 350))){
                line = (mouse_x - 700)/100;
                row = (mouse_z - 40)/100;
                handitem = line + row * 5 + 15;

            }
        }

    }
    //物品合成
    else if(synthesis){

    }
    else{//正常游戏操作
        int x = floor(translate_Pos_x(mouse_x,current_player));
        int chunk = translate_ChunkId(mouse_x,current_player);
        int z = floor(translate_Pos_z(mouse_z,current_player));

        if(Accessible && (event->button() == Qt::LeftButton)){
            //先判断是否是箱子，先处理箱子
            if(chunk == current_player->Chunk_x){
                if(current_player->pId->Blocks[x][z] == 7){
                    box_name = Creat_Box_Name(x,z,chunk);
                    box_path = Creat_Box_Path(WorldName,box_name);
                    box_destroy(box_path,BOX,current_player);
                }
            }else if(chunk == (current_player->Chunk_x - 1)){
                if(current_player->pId->LeftChunk->Blocks[x][z] == 7){
                    box_name = Creat_Box_Name(x,z,chunk);
                    box_path = Creat_Box_Path(WorldName,box_name);
                    box_destroy(box_path,BOX,current_player);

                }
            }else if(chunk == (current_player->Chunk_x + 1)){
                if(current_player->pId->RightChunk->Blocks[x][z] == 7){
                    box_name = Creat_Box_Name(x,z,chunk);
                    box_path = Creat_Box_Path(WorldName,box_name);
                    box_destroy(box_path,BOX,current_player);

                }
            }



            //再计算破坏
            if(chunk == current_player->Chunk_x){

                BREAK(current_player->pId->Blocks[x][z],current_player,1);
                current_player->pId->Blocks[x][z] = 0;

            }else if(chunk == (current_player->Chunk_x - 1)){

                BREAK(current_player->pId->LeftChunk->Blocks[x][z],current_player,1);
                current_player->pId->LeftChunk->Blocks[x][z] = 0;

            }else if(chunk == (current_player->Chunk_x + 1)){

                BREAK(current_player->pId->RightChunk->Blocks[x][z],current_player,1);
                current_player->pId->RightChunk->Blocks[x][z] = 0;

            }
        }
        if(Accessible && (event->button() == Qt::RightButton) /*&&
                (current_player->Player_Backpack[handitem].Item_type != Sword)*/){
            //先判断是否是箱子
            if(chunk == current_player->Chunk_x){
                if(current_player->pId->Blocks[x][z] == 7){
                    Box_Open(x,z,chunk,WorldName,&box_path,BOX);
                    backpack = true;
                    box_page = true;
                }
            }else if(chunk == (current_player->Chunk_x - 1)){
                if(current_player->pId->LeftChunk->Blocks[x][z] == 7){
                    Box_Open(x,z,chunk,WorldName,&box_path,BOX);
                    backpack = true;
                    box_page = true;
                }
            }else if(chunk == (current_player->Chunk_x + 1)){
                if(current_player->pId->RightChunk->Blocks[x][z] == 7){
                    Box_Open(x,z,chunk,WorldName,&box_path,BOX);
                    backpack = true;
                    box_page = true;
                }
            }

            //再判断是否是机床
            if(chunk == current_player->Chunk_x){
                if(current_player->pId->Blocks[x][z] == 3){
                    synthesis = true;
                    SEARCH(current_player);
                }
            }else if(chunk == (current_player->Chunk_x - 1)){
                if(current_player->pId->LeftChunk->Blocks[x][z] == 3){
                    synthesis = true;
                    SEARCH(current_player);
                }
            }else if(chunk == (current_player->Chunk_x + 1)){
                if(current_player->pId->RightChunk->Blocks[x][z] == 3){
                    synthesis = true;
                    SEARCH(current_player);
                }
            }

            //再判断是否能放置
            if((item[current_player->Player_Backpack[handitem].Item_Num].put_cube) &&
                    (current_player->Player_Backpack[handitem].Quantity > 0)){
                if(is_place(chunk,current_player->pId,x,z,0) && (!current_player->isinbox(x,z,chunk))/*If_placeable(current_player,x,z,chunk)*/){
                    if(chunk == current_player->Chunk_x){
                        current_player->pId->Blocks[x][z] =
                                current_player->Player_Backpack[handitem].Item_Num;
                        PUT(current_player->Player_Backpack[handitem].Item_Num,current_player,1);
                    }else if(chunk == (current_player->Chunk_x - 1)){
                        current_player->pId->LeftChunk->Blocks[x][z] =
                                current_player->Player_Backpack[handitem].Item_Num;
                        PUT(current_player->Player_Backpack[handitem].Item_Num,current_player,1);
                    }else if(chunk == (current_player->Chunk_x + 1)){
                        current_player->pId->RightChunk->Blocks[x][z] =
                                current_player->Player_Backpack[handitem].Item_Num;
                        PUT(current_player->Player_Backpack[handitem].Item_Num,current_player,1);
                    }
                }
                Refreash();
            }

        }
    }


}

//****************************************************************************************************
//                                               其他槽函数
//地图线程完成
void Widget::on_MapT_Finished(){
    startGame();
    //游戏线程开始
//    GameT->iniThread(Map,current_player);
//    GameT->start();
    //先绘制界面
    //repaint();
}
//游戏线程一轮计算完成*********************************暂时没用
void Widget::on_GameT_Finished(){
    repaint();
}
//游戏线程开始一轮计算*********************************暂时没用
void Widget::on_GameT_Begin(){
    //GameT->setKeyStatus(A_Status,D_Status,Space_Status);
    if(((A_Status == false)&&(D_Status == false))||(A_Status && D_Status)){
        //current_player->Move_GroundWalk(0, Space_Status);
        if(current_player->Facing == Back){
            delete player_m;
            player_m = new QMovie(PlayerL_Path);
            ui->astronaut_player->setMovie(player_m);
            player_m->start();
        }else{
            delete player_m;
            player_m = new QMovie(PlayerR_Path);
            ui->astronaut_player->setMovie(player_m);
            player_m->start();
        }
    }
    else{
        if(A_Status || D_Status){
            //qDebug() << "走";
            if(A_Status){
                //current_player->Move_GroundWalk(-1, Space_Status);
                current_player->Facing = Back;
                delete player_m;
                player_m = new QMovie(PlayerWL_Path);
                ui->astronaut_player->setMovie(player_m);
                player_m->start();

            }
            else{
                //current_player->Move_GroundWalk(1, Space_Status);
                current_player->Facing = Front;
                delete player_m;
                player_m = new QMovie(PlayerWR_Path);
                ui->astronaut_player->setMovie(player_m);
                player_m->start();

            }
        }
    }
    //qDebug() << "传入信息" << GameT->i << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd");
}
//****************************************************************************************************
//                                          按钮槽函数
//当退出键按下
void Widget::on_Exit_Button_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    this->close();
}

//当单人玩家按钮被按下
void Widget::on_Single_Button_clicked()
{
    //检查文件夹saves是否存在

    ui->stackedWidget->setCurrentWidget(ui->single_player);
    WorldName = GetNthMap(Nth_Map);
    qDebug() << WorldName;

    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    if(player_name == ""){
        player_name = "Player";
    }
    ui->showmap_label->setText(WorldName);
}

//当返回键被按下
void Widget::on_return_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->lobby);
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
}

//当创建世界按钮被按下
void Widget::on_creat_world_clicked()
{

    ui->stackedWidget->setCurrentWidget(ui->Create_map);
    delete m;
    m = new QMovie("../res/videos/bgi_2.gif");
    ui->bgi->setScaledContents(true);
    ui->bgi->setMovie(m);
    m->start();

    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }


}

//当取消创建被按下
void Widget::on_Cancel_Creating_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    ui->stackedWidget->setCurrentWidget(ui->single_player);
    ui->seed_lineEdit->setText("");
    ui->worldname_lineEdit->setText("");
    delete m;
    m = new QMovie("../res/videos/PlanetEarth.gif");
    ui->bgi->setScaledContents(true);
    ui->bgi->setMovie(m);
    m->start();
}
//当创建世界按钮（已输入种子，世界名）被按下
void Widget::on_Begin_Creat_World_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    //首先传递名称和种子
    //先判断是否为空,为空调出对话框，不为空执行创建世界
    if(ui->worldname_lineEdit->text().isEmpty() || ui->seed_lineEdit->text().isEmpty()){
        if(ui->worldname_lineEdit->text().isEmpty()){
             ui->emptyworldname->setText("世界名不能为空!");
         }else{
            ui->emptyworldname->setText("");
         }
         if(ui->seed_lineEdit->text().isEmpty()){
             ui->emptyseed->setText("种子不能为空!");
         }else{
             ui->emptyseed->setText("");
         }
         return;
    }
    WorldName = ui->worldname_lineEdit->text();
    Seed = (ui->seed_lineEdit->text().toLong());
    //当种子和世界名都不为空时，调出加载页面
    ui->stackedWidget->setCurrentWidget(ui->Loading_Page);
    ui->loading_label->setText("生成世界中...");
    ui->loading_label->setStyleSheet("QLabel{color:white;}");
    //创建世界
    CreateNewMap(WorldName,Seed);
    //sleep
    QTime t;
    t.start();
    while(t.elapsed() < 2000)
    {
        QCoreApplication::processEvents();
    }

    //清空行编辑器
    ui->seed_lineEdit->setText("");
    ui->worldname_lineEdit->setText("");
//    delete m;
//    m = NULL;

    //先加载玩家
    current_player = new PLAYER;
    if(!current_player->LoadPlayer(WorldName)){
        qDebug() << "加载失败";
    }
    current_item = new Item;
//    if(!LoadItem(current_player->Player_Backpack->Item_Num,current_item)){
//        qDebug() << "创建失败";
//    }
    //设置玩家位置
    current_player->pId = new MapChunk;
    current_player->Position_x = 0;
    current_player->Position_z = 10;
    current_player->Chunk_x = 0;

    //开始线程处理
    MapT->map = new GenerateMap(1,WorldName,Seed);     //给map开辟内存
    Map = MapT->map;
    MapT->player = current_player;
    MapT->getexistance(false);

    //新建玩家
    current_player->CreatPath(WorldName);
    current_player->Player_Name = player_name;
    current_player->SavePlayer();
    //开始线程
    MapT->start();
    //this->setStyleSheet("QWidget{background-color:black}");    //设置黑色背景


}
//当进入世界按钮被按下
void Widget::on_enter_world_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    ui->stackedWidget->setCurrentWidget(ui->Loading_Page);
    ui->loading_label->setText("进入世界中...");
    ui->loading_label->setStyleSheet("QLabel{color:white;}");
    //进入世界
    //先加载玩家
    current_player = new PLAYER;
    current_player->CreatPath(WorldName);
    if(!current_player->LoadPlayer(WorldName)){
        qDebug() << "创建失败";
    }

    //==================================debug
    int i;
    for (i=0;i<15;i++) {
        qDebug() << "第" << i << "格背包" << current_player->Player_Backpack[i].Item_Num
                 << "数量" << current_player->Player_Backpack[i].Quantity;
    }
    //===================================

    current_item = new Item;
//    if(!LoadItem(current_player->Player_Backpack->Item_Num,current_item)){
//        qDebug() << "创建物品失败";
//    }
    //+++++++++++++++++++++++++++++++++++=============================================
    current_player->pId = new MapChunk;
    current_player->Position_x = 0;
    current_player->Position_z = 10;
    current_player->Chunk_x = 0;

    //Seed = 4183201; //先设一个后期删++++++++++++++++++++++++++++=======================
    //加载世界
                                  //建立线程
    MapT->map = new GenerateMap(1,WorldName,Seed);     //给map开辟内存
    Map = MapT->map;
    MapT->player = current_player;
    MapT->getexistance(true);


    //开始线程
    MapT->start();
    //this->setStyleSheet("QWidget{background-color:black}");    //设置黑色背景




}
//当对话框的否被按下
void Widget::on_no_button_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    ui->sure_widget->hide();
}
//当删除世界被按下
void Widget::on_delete_world_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    ui->sure_label->setText("确认删除？");
    ui->sure_widget->show();
    ui->yes_button->setText("是");
    ui->no_button->setText("否");
    Dialog_status = 0;
}
//当对话框的是   确认删除被按下
void Widget::on_yes_button_clicked()
{
    switch(Dialog_status){
    case 0 ://删除世界
    {
        ui->sure_widget->hide();
        ui->stackedWidget->setCurrentWidget(ui->Loading_Page);
        ui->loading_label->setText("正在删除世界...");
        //下面执行删除世界的操作
        if(NumOfMaps() == 0){
            ui->loading_label->setText("无世界，无法删除");
        }else{
            bool flag = DeleteFolder(WorldName);
            WorldName = GetNthMap(Nth_Map);
            ui->showmap_label->setText(WorldName);
        }
        QTime t;
        t.start();
        while(t.elapsed() < 3000)
        {
            QCoreApplication::processEvents();
        }
        ui->stackedWidget->setCurrentWidget(ui->single_player);
        break;
    }
    case 1:
        break;
    default:
        break;


    }

    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }


}
//世界向左翻转按钮
void Widget::on_left_map_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    Nth_Map--;
    WorldName = GetNthMap(Nth_Map);
    //qDebug() << world;
    ui->showmap_label->setText(WorldName);
}
//世界向右翻转按钮
void Widget::on_right_map_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    Nth_Map++;
    WorldName = GetNthMap(Nth_Map);
    //qDebug() << world;
    ui->showmap_label->setText(WorldName);
}
//设置中的返回菜单
void Widget::on_return_fromsetting_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    if(ui->playername_lineEdit->text() != ""){
        player_name = "Player";
    }
    player_name = ui->playername_lineEdit->text();
    ui->stackedWidget->setCurrentWidget(ui->lobby);
}
//主页面中的进入设置
void Widget::on_setting_Button_clicked()
{
    if(soundeffect){
        QSound::play("../res/audios/buttonsound.wav");
    }
    ui->stackedWidget->setCurrentWidget(ui->setting_page);
}

//设置中的音效按钮
void Widget::on_soundeffect_yes_radioButton_clicked()
{
    soundeffect = true;
}
void Widget::on_soundeffect_no_radioButton_clicked()
{
    soundeffect = false;
}
//****************************************************************************************************
//                                                  其他函数

//====================================    坐标转换    ===========================================

float Widget::translate_Pos_x(float mouse_x,  PLAYER *player){
    int dis_x = mouse_x - Screen_Width/2;
    float mouse_Position_x = (((float)(dis_x))/BASIS_PX);
    mouse_Position_x += player->Position_x;
    if(mouse_Position_x >= 0){
        return ((mouse_Position_x-((int)(mouse_Position_x)))+((int)(mouse_Position_x))%CHUNK_WIDTH);
    }else{
        return ((mouse_Position_x-((int)(mouse_Position_x)))+((int)(mouse_Position_x))%CHUNK_WIDTH)+16;
    }

}

float Widget::translate_Pos_z( float mouse_z, PLAYER *player){
    int dis_z = mouse_z - (Screen_Height/2);
    float mouse_Position_z = (((float)(dis_z))/BASIS_PX);
    mouse_Position_z += player->Position_z;
    if(mouse_Position_z > CHUNK_HEIGHT){
        return 100.0;
    }else if(mouse_Position_z < 0){
        return 0.0;
    }
    return mouse_Position_z;
}

int Widget::translate_ChunkId(float mouse_x, PLAYER *player){
    int dis_x = mouse_x - (Screen_Width/2);
    float mouse_Position_x = (((float)(dis_x))/BASIS_PX);
    mouse_Position_x += player->Position_x;

    return floor((mouse_Position_x)/CHUNK_WIDTH) + player->Chunk_x;
//    if(mouse_Position_x >= 0){
//        return (((int)(mouse_Position_x))/CHUNK_WIDTH)+player->Chunk_x;
//    }else{
//        return (((int)(mouse_Position_x))/CHUNK_WIDTH)+player->Chunk_x + 1;
//    }
}

int Widget::translate_Screen_x(int Block_Pos_x , int Chunk_Id , PLAYER *player){

    Block_Pos_x += CHUNK_WIDTH * (Chunk_Id - player->Chunk_x);
    //qDebug() << CHUNK_WIDTH * (Chunk_Id - player->Chunk_x);
    float Relative_Pos_x = Block_Pos_x - player->Position_x;
    return (int)(Relative_Pos_x * BASIS_PX + Screen_Width/2);
}

int Widget::translate_Screen_z(int Block_Pos_z, PLAYER *player){

    float Relative_Pos_z = Block_Pos_z - player->Position_z;
    return (int)(Relative_Pos_z * BASIS_PX + Screen_Height/2);
}

//放置函数 fjy ===========================================
bool Widget::is_place(int CurrentChunk, MapChunk *pId, int x, int z , int depth){
    if(depth == 0){
        if(CurrentChunk != pId->ChunkId){
            if(CurrentChunk > pId->ChunkId){
                pId = pId->RightChunk;

            }else if(CurrentChunk < pId->ChunkId){

                pId = pId->LeftChunk;

            }
        }
        if(pId->Blocks[x][z] != 0){
            return false;
        }else{
            is_place(CurrentChunk,pId,x-1,z,1);
            is_place(CurrentChunk,pId,x,z-1,2);
            is_place(CurrentChunk,pId,x+1,z,3);
            is_place(CurrentChunk,pId,x,z+1,4);
        }

    }else{
        if(x >= 16){
            pId = pId->RightChunk;
            x -= 16;
        }else if( x < 0){
            //qDebug() << "执行";
            pId = pId->LeftChunk;
            x += 16;
        }


        //qDebug() << x << z << CurrentChunk << pId->ChunkId;

        bool b;
        if(z == 99){
            b = true;
        }else if(z == -1){
            b = false;
        }else{
            if(pId->Blocks[x][z] != 0){
                b = true;
            }else{
                b = false;
            }
        }

        switch (depth) {
        case 1:{
            place1 = b;
            //qDebug() << 1 << (place1||place2||place3||place4);
            return (place1||place2||place3||place4);
            break;
        }
        case 2:{
            place2 = b;
            //qDebug() << 2 << (place1||place2||place3||place4);
            return (place1||place2||place3||place4);
            break;
        }
        case 3:{
            place3 = b;
            //qDebug() << 3 << (place1||place2||place3||place4);
            return (place1||place2||place3||place4);
            break;
        }
        case 4:{
            place4 = b;
            //qDebug() << 4 << (place1||place2||place3||place4);
            return (place1||place2||place3||place4);
            break;
        }
        }

    }
}

void Widget::Refreash(){
    place1 = false;
    place2 = false;
    place3 = false;
    place4 = false;
}

//放置函数 tzy ===========================================
bool Widget::If_placeable(PLAYER *player, int x, int z, int ChunkId)
{
    MapChunk * pId = player->pId;
    if(z==99)
    {
        return true;
    }
    else
    {
        if(pId->ChunkId == ChunkId)
        {
            //如果在本区块
            if(x==0 && z==0)
            {
                if((pId->LeftChunk->Blocks[15][0]!=0)||(pId->Blocks[0][1]!=0)||(pId->Blocks[1][0]!=0))
                {
                    return true;
                }
                else return false;
            }
            else if(x==15 && z==0)
            {
                if((pId->RightChunk->Blocks[0][0]!=0)||(pId->Blocks[15][1]!=0)||(pId->Blocks[14][0]!=0))
                {
                    return true;
                }
                else return false;
            }
            else if(z==0)
            {
                if((pId->Blocks[x-1][0]!=0)||(pId->Blocks[x][1]!=0)||(pId->Blocks[x+1][0]!=0))
                {
                    return true;
                }
                else return false;
            }
            else if(x==0)
            {
                if((pId->LeftChunk->Blocks[15][z]!=0)||((pId)->Blocks[0][z-1]!=0)||((pId)->Blocks[0][z+1]!=0)||(pId->Blocks[1][z]!=0))
                {
                    return true;
                }
                else return false;
            }
            else if(x==15)
            {
                if((pId->RightChunk->Blocks[0][z]!=0)||((pId)->Blocks[15][z-1]!=0)||((pId)->Blocks[15][z+1]!=0)||(pId->Blocks[14][z]!=0))
                {
                    return true;
                }
                else return false;
            }
            else
            {
                if((pId->Blocks[x-1][z]!=0)||(pId->Blocks[x+1][z]!=0)||(pId->Blocks[x][z-1]!=0)||(pId->Blocks[x][z+1]!=0))
                {
                    return true;
                }
                else return false ;
            }
        }
        else if(pId->ChunkId == ChunkId + 1)//在左边
        {
            if(x==15 && z==0)
            {
                if((pId->Blocks[0][0]!=0)||(pId->LeftChunk->Blocks[15][1]!=0)||(pId->LeftChunk->Blocks[14][0]!=0))
                {
                    return true;
                }
                else return false;
            }
            else if(x==15)
            {
                if((pId->Blocks[0][z]!=0)||(pId->LeftChunk->Blocks[15][z+1]!=0)||(pId->LeftChunk->Blocks[14][z]!=0)||(pId->LeftChunk->Blocks[15][z-1]!=0))
                {
                    return true;
                }
                else return false;
            }
            else
            {
                if((pId->LeftChunk->Blocks[x][z+1]!=0)||(pId->LeftChunk->Blocks[x][z-1]!=0)||(pId->LeftChunk->Blocks[x+1][z]!=0)||(pId->LeftChunk->Blocks[x-1][z]!=0))
                {
                    return true;
                }
                else return false;
            }

        }
        else //在右边
        {
            if(x==0&&z==0)
            {
                if((pId->Blocks[15][0]!=0)||(pId->RightChunk->Blocks[1][0]!=0)||(pId->RightChunk->Blocks[0][1]!=0))
                {
                    return true;
                }
                else return false;
            }
            else if(x==0)
            {
                if((pId->Blocks[15][z]!=0)||(pId->RightChunk->Blocks[0][z-1]!=0)||(pId->RightChunk->Blocks[0][z+1]!=0)||(pId->RightChunk->Blocks[1][z]!=0))
                {
                    return true;
                }
                else return false;
            }
            else
            {
                if((pId->RightChunk->Blocks[x][z-1]!=0)||(pId->RightChunk->Blocks[x][z+1]!=0)||(pId->RightChunk->Blocks[x-1][z]!=0)||(pId->RightChunk->Blocks[x+1][z]!=0))
                {
                    return true;
                }
                else return false;
            }
        }
    }
    return true;
}

//========================================== 帧率计算 ====================================
void Widget::get_interval(){                                 //将间隔插入数组
    Last_time = Current_time;
    Current_time = QDateTime::currentDateTime().toString("zzz").toInt();
    int Interval;
    if(Last_time > Current_time){
        Interval = Current_time + 1000 - Last_time;
    }else{
        Interval = Current_time - Last_time;
    }
    //qDebug() << "两帧间隔" << Interval;
    frame_interval[frame_state] = Interval;
    frame_state ++;
    frame_state = frame_state % 20;
}

void Widget::frameRate(){
    get_interval();
    float Rate = 0;
    int i;
    if(frame_state == 0){
        for (i=0;i<20;i++) {
            Rate += frame_interval[i];
        }
        if(Rate <= 0){
            Rate = 0;
        }else{
            Rate = (1000 * 20) / Rate;
        }

        QString title = Game_Name;
        title.append(" 实时帧率:");
        title.append(QString::number(Rate));
        title.append(" fps");
        this->setWindowTitle(title);
    }
}

//========================================== 地图贴图 ====================================

QString Widget::Select_CubePath(int CubeNum){
    switch (CubeNum) {
    case 1 :{
        return Earth_cube_image;
        break;
        }

    case 2:{
        return Crystal_cube_image;
        break;
        }
    case 3:{
        return Lathe_image;
        break;
    }
    case 5:{
        return Collecting_mach;
        break;
        }
    case 7:{
        return Box_cube_image;
        break;
        }

    case 25:{
        return Meterorite_cube_image;
        break;
        }

    case 26:{
        return Iron_cube_image;
        break;
        }
    default:
        return "";
    }
}

QString Widget::Select_IconPath(int IconNum){
    switch (IconNum) {
    case 1 :{
        return Earth_cube_image;
        break;
        }

    case 2:{
        return Crystal_Icon_image;
        break;
        }

    case 3:{
        return Lathe_image;
        break;
    }
    case 4:{
        return Damage_lathe;
        break;
    }
    case 5:{
        return Collecting_mach;
        break;
    }
    case 6:{
        return Smelting_mach;
        break;
    }

    case 7:{
        return Box_cube_image;
        break;
        }
    case 8:{
        return Core_Image;
        break;
    }

    case 9:{
        return Part;
        break;
    }
    case 10:{
        return Iron_ingot;
        break;
    }
    case 11:{
        return Energy_core;
        break;
    }
    case 12:{
        return Laser_gun;
        break;
    }
    case 13:{
        return laser_blade;
        break;
    }
    case 14:{
        return Iron_stick;
        break;
    }
    case 15:{
        return Crystal_blade;
        break;
    }
    case 16:{
        return Crystal_gun;
    }
    case 25:{
        return Meterorite_cube_image;
        break;
        }

    case 26:{
        return Iron_cube_image;
        break;
        }
    default:
        return "";
    }
}

void Widget::startGame(){
    if(isGame){
        return;
    }

    //创建箱子文件夹
    QString path = "../saves/";
    path.append(WorldName);
    QDir* dir = new QDir(path);
    if(dir->exists("BOX") == false){
        dir->mkdir("BOX");
    }
    delete dir;

    //隐藏stackedwidget
    ui->bgi->hide();
    ui->stackedWidget->hide();

    //显示玩家
    ui->astronaut_player->show();
    ui->player_name->show();

    //如果存档里玩家无名称，则自动为默认名称
    if(current_player->Player_Name == ""){
        current_player->Player_Name = player_name;
    }
    ui->player_name->setText(current_player->Player_Name);
    //===========================================
    //保存玩家（测试）


//    if(current_player->SavePlayer()){
//        qDebug() << "保存玩家成功";
//    }else{
//        qDebug() << "保存玩家失败";
//    }
    //===========================================
    //播放玩家动画
    //delete m;
    player_m = new QMovie(PlayerR_Path);
    ui->astronaut_player->setMovie(player_m);
    player_m->start();
    //开始计时器
    timer_ID = this->startTimer(10);
    isGame = true;
    qDebug() << "开始游戏" ;
    ui->astronaut_player->setStyleSheet("QLabel{background:transparent}");
    ui->player_name->setStyleSheet("QLabel{background:transparent;color:white}");

    //===================================================用于物品测试
    Load_Item(&current_player->Player_Backpack[0],26,10);
    Load_Item(&current_player->Player_Backpack[1],1,10);
    Load_Item(&current_player->Player_Backpack[2],2,10);
    Load_Item(&current_player->Player_Backpack[3],25,10);
    Load_Item(&current_player->Player_Backpack[4],5,10);
    Load_Item(&current_player->Player_Backpack[5],7,10);
    Load_Item(&current_player->Player_Backpack[6],3,10);
    Load_Item(&current_player->Player_Backpack[7],10,1000);
    Load_Item(&current_player->Player_Backpack[8],8,100);
    Load_Item(&current_player->Player_Backpack[9],9,100);
    //===================================================

    delete m;
    m = NULL;
}

void Widget::pauseGame(){
    if(!isGame){
        return;
    }
    isGame = false;
    A_Status = false;
    D_Status = false;
    Space_Status = false;
    //pausepaint = true;
    ui->player_name->hide();
    ui->astronaut_player->hide();
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentWidget(ui->pause_Page);
    blureffect = new QGraphicsBlurEffect(this);
    blureffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    blureffect->setBlurRadius(20);
    ui->bgi->setGraphicsEffect(blureffect);
    ui->bgi->show();
    ui->bgi->setPixmap(QPixmap(Synbgi_Path));
    qDebug() << "pause!!";

}

void Widget::resumeGame(){

    if(isGame){
        return;
    }
    isGame = true;
    //pausepaint = false;
    ui->player_name->show();
    ui->astronaut_player->show();
    ui->stackedWidget->hide();
    ui->bgi->hide();
    delete blureffect;

}

void Widget::exitGame(){
    //保存玩家
    if(!isGame){
        return;
    }
    if(current_player->SavePlayer()){
        qDebug() << "存储玩家成功";
    }else{
        qDebug() << "存储玩家失败";
    }
    //游戏状态为F
    isGame = false;
    //定时器关闭
    this->killTimer(timer_ID);
    //保存玩家区块
    Map->SaveChunks(&(current_player->pId));
    //玩家及玩家名label隐藏
    ui->player_name->hide();
    ui->astronaut_player->hide();
    //主菜单展示
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentWidget(ui->lobby);

    delete Map;
    delete current_player;
    delete player_m;
    Map = NULL;
    current_player = NULL;
    player_m = NULL;

    //背景gif显示
//    delete m;
    m = new QMovie(":/new/background_images/res/videos/PlanetEarth.gif");
    ui->bgi->setMovie(m);
    m->start();
    ui->bgi->show();
    this->setWindowTitle(Game_Name);

}


//*********************物品************************

//物品切换
void Widget::Switch_Item(Container *firstitem, Container *seconditem){
    Container tool;
    if(firstitem->Item_Num == seconditem->Item_Num){
        seconditem->Quantity += firstitem->Quantity;
        firstitem->Item_Num = 0;
        firstitem->Quantity = 0;
        firstitem->Item_type = Null;
    }else{
        tool.Item_Num = firstitem->Item_Num;
        tool.Quantity = firstitem->Quantity;
        tool.Item_type = firstitem->Item_type;

        firstitem->Item_Num = seconditem->Item_Num;
        firstitem->Quantity = seconditem->Quantity;
        firstitem->Item_type = seconditem->Item_type;

        seconditem->Item_Num = tool.Item_Num;
        seconditem->Quantity = tool.Quantity;
        seconditem->Item_type = tool.Item_type;
    }


}

//物品破坏
//当方块被破坏时背包的物品数量+1
void Widget::BREAK(int item_num, PLAYER *player,int num){
    int i;
    for(i=0;i<15;i++){
        if(player->Player_Backpack[i].Item_Num == item_num){
            player->Player_Backpack[i].Quantity += num;
            break;
        }else if (player->Player_Backpack[i].Item_Num == 0) {
            player->Player_Backpack[i].Item_Num = item_num;
            player->Player_Backpack[i].Quantity = num;

        }

    }
}

//物品放置
//当方块被放置时其数量减一
void Widget::PUT(int item_num, PLAYER *player,int num){
    int i;
    if(item_num == 0 || item_num == 1000){
        return;
    }
    for(i=0;i<15;i++){
        if(player->Player_Backpack[i].Item_Num==item_num){
            player->Player_Backpack[i].Quantity-=num;
            break;
        }else if (player->Player_Backpack[i].Item_Num==0) {
            break;
        }

    }
}

//物品合成函数
//遍历背包将材料储存至material中
void Widget::SEARCH(PLAYER *player){
    int i,t;
    for (i=0;i<5;i++) {
        material[i].Item_Num = 0;
        material[i].Quantity = 0;
        material[i].Item_type = Null;
    }
    for(i=0;i<15;i++){
            if(player->Player_Backpack[i].Item_type==Material){
               for(t=0;t<5;t++){
                   if(material[t].Item_Num==player->Player_Backpack[i].Item_Num){
                       material[t].Quantity=player->Player_Backpack[i].Quantity;
                       break;
                   }
                   else if(material[t].Item_Num==0){
                       material[t].Item_Num=player->Player_Backpack[i].Item_Num;
                       material[t].Quantity=player->Player_Backpack[i].Quantity;
                       break;
                   }
               }
        }
    }
};

//判断是否能够合成
bool Widget::Synthesis(int line)
{
    int t,n;
    //bool flag = false;
    for(n=0;n<4;n++)
    {
        if(Synthesis_List[line].synthesis_item[n].Item_Num == 0){
            continue;
        }
        for(t=0;t<5;t++)
        {

            if(Synthesis_List[line].synthesis_item[n].Item_Num == material[t].Item_Num)
            {
                if(Synthesis_List[line].synthesis_item[n].Quantity > material[t].Quantity)
                {
                    return false;
                }
                else{

                    break;
                }

            }
            if(t == 4){
                return false;
            }

        }
    }
    return true;
};

//合成操作
void Widget::SYNTHESIS(int line,PLAYER *player)
{
    bool a = Synthesis(line);
    int i , n , t , require;
    qDebug() << "进入合成函数" << a;
    if(a)
    {
        for(n=0;n<4;n++)
        {
            if(Synthesis_List[line].synthesis_item[n].Item_Num != 0){
                  require = Synthesis_List[line].synthesis_item[n].Quantity;
            }else{
                continue;
            }

            for(i=0;i<15;i++)
            {
                if(Synthesis_List[line].synthesis_item[n].Item_Num == player->Player_Backpack[i].Item_Num)
                {
                    do{
                        require--;
                        player->Player_Backpack[i].Quantity--;
                    }while ((require > 0) && (player->Player_Backpack[i].Quantity > 0));
                    if(require > 0){
                        continue;
                    }else{
                        break;
                    }

                }
            }
        }
        for (i=0;i<15;i++) {
            if(Synthesis_List[line].product == player->Player_Backpack[i].Item_Num){
                player->Player_Backpack[i].Quantity++;
                player->Player_Backpack[i].Item_type = item[player->Player_Backpack[i].Item_Num].Item_type;
                break;
            }else if( player->Player_Backpack[i].Item_Num == 0){
                player->Player_Backpack[i].Item_Num = Synthesis_List[line].product;
                player->Player_Backpack[i].Quantity++;
                player->Player_Backpack[i].Item_type = item[player->Player_Backpack[i].Item_Num].Item_type;
                break;
            }
        }
    }
};








//物品合成gui以及鼠标事件




//鼠标事件，导入物品信息，
//箱子内物品的增减,按一次一个。



//箱子破坏，里头的物品先全部进入背包，多余物品X掉，删除文件。


//背包gui






