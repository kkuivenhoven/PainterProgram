#ifndef STRAIGHTLINE_H
#define STRAIGHTLINE_H

#include <QPoint>

class StraightLine {
    QPoint m_pointOne;
    QPoint m_pointTwo;
    int m_posInOrderOfActions;

public:
    StraightLine();
    void setPoints(QPoint pointOne, QPoint pointTwo);
    void setPosInOrderOfActions(int posOfLastAction);

    QPoint getPointOne();
    QPoint getPointTwo();
};

#endif // STRAIGHTLINE_H
