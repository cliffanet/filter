#ifndef WNDIMPORT_H
#define WNDIMPORT_H

#include <QDialog>
#include "graphpaint.h"

namespace Ui {
class WndImport;
}

class WndImport : public QDialog
{
    Q_OBJECT

public:
    explicit WndImport(QWidget *parent = nullptr);
    ~WndImport();

    bool chkOptions(QString fname, GraphPaint::LoadOpt &opt);
    bool loadData(QString fname);
    void clearData();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

    void on_cmbSig_currentIndexChanged(int index);

private:
    void updateBtnEnable();

private:
    Ui::WndImport *ui;

    QStringList m_hdr;
    QList <QStringList>m_data;
};

#endif // WNDIMPORT_H
