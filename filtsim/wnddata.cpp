#include "wnddata.h"
#include "ui_wnddata.h"
#include "moddata.h"
#include "graphpaint.h"

#include <QHideEvent>
#include <QModelIndex>

WndData::WndData(GraphPaint *chrt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WndData),
    m_chrt(chrt)
{
    ui->setupUi(this);
    setWindowTitle( "Данные в табличном представлении" );

    ui->twData->setModel(new ModData(chrt->rdata(), this));
    connect(
        ui->twData->selectionModel(), &QItemSelectionModel::selectionChanged,
        this, &WndData::twSelectionChanged
    );
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

void WndData::setSelected(int index)
{
    auto tbl = ui->twData;
    tbl->setCurrentIndex(tbl->model()->index(index, 0));
}

void WndData::on_spinFloatNum_valueChanged(int arg1)
{
    reinterpret_cast<ModData *>(ui->twData->model())->setFNum(arg1);
}

void WndData::twSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
    auto &sel = ui->twData->selectionModel()->selection();
    if (sel.count() > 0)
        m_chrt->setSelected(sel.indexes().first().row());
    else
        m_chrt->clearSelected();
}

void WndData::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    ui->twData->clearSelection();
    m_chrt->clearSelected();
}

