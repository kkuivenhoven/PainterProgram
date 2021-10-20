#include "freehandline.h"

FreeHandLine::FreeHandLine() {

}

void FreeHandLine::setNewPoint(QPoint curPoint) {
    m_allPoints.append(curPoint);
}

void FreeHandLine::setPenWidth(int penWidth) {
    m_penWidth = penWidth;
}

void FreeHandLine::setPenColor(QColor penColor) {
    m_penColor = penColor;
}

void FreeHandLine::setPosInOrderOfActions(int posOfLastAction)  {
    m_posInOrderOfActions = posOfLastAction;
}

QColor FreeHandLine::getPenColor() {
    return m_penColor;
}

int FreeHandLine::getPenWidth() {
    return m_penWidth;
}

QQueue<QPoint> FreeHandLine::getAllSetPoints() {
    return m_allPoints;
}

