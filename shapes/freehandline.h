#ifndef FREEHANDLINE_H
#define FREEHANDLINE_H

#include <QPoint>
#include <QColor>
#include <QQueue>

class FreeHandLine {
    QQueue<QPoint> m_allPoints;
    int m_penWidth;
    QColor m_penColor;
    int m_posInOrderOfActions;

public:
    FreeHandLine();

    void setNewPoint(QPoint curPoint);
    void setPenWidth(int penWidth);
    void setPenColor(QColor penColor);
    void setPosInOrderOfActions(int posOfLastAction);

    QColor getPenColor();
    int getPenWidth();
    QQueue<QPoint> getAllSetPoints();
};

#endif // FREEHANDLINE_H
