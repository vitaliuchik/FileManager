#ifndef LISTDECORATOR_H
#define LISTDECORATOR_H

//#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QString>
#include <QListWidget>
#include <QFileInfoList>
#include <QDir>
#include <QLabel>
#include <QDateTime>


class ListDecorator
{
public:


    ListDecorator(QListWidget *listWidget_, QString dirPath_, bool isActive_, QLabel *label_);

    QFileInfoList getFileInfoList();

    QListWidget *listWidget;
    QString dirPath;
    bool isActive;
    QFileInfoList fileInfoList;
    QLabel *label;

};

#endif // LISTDECORATOR_H
