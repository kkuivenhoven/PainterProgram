#ifndef CONICALGRADIENTSHAPE_H
#define CONICALGRADIENTSHAPE_H

#include <QConicalGradient>

class ConicalGradientShape {
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    QConicalGradient m_conicalGradient;
    int m_posInOrderOfActions;

public:
    ConicalGradientShape();
    void setCoords(int x1, int x2, int y1, int y2);
    void setConicalGradient(QConicalGradient linearGradient);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();
    QConicalGradient getConicalGradient();
};

#endif // CONICALGRADIENTSHAPE_H
