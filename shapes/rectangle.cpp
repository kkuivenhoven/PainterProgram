#include "rectangle.h"

Rectangle::Rectangle() {

}

void Rectangle::setCoords(int x1, int x2, int y1, int y2) {
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

void Rectangle::setPenWidth(int penWidth) {
    m_penWidth = penWidth;
}

void Rectangle::setPenColor(QColor penColor) {
    m_penColor = penColor;
}

void Rectangle::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

int Rectangle::getX1() {
    return m_x1;
}

int Rectangle::getX2() {
    return m_x2;
}

int Rectangle::getY1() {
    return m_y1;
}

int Rectangle::getY2() {
    return m_y2;
}

QColor Rectangle::getPenColor() {
    return m_penColor;
}

int Rectangle::getPenWidth() {
    return m_penWidth;
}
