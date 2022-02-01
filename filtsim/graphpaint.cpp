#include "graphpaint.h"
#include <QPainter>
#include <QDebug>

#include <cmath>

GraphPaint::GraphPaint(QWidget *parent)
    : QWidget{parent}
{

}

void GraphPaint::tick(double val, uint32_t tm)
{
    _data.push_back({ tm, { val } });
    update();
}

void GraphPaint::clear()
{
    _data.clear();
    update();
}

void GraphPaint::setVMax(double val)
{
    vmax = val;
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

void GraphPaint::setDrawType(DataID id, DrawType type)
{
    _info[id].draw = type;
    update();
}

void GraphPaint::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter p(this);
    int h = this->size().height();
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    QRect rect(QPoint(0, 0), QSize(this->size().width()-1, h-1));
    p.drawRect(rect);

    int src = 0;
    for (const auto &inf: _info) {
        int x = 0;

        p.setPen(QPen(inf.color, 1, Qt::SolidLine, Qt::FlatCap));
 #define y_calc(val) h - 1 - static_cast<int>(std::round((val)*(h-3)/vmax))
        QPoint prv(x, y_calc(_data[0].val[src]));

        for (const auto &d : _data) {
            int y = y_calc(d.val[src]);
            switch (inf.draw) {
                case DrawPoint:
                    p.drawPoint(x, y);
                    break;
                case DrawLine: {
                    QPoint nxt(x, y);
                    p.drawLine(prv, nxt);
                    prv = nxt;
                }
            }

            x++;
        }

        src++;
    }
}

void GraphPaint::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)
    _data.resize(static_cast<uint>(this->size().width()));
}
