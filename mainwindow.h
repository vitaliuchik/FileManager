#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QListWidget>
#include <QDateTime>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void init();

    void generateFilePanel1(QListWidget *, QString);
    void generateFilePanel2(QListWidget *, QString);

    QVector<QDir> m_currDirVector = {QDir("/"), QDir("/")};
    QVector<QFileInfoList> m_filesVector = {QFileInfoList(), QFileInfoList()};
    QListWidget *m_activeList = new QListWidget();
    QListWidget *m_passiveList = new QListWidget();

private slots:
    void processItem1(QListWidgetItem *);
    void processItem2(QListWidgetItem *);

    void copyItem();
    void moveItem();
    void removeItem();
};
#endif // MAINWINDOW_H
