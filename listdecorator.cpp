#include "listdecorator.h"

ListDecorator::ListDecorator(QListWidget *listWidget_, QString dirPath_, bool isActive_)
    : listWidget(listWidget_), dirPath(dirPath_), isActive(isActive_) {

    fileInfoList = QDir(dirPath_).entryInfoList();

}
