#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->portName->addItem("/dev/ttyUSB0",0);
    ui->portName->addItem("COM4",0);
    connect(&serial,SIGNAL(lineReceived(QString)),
            this,SLOT(onLineReceived(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    serial.write(ui->commandText->text()+"\n");
}

void MainWindow::onLineReceived(QString data)
{
    ui->textBrowser->append(data);
}

void MainWindow::on_openCloseButton_clicked()
{
    if(serial.isOpen())
    {
        serial.close();
        ui->openCloseButton->setText("Open");
    } else {
        ui->textBrowser->clear();
        serial.open(ui->portName->currentText(),115200);
        if(!serial.isOpen() || serial.errorStatus()) return;
        ui->openCloseButton->setText("Close");
    }
}
