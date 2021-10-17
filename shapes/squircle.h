#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QColor>
#include <QPainterPath>

class Squircle {
    QPainterPath m_painterPath;
    int m_penWidth;
    QColor m_penColor;
    int m_posInOrderOfActions;

public:
    Squircle();
    void setPainterPath(QPainterPath painterPath);
    void setPenWidth(int penWidth);
    void setPenColor(QColor penColor);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();
    QColor getPenColor();
    int getPenWidth();
    QPainterPath getPainterPath();
};

#endif // SQUIRCLE_H
