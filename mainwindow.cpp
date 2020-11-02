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

void MainWindow::setActiveList(QListWidget *listWidget) {
    m_activeList = listWidget;
    ++m_activeInd %= 2;
}

void MainWindow::setPassiveList(QListWidget *listWidget) {
    m_passiveList = listWidget;
    ++m_passiveInd %= 2;
}



void MainWindow::init() {
    ui->menuFile->addAction(ui->actionRename);
    ui->menuFile->addAction(ui->actionCopy);
    ui->menuFile->addAction(ui->actionMove);

    generateFilePanel(ui->listWidget1, "/home/papka");
    generateFilePanel(ui->listWidget2, "/");

    // enter and double click
    connect(ui->listWidget1, &QListWidget::itemActivated, this, &MainWindow::processItem);
    connect(ui->listWidget2, &QListWidget::itemActivated, this, &MainWindow::processItem);

}

void MainWindow::processItem(QListWidgetItem *item) {

    auto files = m_filesVector[0];

    auto itemIndex = item->listWidget()->row(item) + 1;

    if (files[itemIndex].isDir()) {
        generateFilePanel(item->listWidget(), files[itemIndex].absoluteFilePath());
    }

}

void MainWindow::generateFilePanel(QListWidget *listPanel, QString pathString) {

    listPanel->clear();

    m_currDirVector[0] = QDir(pathString);
    m_filesVector[0] = m_currDirVector[0].entryInfoList();
    auto files = m_filesVector[0];

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
