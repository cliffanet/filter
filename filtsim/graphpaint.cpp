#include "graphpaint.h"
#include <QPainter>
#include <QDebug>

GraphPaint::GraphPaint(QWidget *parent)
    : QWidget{parent}
{

}

void GraphPaint::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    QRect rect(QPoint(0, 0), QSize(this->size().width()-1, this->size().height()-1));
    p.drawRect(rect);
}

void GraphPaint::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    //update();
}
