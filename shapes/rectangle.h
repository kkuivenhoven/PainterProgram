#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QColor>

class Rectangle {
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    int m_penWidth;
    QColor m_penColor;
    int m_posInOrderOfActions;

public:
    Rectangle();
    void setCoords(int x1, int x2, int y1, int y2);
    void setPenWidth(int penWidth);
    void setPenColor(QColor penColor);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();
    QColor getPenColor();
    int getPenWidth();
};

#endif // RECTANGLE_H
