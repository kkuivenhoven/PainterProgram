#include "toolsethandling.h"


const QString ToolSetHandling::RECTANGLE("RECTANGLE");
const QString ToolSetHandling::ELLIPSE("ELLIPSE");
const QString ToolSetHandling::SQUIRCLE("SQUIRCLE");
const QString ToolSetHandling::FREE_HAND_LINE("FREE_HAND_LINE");
const QString ToolSetHandling::CONVEX_POLYGON("CONVEX_POLYGON");
const QString ToolSetHandling::STRAIGHT_LINE("STRAIGHT_LINE");
const QString ToolSetHandling::LINEAR_GRADIENT_SHAPE("LINEAR_GRADIENT_SHAPE");
const QString ToolSetHandling::CONICAL_GRADIENT_SHAPE("CONICAL_GRADIENT_SHAPE");
const QString ToolSetHandling::RADIAL_GRADIENT_SHAPE("RADIAL_GRADIENT_SHAPE");
const QString ToolSetHandling::TEXT_BOX("TEXT_BOX");


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


void ToolSetHandling::addSquircleToQueue(Squircle squircle) {
    addLastActionToStack(SQUIRCLE);
    squircle.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _squircleQueue.append(squircle);
}

void ToolSetHandling::removeLastSquircle() {
    _squircleQueue.pop_back();
}

QQueue<Squircle> ToolSetHandling::getQueueOfSquircles() {
    return _squircleQueue;
}

void ToolSetHandling::removeFrontSquircleFromQueue() {
    _squircleQueue.removeFirst();
}


void ToolSetHandling::addFreeHandLineToQueue(FreeHandLine freeHandLine) {
    addLastActionToStack(FREE_HAND_LINE);
    freeHandLine.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _freeHandLineQueue.append(freeHandLine);
}

void ToolSetHandling::removeLastFreeHandLine() {
    _freeHandLineQueue.pop_back();
}

QQueue<FreeHandLine> ToolSetHandling::getQueueOfFreeHandLines() {
    return _freeHandLineQueue;
}

void ToolSetHandling::removeFrontFreeHandLineFromQueue() {
    _freeHandLineQueue.removeFirst();
}

FreeHandLine& ToolSetHandling::obtainCurFreeHandLineInstance() {
    return _freeHandLineQueue.last();
}


void ToolSetHandling::addConvexPolygonToQueue(ConvexPolygon convexPolygon) {
    addLastActionToStack(CONVEX_POLYGON);
    convexPolygon.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _convexPolygonQueue.append(convexPolygon);
}

void ToolSetHandling::removeLastConvexPolygon() {
    _convexPolygonQueue.pop_back();
}

QQueue<ConvexPolygon> ToolSetHandling::getQueueOfConvexPolygons() {
    return _convexPolygonQueue;
}


void ToolSetHandling::addStraightLineToQueue(StraightLine straightLine) {
    addLastActionToStack(STRAIGHT_LINE);
    straightLine.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _straightLineQueue.append(straightLine);
}

void ToolSetHandling::removeLastStraightLine() {
    _straightLineQueue.pop_back();
}

QQueue<StraightLine> ToolSetHandling::getQueueOfStraightLines() {
    return _straightLineQueue;
}


void ToolSetHandling::addLinearGradientShapeToQueue(LinearGradientShape linearGradientShape) {
    addLastActionToStack(LINEAR_GRADIENT_SHAPE);
    linearGradientShape.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _linearGradientShapeQueue.append(linearGradientShape);
}

void ToolSetHandling::removeLastLinearGradientShape() {
    _linearGradientShapeQueue.pop_back();
}

QQueue<LinearGradientShape> ToolSetHandling::getQueueOfLinearGradientShapes() {
    return _linearGradientShapeQueue;
}

void ToolSetHandling::updateLinearGradient(QLinearGradient linearGradient) {
    int indexLastElement = (_linearGradientShapeQueue.size() - 1);
    LinearGradientShape linearGradientShape = _linearGradientShapeQueue.at(indexLastElement);
    _linearGradientShapeQueue.pop_back();
    linearGradientShape.setLinearGradient(linearGradient);
    _linearGradientShapeQueue.append(linearGradientShape);
}


void ToolSetHandling::addConicalGradientShapeToQueue(ConicalGradientShape conicalGradientShape) {
    addLastActionToStack(CONICAL_GRADIENT_SHAPE);
    conicalGradientShape.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _conicalGradientShapeQueue.append(conicalGradientShape);
}

void ToolSetHandling::removeLastConicalGradientShape() {
    _conicalGradientShapeQueue.pop_back();
}

QQueue<ConicalGradientShape> ToolSetHandling::getQueueOfConicalGradientShapes() {
    return _conicalGradientShapeQueue;
}

void ToolSetHandling::updateConicalGradient(QConicalGradient conicalGradient) {
    int indexLastElement = (_conicalGradientShapeQueue.size() - 1);
    ConicalGradientShape conicalGradientShape = _conicalGradientShapeQueue.at(indexLastElement);
    _conicalGradientShapeQueue.pop_back();
    conicalGradientShape.setConicalGradient(conicalGradient);
    _conicalGradientShapeQueue.append(conicalGradientShape);
}


void ToolSetHandling::addRadialGradientShapeToQueue(RadialGradientShape radialGradientShape) {
    addLastActionToStack(RADIAL_GRADIENT_SHAPE);
    radialGradientShape.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _radialGradientShapeQueue.append(radialGradientShape);
}

void ToolSetHandling::removeLastRadialGradientShape() {
    _radialGradientShapeQueue.pop_back();
}

QQueue<RadialGradientShape> ToolSetHandling::getQueueOfRadialGradientShapes() {
    return _radialGradientShapeQueue;
}

void ToolSetHandling::updateRadialGradient(QRadialGradient radialGradient) {
    int indexLastElement = (_radialGradientShapeQueue.size() - 1);
    RadialGradientShape radialGradientShape = _radialGradientShapeQueue.at(indexLastElement);
    _radialGradientShapeQueue.pop_back();
    radialGradientShape.setRadialGradient(radialGradient);
    _radialGradientShapeQueue.append(radialGradientShape);
}


void ToolSetHandling::addTextBoxToQueue(TextBox textBox) {
    addLastActionToStack(TEXT_BOX);
    textBox.setPosInOrderOfActions(getPositionOfLastActionAdded());
    _textBoxQueue.append(textBox);
}

void ToolSetHandling::removeLastTextBox() {
    _textBoxQueue.pop_back();
}

QQueue<TextBox> ToolSetHandling::getQueueOfTextBoxes() {
    return _textBoxQueue;
}

void ToolSetHandling::updateTextBox(QString textWritten) {
    int indexLastElement = (_textBoxQueue.size() - 1);
    TextBox textBox = _textBoxQueue.at(indexLastElement);
    _textBoxQueue.pop_back();
    textBox.setWrittenText(textWritten);
    _textBoxQueue.append(textBox);
}
