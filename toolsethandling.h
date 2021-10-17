#ifndef TOOLSETHANDLING_H
#define TOOLSETHANDLING_H

#include <QStack>
#include <QQueue>
#include <QMap>
#include <QDebug>

#include "shapes/rectangle.h"
#include "shapes/ellipse.h"

class ToolSetHandling {
    QStack<QString> _orderOfActions;

    QQueue<Rectangle> _rectangleQueue;
    QQueue<Ellipse> _ellipseQueue;

    QMap<int /*positionInActions*/,
         int /*posInShapeStack*/> _posMap;

public:
    ToolSetHandling();
    QStack<QString> verifyLastAction();
    void addLastActionToStack(QString objType);
    QStack<QString> getOrderOfObjectsDrawn();
    int getPositionOfLastActionAdded();
    void removeLastActionFromStack();

    void setActionPosAndShapePos(int actionPos, int shapePos);
    QMap<int, int> getPosMap();
    void removeLastPosStored(int keyValueToRemove);

    static const QString RECTANGLE;
    void addRectangleToQueue(Rectangle rectangle);
    void removeLastRectangle();
    QQueue<Rectangle> getQueueOfRectangles();
    void removeFrontRectFromQueue();

    static const QString ELLIPSE;
    void addEllipseToQueue(Ellipse ellipse);
    void removeLastEllipse();
    QQueue<Ellipse> getQueueOfEllipses();
    void removeFrontEllipseFromQueue();

};

#endif // TOOLSETHANDLING_H
