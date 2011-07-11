#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "QAsyncSerial.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAsyncSerial serial;

private slots:
    void on_openCloseButton_clicked();
    void on_pushButton_clicked();

    void onLineReceived(QString data);
};

#endif // MAINWINDOW_H
