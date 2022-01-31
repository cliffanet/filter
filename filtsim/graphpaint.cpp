#include "graphpaint.h"
#include <QPainter>
#include <QDebug>

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

void GraphPaint::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter p(this);
    int h = this->size().height();
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    QRect rect(QPoint(0, 0), QSize(this->size().width()-1, h-1));
    p.drawRect(rect);

    int x = 0;
    p.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
    for (const auto &d: _data) {
        x++;
        p.drawPoint(x, h - static_cast<int>(d.val[0]*h/vmax));
    }
}

void GraphPaint::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)
    //qDebug() << "resize: " << this->size().width();
}
