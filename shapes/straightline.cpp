#include "straightline.h"

StraightLine::StraightLine() {

}

void StraightLine::setPoints(QPoint pointOne, QPoint pointTwo) {
    m_pointOne = pointOne;
    m_pointTwo = pointTwo;
}

void StraightLine::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

void StraightLine::setPenWidth(int penWidth) {
    m_penWidth = penWidth;
}

void StraightLine::setPenColor(QColor penColor) {
    m_penColor = penColor;
}

QPoint StraightLine::getPointOne() {
    return m_pointOne;
}

QPoint StraightLine::getPointTwo() {
    return m_pointTwo;
}

QColor StraightLine::getPenColor() {
    return m_penColor;
}

int StraightLine::getPenWidth() {
    return m_penWidth;
}
