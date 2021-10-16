#include "toolsethandling.h"


const QString ToolSetHandling::RECTANGLE("RECTANGLE");


ToolSetHandling::ToolSetHandling() {

}


QStack<QString> ToolSetHandling::verifyLastAction() {
    return _orderOfActions;
}


void ToolSetHandling::addLastActionToStack(QString objType) {
    _orderOfActions.append(objType);
}


void ToolSetHandling::addRectangleToStack(Rectangle rectangle) {
    addLastActionToStack(RECTANGLE);
    rectangle.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _rectangleStack.append(rectangle);
    _rectangleQueue.append(rectangle);
}


int ToolSetHandling::getPositionOfLastActionAdded() {
    return (_orderOfActions.size()-1);
}


QStack<QString> ToolSetHandling::getOrderOfObjectsDrawn() {
    return _orderOfActions;
}


QStack<Rectangle> ToolSetHandling::getOrderOfRectangles() {
    return _rectangleStack;
}


void ToolSetHandling::removeLastRectangle() {
    _rectangleStack.pop();
    _rectangleQueue.pop_back();
}


void ToolSetHandling::removeLastActionFromStack() {
    _orderOfActions.pop();
}


QQueue<Rectangle> ToolSetHandling::getQueueOfRectangles() {
    return _rectangleQueue;
}


void ToolSetHandling::removeFrontRectFromQueue() {
    _rectangleQueue.removeFirst();
}
