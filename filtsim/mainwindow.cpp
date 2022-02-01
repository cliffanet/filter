#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->chrt->setVMax(ui->slSigLevel->maximum());
    on_slSigLevel_sliderMoved(0);
    on_slSigNoise_sliderMoved(0);

    ui->wColSig->setStyleSheet("background-color: red;");
    const QColor sigColor[] = { Qt::red, Qt::green, Qt::darkYellow, Qt::magenta };
    for (auto id = GraphPaint::DataSrc; id < GraphPaint::DataCount; id = static_cast<GraphPaint::DataID>(id+1))
        if ((id >= 0) && (id < sizeof(sigColor)/sizeof(QColor)))
            ui->chrt->setDataColor(id, sigColor[id]);

    tmrSim = new QTimer(this);
    connect(tmrSim, &QTimer::timeout, this, &MainWindow::dataSym);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataSym()
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> d( ui->slSigLevel->value(), ui->slSigNoise->value() );
    ui->chrt->tick(d(gen), 0);
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


void MainWindow::on_slSigLevel_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVSigLevel->setText(QString::number(ui->slSigLevel->value()));
}


void MainWindow::on_slSigNoise_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVSigNoise->setText(QString::number(ui->slSigNoise->value()));
}


void MainWindow::on_cmbSigType_currentIndexChanged(int index)
{
    ui->chrt->setDrawType(
        GraphPaint::DataSrc,
        index == 1 ?
            GraphPaint::DrawLine :
            GraphPaint::DrawPoint
    );
}

