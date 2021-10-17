#include "ellipse.h"

Ellipse::Ellipse() {

}

void Ellipse::setCoords(int x1, int x2, int y1, int y2) {
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

void Ellipse::setPenWidth(int penWidth) {
    m_penWidth = penWidth;
}

void Ellipse::setPenColor(QColor penColor) {
    m_penColor = penColor;
}

void Ellipse::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

int Ellipse::getX1() {
    return m_x1;
}

int Ellipse::getX2() {
    return m_x2;
}

int Ellipse::getY1() {
    return m_y1;
}

int Ellipse::getY2() {
    return m_y2;
}

QColor Ellipse::getPenColor() {
    return m_penColor;
}

int Ellipse::getPenWidth() {
    return m_penWidth;
}
