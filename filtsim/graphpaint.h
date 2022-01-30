#ifndef GRAPHPAINT_H
#define GRAPHPAINT_H

#include <QWidget>

class GraphPaint : public QWidget
{
    Q_OBJECT
public:
    explicit GraphPaint(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

signals:

};

#endif // GRAPHPAINT_H
