#include "widget.h"
#include "MapInfo.h"

#include <QApplication>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QDirIterator>
#include <stdio.h>
#include <windows.h>




//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Widget w;
//    QString worldname ="hello";


////    bool flag = CreateNewFolder(worldname);

//    qDebug() << NumOfMaps() <<endl;

//    CreateNewMap("World311wtf",313412);
//    qDebug()<<NumOfMaps()<<endl;
////    w.show();
////    return a.exec();

//    qDebug()<<GetNthMap(2)<<endl;

//    return 0;
//}

bool CreateNewFolder(QString WorldName)//创建文件夹,direct.h and iostream
{
    QDir path = QDir(SAVES_PATH);

    if (false == path.exists(WorldName))
    {
        //若没有则创建
        int flag = path.mkdir(WorldName);
        return flag;
    }
    else
    {
        return 0;
    }
}

bool DeleteFolder(QString WorldName)
{
    QDir path = QDir(SAVES_PATH);

    path.cd(WorldName);

    if(path.removeRecursively())
    {
        return true;
    }
    else return false;
}

bool IfisMap(QString WorldName)
{
    QDir path = QDir(SAVES_PATH);
    path.cd(WorldName);

    if((path.exists("WorldInfo.dat"))&&(path.cd("DIM0")))
    {
        QDir(SAVES_PATH);
        return true;
    }
    else
    {
        QDir(SAVES_PATH);
        return false;
    }
}

int NumOfMaps()
{
    int Num = 0;

    QDir path = QDir(SAVES_PATH);
    path.setFilter(QDir::Dirs);
    path.setSorting(QDir::Time);
    //只筛查文件夹

    QFileInfoList list = path.entryInfoList();
    int i = 0;
    do {
            QFileInfo fileInfo = list.at(i);
            if (fileInfo.fileName() == "." | fileInfo.fileName() == "..")
            {
                i++;
                continue;
            }
            else
            {
                if(IfisMap(fileInfo.fileName()))
                {
                    Num++;
                    //qDebug()<<fileInfo.fileName()<<endl;
                }
            }
            i++;

        } while (i < list.size());

    return Num;
}

QString GetNthMap(int Nth)
{
    QDir path = QDir(SAVES_PATH);
    path.setFilter(QDir::Dirs);
    path.setSorting(QDir::Time);
    QFileInfoList list = path.entryInfoList();
    int Num = NumOfMaps();
    if(Num == 0){
        return "无现有世界，请创建世界";
    }
    Nth = Nth % Num;
    if(Nth<=0)
    {
        Nth += Num;
    }
    int temp = 0;
    int i = 0;
    do {
            QFileInfo fileInfo = list.at(i);
            if (fileInfo.fileName() == "." | fileInfo.fileName() == "..")
            {
                i++;
                continue;
            }

            if (IfisMap(fileInfo.fileName()))
            {
                temp++;
                if(temp==Nth)
                {
                    return fileInfo.fileName();
                }

            }
            i++;

    }while(i<list.size());

}

bool CreateNewMap(QString WorldName,long int seed)
{
    QDir path = QDir(SAVES_PATH);
    path.setFilter(QDir::Dirs);
    path.setSorting(QDir::Time);

    if(path.exists(WorldName))
    {
        //如果已经存在此文件夹
        return false;
    }
    else
    {

        path.mkdir(WorldName);
        //创建了一个文件夹
        path.cd(WorldName);
        //进入这个文件夹
        path.mkdir("DIM0");
        //创建了0号次元

        //==================写入世界信息==================//
        QString worldpath = SAVES_PATH;
        worldpath.append("/").append(WorldName).append("/WorldInfo.dat");
        qDebug()<<worldpath<<endl;
        FILE *p = fopen(worldpath.toStdString().c_str(),"wb");
        int Size = sizeof(long int);

        fwrite(&seed,Size,1,p);

        fclose(p);
        //==================写入世界信息==================//
        return true;
    }

}




//void PrintMapInfo(QString WorldName)
//{
//    //需要结合UI
//}


