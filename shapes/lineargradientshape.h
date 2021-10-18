#ifndef LINEARGRADIENTSHAPE_H
#define LINEARGRADIENTSHAPE_H

#include <QLinearGradient>

class LinearGradientShape {
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    QLinearGradient m_linearGradient;
    int m_posInOrderOfActions;

public:
    LinearGradientShape();
    void setCoords(int x1, int x2, int y1, int y2);
    void setLinearGradient(QLinearGradient linearGradient);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();
    QLinearGradient getLinearGradient();
};

#endif // LINEARGRADIENTSHAPE_H
