#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dataSym();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *tmrSim;
};
#endif // MAINWINDOW_H
