#ifndef RADIALGRADIENTSHAPE_H
#define RADIALGRADIENTSHAPE_H

#include <QRadialGradient>

class RadialGradientShape {
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    QRadialGradient m_radialGradient;
    int m_posInOrderOfActions;

public:
    RadialGradientShape();
    void setCoords(int x1, int x2, int y1, int y2);
    void setRadialGradient(QRadialGradient radialGradient);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();
    QRadialGradient getRadialGradient();
};

#endif // RADIALGRADIENTSHAPE_H
