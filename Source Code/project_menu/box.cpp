#include "box.h"


//*************箱子*************************
//箱子打开
//首先将在创建地图时设置箱子文件夹，箱子坐标转换成箱子文件名，生成路径读取文件，结合GUI

//创建箱子名
QString Creat_Box_Name(int x,int y,int n)
{
    QString box_name = "";
    box_name.append(QString::number(x));
    box_name.append("_");
    box_name.append(QString::number(y));
    box_name.append("_");
    box_name.append(QString::number(n));
    box_name.append(".txt");
    return box_name;
}

//创造箱子路径
QString Creat_Box_Path(QString worldname,QString box_name)
{

    QString box_path = "../saves/" ;
    box_path.append(worldname);
    box_path.append("/");
    box_path.append("BOX/");
    box_path.append(box_name);
    return box_path;
}

//打开箱子文件
bool Box_Open(int x,int y,int chunk_num,QString worldname,QString *box_path,Container *BOX)
{
    int i;
    bool Readflag;//真为读模式，假为写模式

    //先创建文件路径及名称
    QString box_name = Creat_Box_Name(x,y,chunk_num);
    *box_path = Creat_Box_Path(worldname,box_name);


    //先检测文件信息
    QFileInfo FILE(*box_path);
    QFile file(*box_path);
    bool is_Open;
    if((!FILE.exists())||(FILE.size() < static_cast<qint64>((sizeof (Container)) * 15))){
        //无有效箱子文件
        qDebug() << "文件不存在或损坏，重新创建文件";
        //现存箱子归零
        for (i=0;i<15;i++) {
            BOX[i].Item_Num=0;
            BOX[i].Quantity=0;
            BOX[i].Item_type=Null;
        }
        //以只写方式打开文件
        is_Open = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        Readflag = false;

    }else{
        //有有效的箱子文件
        //以只读方式打开文件
        is_Open = file.open(QIODevice::ReadOnly | QIODevice::Truncate);
        Readflag = true;
    }

    QDataStream stream(&file);
    int n;

    if(is_Open)
    {

        if(Readflag){
            for(n=0;n<15;n++)
            {
                stream >> BOX[n].Item_Num >> BOX[n].Quantity >>BOX[n].Item_type;   //读取文件
            }
            file.close();

            return true;
        }else{
            for(n=0;n<15;n++)
            {
                stream << BOX[n].Item_Num << BOX[n].Quantity << BOX[n].Item_type;   //写入文件
            }
            file.close();

            return true;
        }


    }


    else
    {
        qDebug() << "箱子文件创建失败";
        return false;
    }

}

//关闭箱子
bool Box_Close(QString BOX_PATH, Container *BOX){
    QFile file(BOX_PATH);
    bool isOpen = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(isOpen){
        QDataStream stream(&file);
        int n;
        for(n=0;n<15;n++)
        {
            stream << BOX[n].Item_Num << BOX[n].Quantity << BOX[n].Item_type;   //写入文件
        }
        file.close();
        return true;
    }else {
        qDebug() << "关闭时箱子保存失败";
        return false;
    }

}


//箱子销毁
bool box_destroy(QString BOX_PATH, Container *BOX, PLAYER *player)
{

    QFile file(BOX_PATH);
    bool isOpen = file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    if(isOpen)
    {
        QDataStream stream(&file);
        int n,t,x;
        for(n=0;n<15;n++)
        {
            stream >> BOX[n].Item_Num >> BOX[n].Quantity >>BOX[n].Item_type;
        }
        for(n=0;n<15;n++)
        {
            if(BOX[n].Item_Num != 0)
            {
                for(t=0;t<15;t++)
                {
                    if(player->Player_Backpack[t].Item_Num == BOX[n].Item_Num)
                    {
                        qDebug() << "将物品放入玩家背包";
                        player->Player_Backpack[t].Quantity += BOX[n].Quantity;
                        qDebug() << player->Player_Backpack[t].Quantity;
                        BOX[n].Item_Num=0;
                        BOX[n].Quantity=0;
                        BOX[n].Item_type=Null;
                        break;
                    }
                    else if(player->Player_Backpack[t].Item_Num == 0)
                    {
                        qDebug() << "将物品放入玩家背包";
                        player->Player_Backpack[t].Item_Num = BOX[n].Item_Num;
                        player->Player_Backpack[t].Quantity = BOX[n].Quantity;
                        BOX[n].Item_Num=0;
                        BOX[n].Quantity=0;
                        BOX[n].Item_type=Null;
                        break;
                    }
                    else if(t==14)
                    {
                        BOX[n].Item_Num=0;
                        BOX[n].Quantity=0;
                        BOX[n].Item_type=Null;
                    }
                }
            }
        }
        file.remove();
        file.close();
        return true;
    }
    else
    {
        qDebug() << "箱子文件删除失败";
        return false;
    }

}


