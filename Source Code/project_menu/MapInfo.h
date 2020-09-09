#ifndef MAPINFO_H
#define MAPINFO_H
#include <QString>
#include <iostream>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <stdio.h>
#include <windows.h>

#define SAVES_PATH "../saves"


bool IfisMap(QString WorldName);
bool CreateNewFolder(QString WorldName);
bool DeleteFolder(QString WorldName);
bool CreateNewMap(QString WorldName,long int seed);
int NumOfMaps();
QString GetNthMap(int Nth);
#endif // MAPINFO_H
