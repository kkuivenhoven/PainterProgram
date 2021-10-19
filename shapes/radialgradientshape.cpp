#include "radialgradientshape.h"

RadialGradientShape::RadialGradientShape() {

}

void RadialGradientShape::setCoords(int x1, int x2, int y1, int y2) {
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

void RadialGradientShape::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

void RadialGradientShape::setRadialGradient(QRadialGradient radialGradient) {
    m_radialGradient = radialGradient;
}

int RadialGradientShape::getX1() {
    return m_x1;
}

int RadialGradientShape::getX2() {
    return m_x2;
}

int RadialGradientShape::getY1() {
    return m_y1;
}

int RadialGradientShape::getY2() {
    return m_y2;
}

QRadialGradient RadialGradientShape::getRadialGradient() {
    return m_radialGradient;
}
