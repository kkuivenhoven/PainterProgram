#include "toolsethandling.h"


const QString ToolSetHandling::RECTANGLE("RECTANGLE");
const QString ToolSetHandling::ELLIPSE("ELLIPSE");


ToolSetHandling::ToolSetHandling() {

}


QStack<QString> ToolSetHandling::verifyLastAction() {
    return _orderOfActions;
}

void ToolSetHandling::addLastActionToStack(QString objType) {
    _orderOfActions.append(objType);
}

QStack<QString> ToolSetHandling::getOrderOfObjectsDrawn() {
    return _orderOfActions;
}

int ToolSetHandling::getPositionOfLastActionAdded() {
    return (_orderOfActions.size()-1);
}

void ToolSetHandling::removeLastActionFromStack() {
    _orderOfActions.pop();
}

void ToolSetHandling::setActionPosAndShapePos(int actionPos,
                                              int shapePos) {
    _posMap.insert(actionPos, shapePos);
}

QMap<int, int> ToolSetHandling::getPosMap() {
    return _posMap;
}

void ToolSetHandling::removeLastPosStored(int keyValueToRemove) {
    _posMap.remove(keyValueToRemove);
}

void ToolSetHandling::addRectangleToQueue(Rectangle rectangle) {
    addLastActionToStack(RECTANGLE);
    rectangle.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _rectangleQueue.append(rectangle);
}

void ToolSetHandling::removeLastRectangle() {
    _rectangleQueue.pop_back();
}

QQueue<Rectangle> ToolSetHandling::getQueueOfRectangles() {
    return _rectangleQueue;
}

void ToolSetHandling::removeFrontRectFromQueue() {
    _rectangleQueue.removeFirst();
}


 void ToolSetHandling::addEllipseToQueue(Ellipse ellipse) {
    qDebug() << " adding ellipse to queue";
    addLastActionToStack(ELLIPSE);
    ellipse.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _ellipseQueue.append(ellipse);
}

void ToolSetHandling::removeLastEllipse() {
    _ellipseQueue.pop_back();
}

QQueue<Ellipse> ToolSetHandling::getQueueOfEllipses() {
    return _ellipseQueue;
}

void ToolSetHandling::removeFrontEllipseFromQueue() {
    _ellipseQueue.removeFirst();
}


