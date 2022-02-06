#include "moddata.h"

ModData::ModData(const GraphPaint::RingData &_data, QObject *parent)
    : QAbstractTableModel(parent),
      m_data(_data)
{

}

int ModData::rowCount(const QModelIndex & /*parent*/) const
{
   return m_data.size();
}

int ModData::columnCount(const QModelIndex & /*parent*/) const
{
    return GraphPaint::DataCount;
}

QVariant ModData::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            switch (section) {
                case 0: return QString("Замер");
                case 1: return QString("Оригинал");
                case 2: return QString("Среднее арифметическое");
                case 3: return QString("Среднее квадратическое");
                case 4: return QString("Наименьшие квадраты");
            }
        else
        if (orientation == Qt::Vertical)
            return QString::number(section+1);
    }
    return QVariant();
}

QVariant ModData::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        auto &d = m_data[index.row()];
        const QString fmt("%0."+QString::number(m_fnum)+"f");
        return QString::asprintf(fmt.toLocal8Bit().data(), d.val[index.column()]);
    }
    else
    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return QVariant();
}

Qt::ItemFlags ModData::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) & ~(Qt::ItemIsEditable);
}

void ModData::setFNum(uint8_t fnum)
{
    m_fnum = fnum;
    emit layoutChanged();
}
