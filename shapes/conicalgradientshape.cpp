#include "conicalgradientshape.h"

ConicalGradientShape::ConicalGradientShape() {

}

void ConicalGradientShape::setCoords(int x1, int x2, int y1, int y2) {
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

void ConicalGradientShape::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

void ConicalGradientShape::setConicalGradient(QConicalGradient conicalGradient) {
    m_conicalGradient = conicalGradient;
}

int ConicalGradientShape::getX1() {
    return m_x1;
}

int ConicalGradientShape::getX2() {
    return m_x2;
}

int ConicalGradientShape::getY1() {
    return m_y1;
}

int ConicalGradientShape::getY2() {
    return m_y2;
}

QConicalGradient ConicalGradientShape::getConicalGradient() {
    return m_conicalGradient;
}
