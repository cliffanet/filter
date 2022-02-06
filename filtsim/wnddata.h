#ifndef WNDDATA_H
#define WNDDATA_H

#include <QDialog>

namespace Ui {
class WndData;
}

class GraphPaint;

class WndData : public QDialog
{
    Q_OBJECT

public:
    explicit WndData(GraphPaint *chrt, QWidget *parent = nullptr);
    ~WndData();

    void dataUpdate(bool scroll2end = false);

private slots:
    void on_spinFloatNum_valueChanged(int arg1);

private:
    Ui::WndData *ui;
    GraphPaint *m_chrt;
};

#endif // WNDDATA_H
