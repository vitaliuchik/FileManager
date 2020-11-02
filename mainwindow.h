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

    void generateFilePanel(QListWidget *, QString);

    QVector<QDir> m_currDirVector = {QDir("/"), QDir("/")};
    QVector<QFileInfoList> m_filesVector = {QFileInfoList(), QFileInfoList()};
    QListWidget *m_activeList = new QListWidget();
    QListWidget *m_passiveList = new QListWidget();
    int m_activeInd = 0;
    int m_passiveInd = 1;

    void setActiveList(QListWidget *);
    void setPassiveList(QListWidget *);

private slots:
    void processItem(QListWidgetItem *);
};
#endif // MAINWINDOW_H
