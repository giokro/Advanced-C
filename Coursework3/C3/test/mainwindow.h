#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocalSocket>
#include <QDebug>
#include <wchar.h>
#include <QMessageBox>
#include "connector.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QThread cThread;
    Connector *connectO;

private slots:
    void on_Exit_clicked();

    void on_Connect_clicked();
    void on_connected();

    void on_Break_clicked();

    void on_Compute_clicked();

    void on_Functions_currentIndexChanged(int index);

    void on_readyRead();


private:
    Ui::MainWindow *ui;
    QLocalSocket *sock;
    bool inputOK();
    double getDouble(QVector<double>, bool);
    void stopThread(QThread *Thread);
    void closeEvent(QCloseEvent *Event);
    // this is the default close func that is called when you hit X.
    // We are changing it so that it stops the thread before exiting.
};

#endif // MAINWINDOW_H
