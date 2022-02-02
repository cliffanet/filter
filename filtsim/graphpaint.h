#ifndef GRAPHPAINT_H
#define GRAPHPAINT_H

#include <QWidget>

#include "../filtlib/ring.h"
#include "../filtlib/filter_avg.h"
#include "../filtlib/filter_avg2.h"

typedef FilterBase<double> filtBase;
typedef FilterAvg<double> filtAvg;
typedef FilterAvg2<double> filtAvg2;

#define GRAPH_PAINT_SRC_COUNT   5

class GraphPaint : public QWidget
{
    Q_OBJECT
public:
    typedef enum {
        DataSrc = 0,
        DataAvg,
        DataAvg2,
        DataCount
    } DataID;

    typedef enum {
        DrawPoint,
        DrawLine
    } DrawType;

    typedef struct gpnt {
        uint32_t tdiff = 0;
        double val[DataCount] = { 0 };
    } pnt_t;

    typedef struct ghdr {
        QString name;
        QColor color;
        DrawType draw = DrawPoint;
        filtBase *filter = nullptr;
    } hdr_t;

    explicit GraphPaint(QWidget *parent = nullptr);
    ~GraphPaint() override;

    void tick(double val, uint32_t tm);
    void clear();
    void setVMax(double val);
    void setDataName(DataID id, const QString &name);
    void setDataColor(DataID id, const QColor &color);
    void setDrawType(DataID id, DrawType type);
    void setFilter(DataID id, filtBase *filter);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    hdr_t _info[DataCount];
    uint32_t dcur = 0;
    double vmax = 0;
    ring <pnt_t>_data;
};

#endif // GRAPHPAINT_H
