#ifndef STRAIGHTLINE_H
#define STRAIGHTLINE_H

#include <QPoint>
#include <QColor>

class StraightLine {
    QPoint m_pointOne;
    QPoint m_pointTwo;
    int m_posInOrderOfActions;
    int m_penWidth;
    QColor m_penColor;

public:
    StraightLine();
    void setPoints(QPoint pointOne, QPoint pointTwo);
    void setPosInOrderOfActions(int posOfLastAction);
    void setPenWidth(int penWidth);
    void setPenColor(QColor penColor);

    QPoint getPointOne();
    QPoint getPointTwo();
    QColor getPenColor();
    int getPenWidth();
};

#endif // STRAIGHTLINE_H
