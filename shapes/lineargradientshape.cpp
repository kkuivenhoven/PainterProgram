#include "lineargradientshape.h"

LinearGradientShape::LinearGradientShape() {

}

void LinearGradientShape::setCoords(int x1, int x2, int y1, int y2) {
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

void LinearGradientShape::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

void LinearGradientShape::setLinearGradient(QLinearGradient linearGradient) {
    m_linearGradient = linearGradient;
}

int LinearGradientShape::getX1() {
    return m_x1;
}

int LinearGradientShape::getX2() {
    return m_x2;
}

int LinearGradientShape::getY1() {
    return m_y1;
}

int LinearGradientShape::getY2() {
    return m_y2;
}

QLinearGradient LinearGradientShape::getLinearGradient() {
    return m_linearGradient;
}
