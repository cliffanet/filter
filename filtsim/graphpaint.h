#ifndef GRAPHPAINT_H
#define GRAPHPAINT_H

#include <QWidget>

#include "../filtlib/ring.h"

typedef struct gpnt {
    uint32_t tdiff = 0;
    double val[4] = { 0 };
} gpnt_t;

typedef struct ghdr {
    QString name;
} ghdr_t;

class GraphPaint : public QWidget
{
    Q_OBJECT
public:
    explicit GraphPaint(QWidget *parent = nullptr);

    void tick(double val, uint32_t tm);
    void clear();
    void setVMax(double val);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    ghdr_t hdr;
    uint32_t dcur = 0;
    double vmax = 0;
    ring <gpnt_t>_data;
};

#endif // GRAPHPAINT_H
