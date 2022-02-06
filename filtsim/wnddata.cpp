#include "wnddata.h"
#include "ui_wnddata.h"
#include "moddata.h"
#include "graphpaint.h"

WndData::WndData(GraphPaint *chrt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WndData),
    m_chrt(chrt)
{
    ui->setupUi(this);
    setWindowTitle( "Данные в табличном представлении" );

    ui->twData->setModel(new ModData(chrt->rdata(), this));
}

WndData::~WndData()
{
    delete ui;
}

void WndData::dataUpdate(bool scroll2end)
{
    emit ui->twData->model()->layoutChanged();
    if (scroll2end)
        ui->twData->scrollToBottom();
}

void WndData::on_spinFloatNum_valueChanged(int arg1)
{
    reinterpret_cast<ModData *>(ui->twData->model())->setFNum(arg1);
}

