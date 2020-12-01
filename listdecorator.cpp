#include "listdecorator.h"

ListDecorator::ListDecorator(QListWidget *listWidget_, QString dirPath_, bool isActive_, QLabel* label_)
    : listWidget(listWidget_), dirPath(dirPath_), isActive(isActive_), label(label_) {

    fileInfoList = QDir(dirPath_).entryInfoList();

}

QFileInfoList ListDecorator::getFileInfoList() {
    return QDir(dirPath).entryInfoList();
}
