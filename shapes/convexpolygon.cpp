#include "convexpolygon.h"

ConvexPolygon::ConvexPolygon() {

}

void ConvexPolygon::addPointToQueue(QPointF pointF) {
    m_allPoints.append(pointF);
}

void ConvexPolygon::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

QQueue<QPointF> ConvexPolygon::getAllPoints() {
    return m_allPoints;
}
