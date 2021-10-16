#ifndef TOOLSETHANDLING_H
#define TOOLSETHANDLING_H

#include <QStack>
#include <QQueue>
#include "shapes/rectangle.h"

class ToolSetHandling {
    QStack<Rectangle> _rectangleStack;
    QQueue<Rectangle> _rectangleQueue;
    QStack<QString> _orderOfActions;

public:
    ToolSetHandling();
    QStack<QString> verifyLastAction();
    void addLastActionToStack(QString objType);
    QStack<QString> getOrderOfObjectsDrawn();
    QStack<Rectangle> getOrderOfRectangles();
    void removeLastRectangle();
    int getPositionOfLastActionAdded();
    QQueue<Rectangle> getQueueOfRectangles();
    void removeFrontRectFromQueue();
    void addRectangleToStack(Rectangle rectangle);
    void removeLastActionFromStack();

    static const QString RECTANGLE;
};

#endif // TOOLSETHANDLING_H
