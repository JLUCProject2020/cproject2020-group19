////本文档讲编写Tangraria游戏框架
////进入游戏————选择游戏模式————单人游戏或多人游戏
////单人游戏：选择地图，选择地图后可以点击编辑地图、删除地图、进入地图
////创建地图：可以选择是否输入种子

////多人游戏：选择作主机或连接玩家
////在游戏启动时即决定玩家名称，或内置修改

////需要一个文件储存 游戏玩家偏好设置来进行不同的渲染

////我想把功能性与绘图的分开写，不知道）

//#include"gameframework.h"



////===========================模式判断函数===================================//


//void ModeGUIFunction()
//{
//    switch (GameGUIMode)
//    {
//    case 0:;//游戏大厅
//    case 1:;//加载中
//    case 2:;//加载失败
//    case 10:;//单人游戏大厅，绘制
//    case 11:;//单人游戏创建新游戏
//    case 12:;//单人游戏编辑存档
//    case 13:;//是否删除存档？
//    case 14:;//世界创建失败
//    case 15:;//存档读取失败
//    case 20:;//多人游戏大厅
//    case 21:;//多人游戏-构建主机
//    case 22:;//多人游戏-加入游戏
//    case 23:;//加入游戏失败
//    case 30:;//游戏设置,
//    case 99:;//主游戏
//    }
//}

//void ModeFunction()
//{
//    switch (GameMode)
//    {
//        case 1:;
//    }
//}



////===========================单人游戏GUI====================================//

//void GameLobbyGUI()//游戏大厅
//{
//    //绘图函数
//    //判断鼠标事件，根据不同事件修改值



//}

//void SinglePlayerModeGUI()//单人游戏模式GUI
//{
//    //绘制图像
//    //判断鼠标位置
//    //根据鼠标事件，改变GameMode的值

//    //调用一次绘制存档列表
//    //指向---创建新世界---进入存档---删除存档---编辑存档
//}

//void JoiningGameGUI()
//{

//}

//void CreateNewWorldGUI()//创建新的世界，界面设置,需要一些参数
//{
//    //留一个文本框输入字符串名字
//    //留一个文本框输入种子
//    //创建新世界！！！
//    //取消---
//}

//void CreateFailGUI()//加载存档和创建游戏存档失败
//{

//}

//void DeleteTheWorldGUI(string FolderPath)
//{

//}

//void IfDeleteWorld()//是否删除存档
//{

//}

//void EditWorldGUI()
//{

//}



////===========================单人游戏功能性函数===============================//


//bool ListAllWorlds(int NthMap,MAPINFO * Imap)//传出去的目的是打印地图
//{
//    //给出所有世界，读取特定文件夹
//    //查找路径下的所有世界
//    //绘制出来所有的世界信息
//    //在中央绘制一个地图信息
//    //左右箭头切换地图,不能点击过头
//    string path = SAVES_PATH;

//    long hFile = 0;
//    struct _finddata_t fileInfo;
//    string pathName, WorldPath;

//    int TotalMapNum = NumbersofMaps();
//    int temp=0;

//    if (TotalMapNum == 0) return false;

//    NthMap = NthMap % TotalMapNum;//同余运算

//    hFile = _findfirst(pathName.assign(path).append("\\*.").c_str(), &fileInfo);//回到了第一个文件

//    while (temp < NthMap)
//    {
//        WorldPath = WorldPath.assign(path).append("\\").append(fileInfo.name);
//        if (IfisMap(WorldPath))
//        {
//            temp++;
//        }
//        if (temp == NthMap) break;
//        else _findnext(hFile, &fileInfo);
//    }//WorldPath已有

//    string name = fileInfo.name;

//    strcpy(Imap->MapName, fileInfo.name);

//    _findclose(hFile);

//    return true;
//}

//bool IfisMap(string WorldPath)//判断一个文件夹是不是地图文件夹
//{
//    fstream fp;
//    fp.open("WorldInfo.dat", ios::in | ios::binary);
//    string wp;
//    if (!fp) return 0;
//    if (0 != access(wp.assign(WorldPath).append("\\").append("DIM0").c_str(), 0)) return 0;
//    if (0 != access(wp.assign(WorldPath).append("\\").append("playerdata").c_str(), 0)) return 0;
//    if (0 != access(wp.assign(WorldPath).append("\\").append("data").c_str(), 0)) return 0;

//    return 1;
//}

//int NumbersofMaps()//一个文件夹里有多少个地图文件
//{
//    string path = SAVES_PATH;

//    long hFile = 0;
//    struct _finddata_t fileInfo;
//    string pathName, WorldPath;

//    int TotalMapNum = 0;

//    //  \\* 代表要遍历所有的类型,如改成\\*.jpg表示遍历jpg类型文件
//    if ((hFile = _findfirst(pathName.assign(path).append("\\*.").c_str(), &fileInfo)) == -1) {
//        return 0;
//    }

//    if (fileInfo.attrib & _A_SUBDIR)
//    {
//        WorldPath = WorldPath.assign(path).append(fileInfo.name);
//        if (IfisMap(WorldPath))
//        {
//            TotalMapNum++;
//        }
//    }
//    while (_findnext(hFile, &fileInfo) == 0)//如果还有下一个文件
//    {
//        if (fileInfo.attrib & _A_SUBDIR)
//        {
//            WorldPath = WorldPath.assign(path).append("\\").append(fileInfo.name);
//            if (IfisMap(WorldPath))
//            {
//                TotalMapNum++;
//            }
//        }
//    }//求出了Maps总数
//}

//void CreateNewWorld()
//{
//    //需要记录的数据有什么呢？
//    //内存中的玩家ID

//    //1.创建区块，写入DIM0 构建世界坐标，区块内容
//    //2.玩家载入，生物刷新
//    //开始游戏绘图模式
//    //由于以区块为核心，所以可以给一个二阶指针
//    //如果没有全局的指针，那么我需要满足：切换函数绘图，同时生成区块，玩家实体

//    //玩家有一个指针是指向当前区块的，区块是一个结构体
//    //地图类中有initial区块函数
//    //先生成一个玩家实体，然后再根据玩家实体中的指针和id生成区块
//    //再本地绘图就可以了

//    //下面创建文件夹

//    char worldname;//虚假地赋值

//    string SavesPath = SAVES_PATH ; //存档路径
//    string WorldPath = WorldPath.assign(SavesPath).append("\\").append(& worldname); //加上了世界名称
//    if (CreateNewFolder(WorldPath)==-1)
//    {
//        //没有创建成功，或已经存在
//        GameGUIMode = MODE_CREATEFAIL;
//        return;
//    }
//    else
//    {
//        //创建成功了世界路径

//        ////创建DIM0
//        //if (CreateNewFolder(WorldPath.append("\\").append("DIM0")) == -1)
//        //{
//        //    GameMode = MODE_CTEATEFIAL;
//        //    return;
//        //}

//    }

//    //思考








//}

//void DeleteTheWorld(MAPINFO * Imap)//删除已有世界,功能性//删除世界的FolderPath如下获得
//{
//    //先绘图是否要删除  MODE_IFDELETE
//    //-----绘图-------//
//    //检测鼠标事件
//    string FolderPath = SAVES_PATH;

//    FolderPath.append("\\").append(Imap->MapName);

//    if (1)//<------
//    {
//        DeleteFolder(FolderPath);
//        GameGUIMode = MODE_SINGLE;
//    }
//    else if (1)
//    {
//        GameGUIMode = MODE_SINGLE;
//    }
//}

//void EditTheWorld(MAPINFO * Imap)//编辑选中世界 编辑世界的信息 Widget!!
//{
//    //先找一些变量存这些数据，这时候绘制着gui
//    //弄几个控件出来
//    char NewName[30];
//    //文本框数据读入）
//    //这些写
//    string oringinal_name = SAVES_PATH,new_name=SAVES_PATH;
//    oringinal_name.append("\\").append(Imap->MapName);
//    new_name.append("\\").append(NewName);

//    //这里控件触发
//}

//void JoinTheWorld(MAPINFO * Imap)//进入游戏，功能性,单人游戏
//{



//}

//void GameConfig()//游戏设置，在游戏内的渲染设置
//{
//    //未开发
//}

//int CreateNewFolder(string folderPath)//创建文件夹,direct.h and iostream
//{
//    if (0 != access(folderPath.c_str(), 0))
//    {
//        //如果不存在此文件夹，则创建一个.
//        int flag = mkdir(folderPath.c_str());
//        // 返回 0 表示创建成功，-1 表示失败
//        return flag;
//    }
//    else
//    {
//        return -1;
//    }
//}

//int DeleteFolder(string folderpath)//删除文件夹
//{
//    RemoveDirectory()
//   int flag = RemoveDirectory(folderpath.c_str());
//    //删除成功则返回1，失败返回0
//    return(flag-1);
//}

//int RenameFolder(string original_name,string new_name)//文件夹重命名
//{
//    return rename(original_name.c_str(), new_name.c_str());
//    //成功返回0，失败返回-1
//}

//void MainGameGUI()
//{
//    //游戏主要部分，先写在这里
//}
