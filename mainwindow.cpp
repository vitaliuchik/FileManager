#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTextStream>
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_listWidgetCollection = {
            new ListDecorator(ui->listWidget1, "/", true), new ListDecorator(ui->listWidget2, "/", false)};

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::init() {

//}

//ListDecorator *MainWindow::getActiveList() {
//    return m_listWidgetCollection[0]->isActive ? m_listWidgetCollection[0] : m_listWidgetCollection[1];
//}


void MainWindow::init() {

    generateFilePanel1(m_activeList, "/");
    generateFilePanel2(m_passiveList, "/");

    ui->menuFile->addAction(ui->actionRemove);
    ui->menuFile->addAction(ui->actionCopy);
    ui->menuFile->addAction(ui->actionMove);

    // enter and double click
    connect(m_activeList, &QListWidget::itemActivated, this, &MainWindow::processItem1);
    connect(m_passiveList, &QListWidget::itemActivated, this, &MainWindow::processItem2);

    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copyItem);
    connect(ui->actionMove, &QAction::triggered, this, &MainWindow::moveItem);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::removeItem);

}

void MainWindow::processItem1(QListWidgetItem *item) {

    auto files = m_filesVector[0];

    auto itemIndex = item->listWidget()->row(item) + 1;

    if (files[itemIndex].isDir()) {
        generateFilePanel1(item->listWidget(), files[itemIndex].absoluteFilePath());
    }
}

void MainWindow::processItem2(QListWidgetItem *item) {

    auto files = m_filesVector[1];

    auto itemIndex = item->listWidget()->row(item) + 1;

    if (files[itemIndex].isDir()) {
        generateFilePanel2(item->listWidget(), files[itemIndex].absoluteFilePath());
    }
}

void MainWindow::copyItem() {

    QListWidgetItem *item = m_activeList->currentItem();

    auto files = m_filesVector[0];

    auto itemIndex = item->listWidget()->row(item) + 1;

    QString newPath = m_currDirVector[1].path() + "/" + files[itemIndex].fileName();
    if (QFile::exists(newPath))
    {
        QFile::remove(newPath);
    }

    QFile::copy(files[itemIndex].absoluteFilePath(), newPath);

//    QTextStream out(stdout);
//    out << files[itemIndex].absoluteFilePath() << endl;
//    out << newPath << endl;


    generateFilePanel2(m_passiveList, m_currDirVector[1].path());

}

void MainWindow::moveItem() {

    QListWidgetItem *item = m_activeList->currentItem();

    auto files = m_filesVector[0];

    auto itemIndex = item->listWidget()->row(item) + 1;

    QString newPath = m_currDirVector[1].path() + "/" + files[itemIndex].fileName();
    if (QFile::exists(newPath))
    {
        QFile::remove(newPath);
    }

    QFile::copy(files[itemIndex].absoluteFilePath(), newPath);
    if (QFile::exists(files[itemIndex].absoluteFilePath()))
    {
        QFile::remove(files[itemIndex].absoluteFilePath());
    }

    generateFilePanel1(m_activeList, m_currDirVector[0].path());
    generateFilePanel2(m_passiveList, m_currDirVector[1].path());

}

void MainWindow::removeItem() {

    QListWidgetItem *item = m_activeList->currentItem();

    auto files = m_filesVector[0];
    auto itemIndex = item->listWidget()->row(item) + 1;
    if (QFile::exists(files[itemIndex].absoluteFilePath()))
    {
        QFile::remove(files[itemIndex].absoluteFilePath());
    }

    generateFilePanel1(m_activeList, m_currDirVector[0].path());
    generateFilePanel2(m_passiveList, m_currDirVector[1].path());
}

void MainWindow::generateFilePanel1(QListWidget *listPanel, QString pathString) {

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

    pathString.replace(QString("../"), QString(""));
    ui->label1->setText(pathString);
}

void MainWindow::generateFilePanel2(QListWidget *listPanel, QString pathString) {

    listPanel->clear();

    m_currDirVector[1] = QDir(pathString);
    m_filesVector[1] = m_currDirVector[1].entryInfoList();
    auto files = m_filesVector[1];

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

    pathString.replace(QString("../"), QString(""));
    ui->label2->setText(pathString);
}



