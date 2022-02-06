#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTimer;
class QElapsedTimer;

class WndData;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void dataSym();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnClear_clicked();
    void on_btnData_clicked();

    void on_cbSigVisible_stateChanged(int arg1);
    void on_slSigLevel_sliderMoved(int position);
    void on_slSigNoise_sliderMoved(int position);
    void on_cmbSigType_currentIndexChanged(int index);
    void on_cbTrueVisible_stateChanged(int arg1);
    void on_cmbTrueType_currentIndexChanged(int index);
    void on_slBufSize_sliderMoved(int position);
    void on_cbAvgVisible_stateChanged(int arg1);
    void on_slAvgSize_sliderMoved(int position);
    void on_cmbAvgType_currentIndexChanged(int index);
    void on_cbAvg2Visible_stateChanged(int arg1);
    void on_slAvg2Size_sliderMoved(int position);
    void on_cmbAvg2Type_currentIndexChanged(int index);
    void on_cbLtSqrtVisible_stateChanged(int arg1);
    void on_slLtSqrtSize_sliderMoved(int position);
    void on_cmbLtSqrtType_currentIndexChanged(int index);

    void on_btnViewReset_clicked();

    void on_chkWhiteBg_clicked();
    void on_chkBorder_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *tmrSim;
    QElapsedTimer *tmrElaps;

    WndData * wdata;

    void changeValRange();
    void changeXOffset(int x);
    void changeYOffset(int y);
    void changeXScale(int x);
    void changeYScale(int y);
    void updateViewOffset();
    void updateViewScale();
    void indexSelected(int index);
};
#endif // MAINWINDOW_H
