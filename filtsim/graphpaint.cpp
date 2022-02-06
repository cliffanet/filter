#include "graphpaint.h"
#include <QResizeEvent>
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>

#include <cmath>

GraphPaint::GraphPaint(QWidget *parent)
    : QWidget{parent}
{
    _info[DataAvg].filter = &_filterAvg;
    _info[DataAvg2].filter = &_filterAvg2;
    _info[DataLtSqrt].filter = &_filterLtSqrt;
}

void GraphPaint::tick(double val, uint32_t tm)
{
    tick(val, tm, -1);
}

void GraphPaint::tick(double val, uint32_t tm, double sigtrue)
{
    pnt_t p = {
        tm,
        { val, sigtrue }
    };
    for (int id = DataSrc+1; id < DataCount; id++) {
        auto &inf = _info[id];
        if (inf.filter != nullptr) {
            inf.filter->tick(val, tm);
            p.val[id] = inf.filter->value();
        }
    }

    _data.push_back(p);
    update();
}

void GraphPaint::clear()
{
    _data.clear();
    update();
}

void GraphPaint::setDataSize(size_t size)
{
    _data.resize(size);
    update();
}

void GraphPaint::setDataName(DataID id, const QString &name)
{
    _info[id].name = name;
    update();
}

void GraphPaint::setDataColor(DataID id, const QColor &color)
{
    _info[id].color = color;
    update();
}

void GraphPaint::setDataVisible(DataID id, bool visible)
{
    _info[id].visible = visible;
    update();
}

void GraphPaint::setDrawType(DataID id, DrawType type)
{
    _info[id].draw = type;
    update();
}

void GraphPaint::setSelected(int selected)
{
    if (m_selected == selected)
        return;
    m_selected = selected;
    update();
}

void GraphPaint::clearSelected()
{
    if (m_selected < 0)
        return;
    m_selected = -1;
    update();
}

void GraphPaint::setOffset(int x, int y)
{
    if (offset_x != x) {
        offset_x = x;
        emit offsetXChanged(offset_x);
    }

    if (offset_y != y) {
        offset_y = y;
        emit offsetYChanged(offset_y);
        emit valDrawRangeChanged();
    }
    update();
}

void GraphPaint::setScale(uint x, uint y, int fix_x, int fix_y)
{
    if (scale_x != x) {
        if (fix_x < 0)
            fix_x += size().width();
        int fix_ind = static_cast<int>(std::round(x2index(static_cast<double>(fix_x))));

        scale_x = x;

        int new_x = static_cast<int>(std::round(index2x(static_cast<double>(fix_ind))));
        if (new_x != fix_x) {
            offset_x -= new_x - fix_x;
            emit offsetXChanged(offset_x);
        }

        emit scaleXChanged(scale_x);
    }

    if (scale_y != y) {
        if (fix_y < 0)
            fix_y += size().height();
        int fix_val = static_cast<int>(std::round(y2value(static_cast<double>(fix_y))));

        scale_y = y;

        int new_y = static_cast<int>(std::round(value2y(static_cast<double>(fix_val))));
        if (new_y != fix_y) {
            offset_y += new_y - fix_y;
            emit offsetYChanged(offset_y);
        }

        emit scaleYChanged(scale_y);
        emit valDrawRangeChanged();
    }

    update();
}

void GraphPaint::setDrawFlags(DrawFlags flag, bool set)
{
    if (set)
        _flags |= flag;
    else
        _flags &= ~flag;

    update();
}

void GraphPaint::updateFilter(DataID id)
{
    if (id <= DataSrc)
        return;

    auto &inf = _info[id];
    if (inf.filter == nullptr)
        return;

    for (auto &d : _data) {
        inf.filter->tick(d.val[DataSrc], d.tdiff);
        d.val[id] = inf.filter->value();
    }

    update();
}

void GraphPaint::resizeAvg(size_t sz)
{
    _filterAvg.resize(sz);
    updateFilter(DataAvg);
}

void GraphPaint::resizeAvg2(size_t sz)
{
    _filterAvg2.resize(sz);
    updateFilter(DataAvg2);
}

void GraphPaint::resizeLtSqrt(size_t sz)
{
    _filterLtSqrt.resize(sz);
    updateFilter(DataLtSqrt);
}

void GraphPaint::resizeEvent(QResizeEvent *event)
{
    if (event->oldSize().height() != event->size().height())
        emit valDrawRangeChanged();
}

void GraphPaint::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    int h = size().height();
    //p.setRenderHint(QPainter::Antialiasing);

    if (_flags & DrawWhiteBg) {
        QRect rect(QPoint(0, 0), QSize(this->size().width()-1, h-1));
        //p.fillRect(rect, QBrush(Qt::white));

        QPainterPath path;
        path.addRoundedRect(rect, 7, 7);
        p.setPen(QPen(Qt::white, 1));
        p.fillPath(path, Qt::white);
        p.drawPath(path);
    }

    if (_flags & DrawBorder) {
        p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
        QRect rect(QPoint(0, 0), QSize(this->size().width()-1, h-1));
        p.drawRoundedRect(rect, 7, 7);
    }

    if ((m_selected >= 0) && (static_cast<uint>(m_selected) < _data.size())) {
        int x = index2x(m_selected);
        if ((x >= 0) && (x < size().width())) {
            p.setPen(QPen(Qt::black));
            p.drawLine(x, 0, x, size().width()-1);
        }
    }

    int ibeg = x2index(0);
    if (ibeg < 0)
        ibeg = 0;

    int src = 0;
    for (const auto &inf: _info) {
        if (!inf.visible) {
            src++;
            continue;
        }

        int index = ibeg;

        p.setPen(QPen(inf.color, 1, Qt::SolidLine, Qt::FlatCap));
        QPoint prv(index2x(index), static_cast<int>(std::round(value2y(_data[0].val[src]))));

        for (auto di = _data.begin()+ibeg; di != _data.end(); di++) {
            QPoint pnt(index2x(index), static_cast<int>(std::round(value2y(di->val[src]))));
            switch (inf.draw) {
                case DrawPoint:
                    p.drawPoint(pnt);
                    break;
                case DrawLine:
                    p.drawLine(prv, pnt);
                    prv = pnt;
                    break;
            }

            if (pnt.x() > size().width())
                break;
            index++;
        }

        src++;
    }

    /* отрисовка интервалов времени
     * позже надо будет сделать это так же опционально на графике,
     * как и все остальные
    int x = 0;
    p.setPen(QPen(Qt::cyan, 1, Qt::SolidLine, Qt::FlatCap));
    for (const auto &d : _data) {
        p.drawPoint(x, h-d.tdiff);
        x++;
    }
    */
}

void GraphPaint::wheelEvent(QWheelEvent *event)
{
    QPoint d = event->pixelDelta();
    int x = static_cast<int>(scale_x)+d.x();
    int y = static_cast<int>(scale_y)+d.y();
    setScale(
        x > 0 ? x : 1,
        y > 0 ? y : 1,
        event->position().x(),
        event->position().y()
    );
}

void GraphPaint::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        drag_pos = event->pos();
        int index = static_cast<int>(std::round(x2index(static_cast<double>(event->pos().x()))));
        if ((index >= 0) && (static_cast<uint>(index) < _data.size()))
            emit indexSelected(index);
    }
}

void GraphPaint::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && !drag_pos.isNull()) {
        QPoint offset = QPoint(offset_x, offset_y) - (event->pos() - drag_pos);
        drag_pos = event->pos();
        setOffset(offset.x(), offset.y());
    }
}
