#include "wnddata.h"
#include "ui_wnddata.h"
#include "moddata.h"
#include "graphpaint.h"
#include "wndimport.h"

#include <QStandardPaths>
#include <QFileDialog>
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

    m_save_path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    wimport = new WndImport(this);
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

void WndData::on_btnLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать файл"), m_save_path, tr("Coma-separated files (*.csv)"));
    if (fileName.isEmpty())
        return;
    m_save_path = QFileInfo(fileName).path();

    GraphPaint::LoadOpt opt;
    if (!wimport->chkOptions(fileName, opt))
        return;

    if (!m_chrt->dataLoadCSV(fileName, opt))
        return;

    dataUpdate(true);
}

void WndData::on_btnSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Выбрать файл для сохранения"), m_save_path, tr("Coma-separated files (*.csv)"));
    if (fileName.isEmpty()) {
        return;
    }
    m_save_path = QFileInfo(fileName).path();

    m_chrt->dataSaveCSV(fileName, ui->spinFloatNum->value());
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
