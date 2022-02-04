#ifndef GRAPHPAINT_H
#define GRAPHPAINT_H

#include <QWidget>

class QWheelEvent;
class QMouseEvent;

#include "../filtlib/ring.h"
#include "../filtlib/filter_avg.h"
#include "../filtlib/filter_avg2.h"
#include "../filtlib/filter_ltsqrt.h"

typedef FilterBase<double> filtBase;
typedef FilterAvg<double> filtAvg;
typedef FilterAvg2<double> filtAvg2;
typedef FilterLtSqrt<double> filtLtSqrt;

#define GRAPH_PAINT_SRC_COUNT   5

class GraphPaint : public QWidget
{
    Q_OBJECT
public:
    typedef enum {
        DataSrc = 0,
        DataTrue,
        DataAvg,
        DataAvg2,
        DataLtSqrt,
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
        bool    visible = true;
        DrawType draw = DrawPoint;
        filtBase *filter = nullptr;
    } hdr_t;

    explicit GraphPaint(QWidget *parent = nullptr);
    ~GraphPaint() override;

    void tick(double val, uint32_t tm);
    void tick(double val, uint32_t tm, double sigtrue);
    void clear();
    void setVMax(double val);
    void setDataSize(size_t size);
    void setDataName(DataID id, const QString &name);
    void setDataColor(DataID id, const QColor &color);
    void setDataVisible(DataID id, bool visible);
    void setDrawType(DataID id, DrawType type);
    void setFilter(DataID id, filtBase *filter);

    void setOffset(int x, int y);
    void setScale(uint x, uint y, int fix_x = -1, int fix_y = -1);

signals:
    void offsetXChanged(int newValue);
    void offsetYChanged(int newValue);
    void scaleXChanged(int newValue);
    void scaleYChanged(int newValue);

protected:
    void paintEvent(QPaintEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    hdr_t _info[DataCount];
    uint32_t dcur = 0;
    double vmax = 0;
    ring <pnt_t>_data;
    int offset_x = 0, offset_y = 0;
    uint scale_x = 1, scale_y = 1;
    QPoint drag_pos;

    template <typename T>
    inline T index2x(T index) {
        return
            static_cast<T>(size().width()) -
            (
                (static_cast<T>(_data.size()) + static_cast<T>(offset_x) - static_cast<T>(index)) *
                    static_cast<T>(scale_x) / 100
            );
    }

    template <typename T>
    inline T x2index(T x) {
        return
            static_cast<T>(_data.size()) + static_cast<T>(offset_x) -
            ((static_cast<T>(size().width()) - x) * 100 / static_cast<T>(scale_x));
    }

    template <typename T>
    inline T value2y(T value) {
        return
            static_cast<T>(size().height()) -
            ((value - static_cast<T>(offset_y)) * static_cast<T>(scale_y) / 100);
    }

    template <typename T>
    inline T y2value(T y) {
        return
            (static_cast<T>(size().height()) - y) * 100 / static_cast<T>(scale_y) +
            static_cast<T>(offset_y);
    }
};

#endif // GRAPHPAINT_H
