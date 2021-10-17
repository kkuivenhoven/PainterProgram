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

QPoint StraightLine::getPointOne() {
    return m_pointOne;
}

QPoint StraightLine::getPointTwo() {
    return m_pointTwo;
}

