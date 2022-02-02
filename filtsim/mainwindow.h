#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTimer;
class QElapsedTimer;

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
    void on_slSigLevel_sliderMoved(int position);
    void on_slSigNoise_sliderMoved(int position);
    void on_cmbSigType_currentIndexChanged(int index);
    void on_slAvgSize_sliderMoved(int position);
    void on_cmbAvgType_currentIndexChanged(int index);
    void on_slAvg2Size_sliderMoved(int position);
    void on_cmbAvg2Type_currentIndexChanged(int index);
    void on_slLtSqrtSize_sliderMoved(int position);
    void on_cmbLtSqrtType_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QTimer *tmrSim;
    QElapsedTimer *tmrElaps;
};
#endif // MAINWINDOW_H
