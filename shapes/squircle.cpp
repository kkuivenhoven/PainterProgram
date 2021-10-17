#include "squircle.h"

Squircle::Squircle() {

}

void Squircle::setPainterPath(QPainterPath painterPath) {
    m_painterPath = painterPath;
}

void Squircle::setPenWidth(int penWidth) {
    m_penWidth = penWidth;

}

void Squircle::setPenColor(QColor penColor) {
    m_penColor = penColor;

}

void Squircle::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;

}

QColor Squircle::getPenColor() {
    return m_penColor;
}

int Squircle::getPenWidth() {
    return m_penWidth;
}

QPainterPath Squircle::getPainterPath() {
    return m_painterPath;
}
