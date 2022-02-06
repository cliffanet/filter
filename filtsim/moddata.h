#ifndef MODDATA_H
#define MODDATA_H

#include <QAbstractTableModel>

#include "graphpaint.h"

class ModData: public QAbstractTableModel
{
    Q_OBJECT

public:
    ModData(const GraphPaint::RingData &_data, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setFNum(uint8_t fnum);

private:
    const GraphPaint::RingData &m_data;  //holds text entered into QTableView
    uint8_t m_fnum = 2;

};

#endif // MODDATA_H
