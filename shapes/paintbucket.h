#ifndef PAINTBUCKET_H
#define PAINTBUCKET_H

#include <QColor>

class PaintBucket {
    QColor m_penColor;
    int m_posInOrderOfActions;

public:
    PaintBucket();
    void setPosInOrderOfActions(int posOfLastAction);
    void setPenColor(QColor penColor);
    QColor getPenColor();
};

#endif // PAINTBUCKET_H
