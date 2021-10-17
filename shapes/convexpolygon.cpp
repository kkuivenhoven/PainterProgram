#include "convexpolygon.h"

ConvexPolygon::ConvexPolygon() {

}

void ConvexPolygon::addPointToQueue(QPointF pointF) {
    _allPoints.append(pointF);
}

void ConvexPolygon::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

QQueue<QPointF> ConvexPolygon::getAllPoints() {
    return _allPoints;
}
