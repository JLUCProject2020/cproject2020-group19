#ifndef GAMEFRAMEWORK_H
#define GAMEFRAMEWORK_H
#include<io.h>
#include<string>
#include<fstream>
#include<iostream>
#include<direct.h>
#include<windows.h>

using namespace std;


#define SAVES_PATH "../saves"

#define DIM_PATH "../saves/world/DIM_0"					// 0世界路径
#define CHUNK_PATH_P "../saves/world/DIM_0/chunkp.dat"	//正坐标存档
#define CHUNK_PATH_N "../saves/world/DIM_0/chunkn.dat"	//负坐标存档



extern short int GameGUIMode;
extern short int GameMode;

extern char PlayerName[20];
extern char MapName[30];

typedef struct MAPINFO
{
    char MapName[30];
    int Mapseed;
} MAPINFO;


typedef enum GAMEGUIMODE
{
    MODE_LOBBY = 0,
    MODE_LOADING = 1,
    MODE_LOADFAIL = 2,

    MODE_SINGLE = 10,
    MODE_CREATENEWWORLD = 11,
    MODE_EDITWORLD = 12,
    MODE_IFDELETE = 13,
    MODE_CREATEFAIL = 14,
    MODE_READFAIL = 15,

    MODE_MULTI = 20,
    MODE_HOST = 21,
    MODE_JOINMULTI = 22,
    MODE_JOINFAIL = 23,
    MODE_CONFIG = 30,

    MODE_GAMING = 99
} GAMEGUIMODE;

typedef enum GAMEMODE {
    NONGAME=0,
    GAMING=1
} GAMEMODE;

//===========================模式判断函数===================================//

void ModeGUIFunction();

void ModeFunction();

//===========================单人游戏GUI====================================//

void GameLobbyGUI();//游戏大厅

void SinglePlayerModeGUI();//单人游戏模式GUI

void JoiningGameGUI();

void CreateNewWorldGUI();//创建新的世界，界面设置,需要一些参数

void CreateFailGUI();//加载存档和创建游戏存档失败

void DeleteTheWorldGUI(string FolderPath);

void IfDeleteWorld();//是否删除存档

void EditWorldGUI();

//===========================单人游戏功能性函数===============================//

bool ListAllWorlds(int NthMap,MAPINFO * Imap);

bool IfisMap(string WorldPath);

int NumbersofMaps();//一个文件夹里有多少个地图文件

void CreateNewWorld();

void DeleteTheWorld(MAPINFO * Imap);//删除已有世界,功能性//删除世界的FolderPath如下获得

void EditTheWorld(MAPINFO * Imap);//编辑选中世界 编辑世界的信息 Widget!!

void JoinTheWorld(MAPINFO * Imap);//进入游戏，功能性,单人游戏

void GameConfig();//游戏设置，在游戏内的渲染设置

int CreateNewFolder(string folderPath);//创建文件夹,direct.h and iostream

int DeleteFolder(string folderpath);//删除文件夹

int RenameFolder(string original_name,string new_name);//文件夹重命名

void MainGameGUI();//主游戏gui


#endif // GAMEFRAMEWORK_H
