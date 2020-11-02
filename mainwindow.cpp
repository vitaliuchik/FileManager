#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    ui->menuFile->addAction(ui->actionRename);

    generateFilePanel(ui->listWidget, "/home/papka");

    // enter and double click
    connect(ui->listWidget, &QListWidget::itemActivated, this, &MainWindow::processItem);

}

void MainWindow::processItem(QListWidgetItem *item) {

    auto itemIndex = item->listWidget()->row(item) + 1;

    if (files[itemIndex].isDir()) {
        generateFilePanel(item->listWidget(), files[itemIndex].absoluteFilePath());
    }

}

void MainWindow::generateFilePanel(QListWidget *listPanel, QString pathString) {

    listPanel->clear();

    currDir = QDir(pathString);
    files = currDir.entryInfoList();

    for (int i = 1; i < files.size(); i++) {
        QListWidgetItem *newItem = new QListWidgetItem;

        QString temp = "";
        QString text = files[i].fileName() + "\t   " + (files[i].isDir() ? "dir " : "file") + "   ";
        text += files[i].lastModified().toString("yyyy.MM.dd") + "   ";
        temp.setNum(files[i].size());
        text += (files[i].isDir() ? "" : temp);

        newItem->setText(text);
        listPanel->insertItem(i, newItem);
    }
    listPanel->item(0)->setSelected(true);

}
