#include "wndimport.h"
#include "ui_wndimport.h"

#include <QFile>
#include <QMessageBox>

void strDelQuot(QStringList &slist) {
    for (auto &s : slist) {
        if ((s.left(1) == "\"") && (s.right(1) == "\""))
            s = s.mid(1, s.length()-2);
    }
}

WndImport::WndImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WndImport)
{
    ui->setupUi(this);
}

WndImport::~WndImport()
{
    delete ui;
}

bool WndImport::chkOptions(QString fname, GraphPaint::LoadOpt &opt)
{
    if (!loadData(fname))
        return false;

    ui->labVCount->setText(QString::number(m_data.size()));

    int ok = exec();

    if (ok == QDialog::Accepted) {
        opt.colTm = ui->cmbTm->currentIndex()-1;
        opt.colSrc = ui->cmbSig->currentIndex()-1;
        opt.colTrue = ui->cmbTrue->currentIndex()-1;
    }

    m_hdr.clear();
    m_data.clear();

    return ok == QDialog::Accepted;
}

bool WndImport::loadData(QString fname)
{
    QFile loadFile(fname);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Ошибка открытия файла: " + loadFile.errorString());
        return false;
    }

    m_hdr.clear();
    m_data.clear();

    QTextStream stxt(&loadFile);
    m_hdr = stxt.readLine().trimmed().split(";");
    strDelQuot(m_hdr);

    while (!stxt.atEnd()) {
        QString str = stxt.readLine().trimmed();
        m_data.push_back(str.split(";"));
        strDelQuot(m_data.back());
    }

    ui->cmbTm->clear();
    ui->cmbSig->clear();
    ui->cmbTrue->clear();
    ui->cmbTm->addItem("- выбрать колонку -");
    ui->cmbSig->addItem("- выбрать колонку -");
    ui->cmbTrue->addItem("- не использовать -");

    int n = 0;
    for (const auto &h : m_hdr) {
        n++;
        QString name("[" + QString::number(n) + "] " + h);
        ui->cmbTm->addItem(name);
        ui->cmbSig->addItem(name);
        ui->cmbTrue->addItem(name);

        if (h.toLower() == "tm")
            ui->cmbTm->setCurrentIndex(n);
        else
        if (h.toLower() == "src")
            ui->cmbSig->setCurrentIndex(n);
        else
        if (h.toLower() == "true")
            ui->cmbTrue->setCurrentIndex(n);
    }

    updateBtnEnable();

    return true;
}

void WndImport::clearData()
{
    m_hdr.clear();
    m_data.clear();
}

void WndImport::on_btnOk_clicked()
{
    accept();
}

void WndImport::on_btnCancel_clicked()
{
    reject();
}

void WndImport::updateBtnEnable()
{
    ui->btnOk->setEnabled(ui->cmbSig->currentIndex() > 0);
}


void WndImport::on_cmbSig_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    updateBtnEnable();
}

