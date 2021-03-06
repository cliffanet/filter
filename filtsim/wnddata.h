#ifndef WNDDATA_H
#define WNDDATA_H

#include <QDialog>

namespace Ui {
class WndData;
}

class GraphPaint;
class QHideEvent;
class QItemSelection;
class WndImport;

class WndData : public QDialog
{
    Q_OBJECT

public:
    explicit WndData(GraphPaint *chrt, QWidget *parent = nullptr);
    ~WndData();

    void dataUpdate(bool scroll2end = false);
    void setSelected(int index);

private slots:
    void on_btnLoad_clicked();
    void on_btnSave_clicked();
    void on_spinFloatNum_valueChanged(int arg1);

private:
    void twSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

protected:
   void hideEvent(QHideEvent * event);

private:
    Ui::WndData *ui;
    GraphPaint *m_chrt;
    QString m_save_path;
    WndImport * wimport;
};

#endif // WNDDATA_H
