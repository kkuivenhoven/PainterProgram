#include "convexpolygon.h"

ConvexPolygon::ConvexPolygon() {

}

void ConvexPolygon::addPointToQueue(QPointF pointF) {
    m_allPoints.append(pointF);
}

void ConvexPolygon::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

void ConvexPolygon::setPenWidth(int penWidth) {
    m_penWidth = penWidth;
}

void ConvexPolygon::setPenColor(QColor penColor) {
    m_penColor = penColor;
}

QQueue<QPointF> ConvexPolygon::getAllPoints() {
    return m_allPoints;
}

QColor ConvexPolygon::getPenColor() {
    return m_penColor;
}

int ConvexPolygon::getPenWidth() {
    return m_penWidth;
}
