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

    m_activeList = ui->listWidget1;
    m_passiveList = ui->listWidget2;

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showException(QString message) {
    QMessageBox::warning(this, tr("Error"), message);
}


void MainWindow::init() {
    ui->menuFile->addAction(ui->actionRemove);
    ui->menuFile->addAction(ui->actionCopy);
    ui->menuFile->addAction(ui->actionMove);
    ui->menuFile->addAction(ui->actionRename);
    ui->menuFile->addAction(ui->actionNew_Directory);
//    ui->menuFile->addAction(ui->actionNew_File);
    ui->menuFile->addAction(ui->actionSearch);

    generateFilePanel1(m_activeList, "/home/papka");
    generateFilePanel2(m_passiveList, "/home/papka/Downloads");

    // enter and double click
    connect(m_activeList, &QListWidget::itemActivated, this, &MainWindow::processItem1);
    connect(m_passiveList, &QListWidget::itemActivated, this, &MainWindow::processItem2);

    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copyItem);
    connect(ui->actionMove, &QAction::triggered, this, &MainWindow::moveItem);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::removeItem);
    connect(ui->actionRename, &QAction::triggered, this, &MainWindow::renameItem);
    connect(ui->actionNew_Directory, &QAction::triggered, this, &MainWindow::makeDir);
//    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::makeFile);
    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::searchItem);

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
    if (files[itemIndex].isFile()) {
        if (QFile::exists(newPath))
        {
            QFile::remove(newPath);
        }

        QFile::copy(files[itemIndex].absoluteFilePath(), newPath);
    }

    else if(files[itemIndex].isDir()) {
        if (!copyRecursively(files[itemIndex].absoluteFilePath(), newPath)) {
            showException("Cannot copy this directory");
        }
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
    if (files[itemIndex].isFile() && QFile::exists(files[itemIndex].absoluteFilePath()))
    {
        if (!QFile::remove(files[itemIndex].absoluteFilePath())){
            showException("Cannot remove this file");
        }
    }

    else if (files[itemIndex].isDir() && QFile::exists(files[itemIndex].absoluteFilePath()))
    {
        if (!QDir(files[itemIndex].absoluteFilePath()).removeRecursively()){
            showException("Cannot remove this dir");
        }
    }

    generateFilePanel1(m_activeList, m_currDirVector[0].path());
    generateFilePanel2(m_passiveList, m_currDirVector[1].path());
}

void MainWindow::makeDir() {

    QString newDirName = QInputDialog::getText(this,"Title","text");

    QListWidgetItem *item = m_activeList->currentItem();

    auto files = m_filesVector[0];
    auto itemIndex = item->listWidget()->row(item) + 1;
    if (QDir(files[itemIndex].absolutePath()).exists())
    {
        if (!QDir().mkdir(files[itemIndex].absolutePath() + "/" + newDirName)){
            showException("Cannot create directory");        }
    }

    generateFilePanel1(m_activeList, m_currDirVector[0].path());
    generateFilePanel2(m_passiveList, m_currDirVector[1].path());

}

void MainWindow::makeFile() {

    QString newFileName = QInputDialog::getText(this,"Title","text");

    QListWidgetItem *item = m_activeList->currentItem();

    auto files = m_filesVector[0];
    auto itemIndex = item->listWidget()->row(item) + 1;
    if (QDir(files[itemIndex].absolutePath()).exists())
    {
        QFile filepath;
        filepath.setFileName(files[itemIndex].absolutePath() + "/" + newFileName);
        if (!filepath.open(QIODevice::WriteOnly|QIODevice::Text)){
            showException("Cannot create file");        }
    }

    generateFilePanel1(m_activeList, m_currDirVector[0].path());
    generateFilePanel2(m_passiveList, m_currDirVector[1].path());

}

void MainWindow::renameItem() {

    QString newFileName = QInputDialog::getText(this,"Title","text");

    QListWidgetItem *item = m_activeList->currentItem();

    auto files = m_filesVector[0];
    auto itemIndex = item->listWidget()->row(item) + 1;
    if (QFile::exists(files[itemIndex].absoluteFilePath()))
    {
        if (!QFile::rename(files[itemIndex].absoluteFilePath(), files[itemIndex].absolutePath() + "/" + newFileName)){
            showException("Cannot rename this file");
        }
    }

    generateFilePanel1(m_activeList, m_currDirVector[0].path());
    generateFilePanel2(m_passiveList, m_currDirVector[1].path());

}

void MainWindow::searchItem() {

//    QString searchedFileName = QInputDialog::getText(this,"Title","text");

//    QVector<QString> *resultPath = {};
//    searchRecursively("", searchedFileName, resultPath);

//    if (resultPath->size() > 0) {

//        QMessageBox::warning(this, tr("Error"), resultPath->at(0));
//    }
//    else {

//        QMessageBox::warning(this, tr("Error"), tr("File did not exist"));
//    }

    QMessageBox::warning(this, tr("Error"), tr("Manager does not support searching files"));

//    generateFilePanel1(m_activeList, m_currDirVector[0].path());
//    generateFilePanel2(m_passiveList, m_currDirVector[1].path());

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

////////////////////////////////////////////////
bool MainWindow::copyRecursively(QString srcFilePath, QString tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!MainWindow::copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}

void MainWindow::searchRecursively(const QString &path, const QString &pattern, QVector<QString> *result)
{
    QDir currentDir(path);
    const QString prefix = path + "/";
    foreach (const QString &match, currentDir.entryList(QStringList(pattern), QDir::Files | QDir::NoSymLinks))
        result->append(prefix + match);
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        MainWindow::searchRecursively(prefix + dir, pattern, result);
}
