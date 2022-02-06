#ifndef GRAPHPAINT_H
#define GRAPHPAINT_H

#include <QWidget>

class QResizeEvent;
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

    typedef enum {
        DrawWhiteBg = 0x01,
        DrawBorder = 0x02,
    } DrawFlags;

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

    void tick(double val, uint32_t tm);
    void tick(double val, uint32_t tm, double sigtrue);
    void clear();
    void setVMax(double val);
    void setDataSize(size_t size);
    void setDataName(DataID id, const QString &name);
    void setDataColor(DataID id, const QColor &color);
    void setDataVisible(DataID id, bool visible);
    void setDrawType(DataID id, DrawType type);

    int offsetX() const { return offset_x; }
    int offsetY() const { return offset_y; }
    void setOffset(int x, int y);
    uint scaleX() const { return scale_x; }
    uint scaleY() const { return scale_y; }
    void setScale(uint x, uint y, int fix_x = -1, int fix_y = -1);

    void setDrawFlags(DrawFlags flag, bool set = true);

    const filtAvg   & filterAvg()   const { return _filterAvg; }
    const filtAvg2  & filterAvg2()  const { return _filterAvg2; }
    const filtLtSqrt& filterLtSqrt()const { return _filterLtSqrt; }

    void updateFilter(DataID id);
    void resizeAvg(size_t sz);
    void resizeAvg2(size_t sz);
    void resizeLtSqrt(size_t sz);

    double valDrawMin() { return y2value(static_cast<double>(size().height()-1)); }
    double valDrawMax() { return y2value(static_cast<double>(0)); }

signals:
    void offsetXChanged(int newValue);
    void offsetYChanged(int newValue);
    void scaleXChanged(int newValue);
    void scaleYChanged(int newValue);
    void valDrawRangeChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    uint32_t _flags = 0;
    hdr_t _info[DataCount];
    uint32_t dcur = 0;
    double vmax = 0;
    ring <pnt_t>_data;
    int offset_x = 0, offset_y = 0;
    uint scale_x = 1, scale_y = 1;
    QPoint drag_pos;

    filtAvg _filterAvg;
    filtAvg2 _filterAvg2;
    filtLtSqrt _filterLtSqrt;

    template <typename T>
    inline T index2x(T index) {
        return
            static_cast<T>(size().width()) -
            (
                (static_cast<T>(_data.size()) - index) *
                    static_cast<T>(scale_x) / 100
            ) -
            static_cast<T>(offset_x);
    }

    template <typename T>
    inline T x2index(T x) {
        return
            static_cast<T>(_data.size()) -
            (
                (
                    static_cast<T>(size().width()) -
                    static_cast<T>(offset_x) -
                    x
                ) *
                100 / static_cast<T>(scale_x)
            );
    }

    template <typename T>
    inline T value2y(T value) {
        return
            static_cast<T>(size().height()) -
            (value * static_cast<T>(scale_y) / 100) -
            static_cast<T>(offset_y);
    }

    template <typename T>
    inline T y2value(T y) {
        return
            (
                static_cast<T>(size().height()) -
                static_cast<T>(offset_y) -
                y
            ) *
            100 / static_cast<T>(scale_y);
    }
};

#endif // GRAPHPAINT_H
