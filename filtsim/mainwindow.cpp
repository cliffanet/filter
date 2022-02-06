#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "wnddata.h"

#include <QTimer>
#include <QElapsedTimer>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle( QCoreApplication::applicationName() );

    wdata = new WndData(ui->chrt, this);

    connect(ui->chrt, &GraphPaint::valDrawRangeChanged, this, &MainWindow::changeValRange);
    connect(ui->chrt, &GraphPaint::offsetXChanged, this, &MainWindow::changeXOffset);
    connect(ui->chrt, &GraphPaint::offsetYChanged, this, &MainWindow::changeYOffset);
    connect(ui->chrt, &GraphPaint::scaleXChanged, this, &MainWindow::changeXScale);
    connect(ui->chrt, &GraphPaint::scaleYChanged, this, &MainWindow::changeYScale);
    connect(ui->chrt, &GraphPaint::indexSelected, this, &MainWindow::indexSelected);

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

    on_btnViewReset_clicked();

    on_chkWhiteBg_clicked();
    on_chkBorder_clicked();

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

    wdata->dataUpdate(true);
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
    wdata->dataUpdate();
}

void MainWindow::on_btnData_clicked()
{
    wdata->showNormal();
    wdata->raise();
}

void MainWindow::on_cbSigVisible_stateChanged(int arg1)
{
    Q_UNUSED(arg1)
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
    Q_UNUSED(arg1)
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
    Q_UNUSED(arg1)
    ui->chrt->setDataVisible(GraphPaint::DataAvg, ui->cbAvgVisible->isChecked());
}

void MainWindow::on_slAvgSize_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVAvgSize->setText(QString::number(ui->slAvgSize->value()));
    ui->chrt->resizeAvg(ui->slAvgSize->value());
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
    Q_UNUSED(arg1)
    ui->chrt->setDataVisible(GraphPaint::DataAvg2, ui->cbAvg2Visible->isChecked());
}

void MainWindow::on_slAvg2Size_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVAvg2Size->setText(QString::number(ui->slAvg2Size->value()));
    ui->chrt->resizeAvg2(ui->slAvg2Size->value());
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
    Q_UNUSED(arg1)
    ui->chrt->setDataVisible(GraphPaint::DataLtSqrt, ui->cbLtSqrtVisible->isChecked());
}

void MainWindow::on_slLtSqrtSize_sliderMoved(int position)
{
    Q_UNUSED(position)
    ui->labVLtSqrtSize->setText(QString::number(ui->slLtSqrtSize->value()));
    ui->chrt->resizeLtSqrt(ui->slLtSqrtSize->value());
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

void MainWindow::on_btnViewReset_clicked()
{
    ui->chrt->setScale(100, 100);
    ui->chrt->setOffset(0, 0);
}

void MainWindow::on_chkWhiteBg_clicked()
{
    ui->chrt->setDrawFlags(GraphPaint::DrawWhiteBg, ui->chkWhiteBg->isChecked());
}

void MainWindow::on_chkBorder_clicked()
{
    ui->chrt->setDrawFlags(GraphPaint::DrawBorder, ui->chkBorder->isChecked());
}

void MainWindow::changeValRange()
{
    int min = static_cast<int>(std::round( ui->chrt->valDrawMin() ));
    ui->slSigLevel->setMinimum(min);
    ui->labVSigMin->setText(QString::number(min));
    int max = static_cast<int>(std::round( ui->chrt->valDrawMax() ));
    ui->labVSigMax->setText(QString::number(max));
    ui->slSigLevel->setMaximum(max);
    on_slSigLevel_sliderMoved(0);
}

void MainWindow::changeXOffset(int x)
{
    Q_UNUSED(x)
    updateViewOffset();
}

void MainWindow::changeYOffset(int y)
{
    Q_UNUSED(y)
    updateViewOffset();
}

void MainWindow::changeXScale(int x)
{
    Q_UNUSED(x)
    updateViewScale();
}

void MainWindow::changeYScale(int y)
{
    Q_UNUSED(y)
    updateViewScale();
}

void MainWindow::updateViewOffset()
{
    ui->labVOffset->setText(
        QString::number(ui->chrt->offsetX()) +
        " x " +
        QString::number(ui->chrt->offsetY())
    );
}

void MainWindow::updateViewScale()
{
    ui->labVScale->setText(
        QString::number(ui->chrt->scaleX()) +
        " x " +
        QString::number(ui->chrt->scaleY())
    );
}

void MainWindow::indexSelected(int index)
{
    if (wdata->isVisible())
        wdata->setSelected(index);
}

