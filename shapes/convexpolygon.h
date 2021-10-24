#ifndef CONVEXPOLYGON_H
#define CONVEXPOLYGON_H

#include <QQueue>
#include <QPointF>
#include <QColor>

class ConvexPolygon {
    QQueue<QPointF> m_allPoints;
    int m_penWidth;
    QColor m_penColor;
    int m_posInOrderOfActions;

public:
    ConvexPolygon();

    void addPointToQueue(QPointF pointF);
    void setPenWidth(int penWidth);
    void setPenColor(QColor penColor);
    void setPosInOrderOfActions(int posOfLastAction);

    QQueue<QPointF> getAllPoints();
    QColor getPenColor();
    int getPenWidth();
};

#endif // CONVEXPOLYGON_H
