#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QElapsedTimer>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle( QCoreApplication::applicationName() );

    ui->chrt->setVMax(ui->slSigLevel->maximum());
    on_cmbSigType_currentIndexChanged(ui->cmbSigType->currentIndex());
    on_cmbAvgType_currentIndexChanged(ui->cmbAvgType->currentIndex());
    on_cmbAvg2Type_currentIndexChanged(ui->cmbAvg2Type->currentIndex());
    on_cmbLtSqrtType_currentIndexChanged(ui->cmbLtSqrtType->currentIndex());
    on_slSigLevel_sliderMoved(0);
    on_slSigNoise_sliderMoved(0);
    on_slBufSize_sliderMoved(0);
    on_slAvgSize_sliderMoved(0);
    on_slAvg2Size_sliderMoved(0);
    on_slLtSqrtSize_sliderMoved(0);

    const QColor sigColor[] = { Qt::red, Qt::magenta, Qt::green, Qt::darkYellow, Qt::blue };
    for (auto id = GraphPaint::DataSrc; id < GraphPaint::DataCount; id = static_cast<GraphPaint::DataID>(id+1))
        if ((id >= 0) && (id < sizeof(sigColor)/sizeof(QColor)))
            ui->chrt->setDataColor(id, sigColor[id]);

    on_slAvgSize_sliderMoved(0);
    on_slAvg2Size_sliderMoved(0);
    on_slLtSqrtSize_sliderMoved(0);

    tmrSim = new QTimer(this);
    connect(tmrSim, &QTimer::timeout, this, &MainWindow::dataSym);
    tmrElaps = new QElapsedTimer;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tmrElaps;
}

void MainWindow::dataSym()
{
    int sigtrue = ui->slSigLevel->value();
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> d( sigtrue, ui->slSigNoise->value() );

    static qint64 tmprev = 0;
    auto tmcur = tmrElaps->elapsed();
    ui->chrt->tick(d(gen), tmcur - tmprev, sigtrue);
    tmprev = tmcur;
}


void MainWindow::on_btnStart_clicked()
{
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    tmrElaps->restart();
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


void MainWindow::on_cbSigVisible_stateChanged(int arg1)
{
    ui->chrt->setDataVisible(GraphPaint::DataSrc, ui->cbSigVisible->isChecked());
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

void MainWindow::on_cbTrueVisible_stateChanged(int arg1)
{
    ui->chrt->setDataVisible(GraphPaint::DataTrue, ui->cbTrueVisible->isChecked());
}

void MainWindow::on_cmbTrueType_currentIndexChanged(int index)
{
    ui->chrt->setDrawType(
        GraphPaint::DataTrue,
        index == 1 ?
            GraphPaint::DrawLine :
            GraphPaint::DrawPoint
    );
}

void MainWindow::on_slBufSize_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVBufSize->setText(QString::number(ui->slBufSize->value()));
    ui->chrt->setDataSize(static_cast<size_t>(ui->slBufSize->value()));
}

void MainWindow::on_cbAvgVisible_stateChanged(int arg1)
{
    ui->chrt->setDataVisible(GraphPaint::DataAvg, ui->cbAvgVisible->isChecked());
}


void MainWindow::on_slAvgSize_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVAvgSize->setText(QString::number(ui->slAvgSize->value()));
    ui->chrt->setFilter(GraphPaint::DataAvg, new filtAvg(ui->slAvgSize->value()));
}


void MainWindow::on_cmbAvgType_currentIndexChanged(int index)
{
    ui->chrt->setDrawType(
        GraphPaint::DataAvg,
        index == 1 ?
            GraphPaint::DrawLine :
            GraphPaint::DrawPoint
    );
}

void MainWindow::on_cbAvg2Visible_stateChanged(int arg1)
{
    ui->chrt->setDataVisible(GraphPaint::DataAvg2, ui->cbAvg2Visible->isChecked());
}

void MainWindow::on_slAvg2Size_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVAvg2Size->setText(QString::number(ui->slAvg2Size->value()));
    ui->chrt->setFilter(GraphPaint::DataAvg2, new filtAvg2(ui->slAvg2Size->value()));
}

void MainWindow::on_cmbAvg2Type_currentIndexChanged(int index)
{
    ui->chrt->setDrawType(
        GraphPaint::DataAvg2,
        index == 1 ?
            GraphPaint::DrawLine :
            GraphPaint::DrawPoint
    );
}

void MainWindow::on_cbLtSqrtVisible_stateChanged(int arg1)
{
    ui->chrt->setDataVisible(GraphPaint::DataLtSqrt, ui->cbLtSqrtVisible->isChecked());
}

void MainWindow::on_slLtSqrtSize_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVLtSqrtSize->setText(QString::number(ui->slLtSqrtSize->value()));
    ui->chrt->setFilter(GraphPaint::DataLtSqrt, new filtAvg2(ui->slLtSqrtSize->value()));
}

void MainWindow::on_cmbLtSqrtType_currentIndexChanged(int index)
{
    ui->chrt->setDrawType(
        GraphPaint::DataLtSqrt,
        index == 1 ?
            GraphPaint::DrawLine :
            GraphPaint::DrawPoint
    );
}

