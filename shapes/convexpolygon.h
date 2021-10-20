#ifndef CONVEXPOLYGON_H
#define CONVEXPOLYGON_H

#include <QQueue>
#include <QPointF>

class ConvexPolygon {
    QQueue<QPointF> m_allPoints;
    int m_posInOrderOfActions;

public:
    ConvexPolygon();

    void addPointToQueue(QPointF pointF);
    void setPosInOrderOfActions(int posOfLastAction);
    QQueue<QPointF> getAllPoints();
};

#endif // CONVEXPOLYGON_H
