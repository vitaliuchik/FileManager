#ifndef LISTDECORATOR_H
#define LISTDECORATOR_H

#include <QString>
#include <QListWidget>
#include <QFileInfoList>
#include <QDir>

class ListDecorator
{
public:


    ListDecorator(QListWidget *listWidget_, QString dirPath_, bool isActive_);

    QListWidget *listWidget;
    QString dirPath;
    bool isActive;
    QFileInfoList fileInfoList;

};

#endif // LISTDECORATOR_H
