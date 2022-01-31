#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->chrt->setVMax(ui->slSigLevel->maximum());

    tmrSim = new QTimer(this);
    connect(tmrSim, &QTimer::timeout, this, &MainWindow::dataSym);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataSym()
{
    ui->chrt->tick(ui->slSigLevel->value(), 0);
}


void MainWindow::on_btnStart_clicked()
{
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    tmrSim->start(100);
}


void MainWindow::on_btnStop_clicked()
{
    ui->btnStop->setEnabled(false);
    ui->btnStart->setEnabled(true);
    tmrSim->stop();
}


void MainWindow::on_btnClear_clicked()
{
    ui->chrt->clear();
}

