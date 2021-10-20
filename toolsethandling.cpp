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
    return m_orderOfActions;
}

void ToolSetHandling::addLastActionToStack(QString objType) {
    m_orderOfActions.append(objType);
}

QStack<QString> ToolSetHandling::getOrderOfObjectsDrawn() {
    return m_orderOfActions;
}

int ToolSetHandling::getPositionOfLastActionAdded() {
    return (m_orderOfActions.size()-1);
}

void ToolSetHandling::removeLastActionFromStack() {
    m_orderOfActions.pop();
}

void ToolSetHandling::setActionPosAndShapePos(int actionPos,
                                              int shapePos) {
    m_posMap.insert(actionPos, shapePos);
}

QMap<int, int> ToolSetHandling::getPosMap() {
    return m_posMap;
}

void ToolSetHandling::removeLastPosStored(int keyValueToRemove) {
    m_posMap.remove(keyValueToRemove);
}

void ToolSetHandling::addRectangleToQueue(Rectangle rectangle) {
    addLastActionToStack(RECTANGLE);
    rectangle.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_rectangleQueue.append(rectangle);
}

void ToolSetHandling::removeLastRectangle() {
    m_rectangleQueue.pop_back();
}

QQueue<Rectangle> ToolSetHandling::getQueueOfRectangles() {
    return m_rectangleQueue;
}

void ToolSetHandling::removeFrontRectFromQueue() {
    m_rectangleQueue.removeFirst();
}


void ToolSetHandling::addEllipseToQueue(Ellipse ellipse) {
    addLastActionToStack(ELLIPSE);
    ellipse.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_ellipseQueue.append(ellipse);
}

void ToolSetHandling::removeLastEllipse() {
    m_ellipseQueue.pop_back();
}

QQueue<Ellipse> ToolSetHandling::getQueueOfEllipses() {
    return m_ellipseQueue;
}

void ToolSetHandling::removeFrontEllipseFromQueue() {
    m_ellipseQueue.removeFirst();
}


void ToolSetHandling::addSquircleToQueue(Squircle squircle) {
    addLastActionToStack(SQUIRCLE);
    squircle.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_squircleQueue.append(squircle);
}

void ToolSetHandling::removeLastSquircle() {
    m_squircleQueue.pop_back();
}

QQueue<Squircle> ToolSetHandling::getQueueOfSquircles() {
    return m_squircleQueue;
}

void ToolSetHandling::removeFrontSquircleFromQueue() {
    m_squircleQueue.removeFirst();
}


void ToolSetHandling::addFreeHandLineToQueue(FreeHandLine freeHandLine) {
    addLastActionToStack(FREE_HAND_LINE);
    freeHandLine.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_freeHandLineQueue.append(freeHandLine);
}

void ToolSetHandling::removeLastFreeHandLine() {
    m_freeHandLineQueue.pop_back();
}

QQueue<FreeHandLine> ToolSetHandling::getQueueOfFreeHandLines() {
    return m_freeHandLineQueue;
}

void ToolSetHandling::removeFrontFreeHandLineFromQueue() {
    m_freeHandLineQueue.removeFirst();
}

FreeHandLine& ToolSetHandling::obtainCurFreeHandLineInstance() {
    return m_freeHandLineQueue.last();
}


void ToolSetHandling::addConvexPolygonToQueue(ConvexPolygon convexPolygon) {
    addLastActionToStack(CONVEX_POLYGON);
    convexPolygon.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_convexPolygonQueue.append(convexPolygon);
}

void ToolSetHandling::removeLastConvexPolygon() {
    m_convexPolygonQueue.pop_back();
}

QQueue<ConvexPolygon> ToolSetHandling::getQueueOfConvexPolygons() {
    return m_convexPolygonQueue;
}


void ToolSetHandling::addStraightLineToQueue(StraightLine straightLine) {
    addLastActionToStack(STRAIGHT_LINE);
    straightLine.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_straightLineQueue.append(straightLine);
}

void ToolSetHandling::removeLastStraightLine() {
    m_straightLineQueue.pop_back();
}

QQueue<StraightLine> ToolSetHandling::getQueueOfStraightLines() {
    return m_straightLineQueue;
}


void ToolSetHandling::addLinearGradientShapeToQueue(LinearGradientShape linearGradientShape) {
    addLastActionToStack(LINEAR_GRADIENT_SHAPE);
    linearGradientShape.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_linearGradientShapeQueue.append(linearGradientShape);
}

void ToolSetHandling::removeLastLinearGradientShape() {
    m_linearGradientShapeQueue.pop_back();
}

QQueue<LinearGradientShape> ToolSetHandling::getQueueOfLinearGradientShapes() {
    return m_linearGradientShapeQueue;
}

void ToolSetHandling::updateLinearGradient(QLinearGradient linearGradient) {
    int indexLastElement = (m_linearGradientShapeQueue.size() - 1);
    LinearGradientShape linearGradientShape = m_linearGradientShapeQueue.at(indexLastElement);
    m_linearGradientShapeQueue.pop_back();
    linearGradientShape.setLinearGradient(linearGradient);
    m_linearGradientShapeQueue.append(linearGradientShape);
}


void ToolSetHandling::addConicalGradientShapeToQueue(ConicalGradientShape conicalGradientShape) {
    addLastActionToStack(CONICAL_GRADIENT_SHAPE);
    conicalGradientShape.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_conicalGradientShapeQueue.append(conicalGradientShape);
}

void ToolSetHandling::removeLastConicalGradientShape() {
    m_conicalGradientShapeQueue.pop_back();
}

QQueue<ConicalGradientShape> ToolSetHandling::getQueueOfConicalGradientShapes() {
    return m_conicalGradientShapeQueue;
}

void ToolSetHandling::updateConicalGradient(QConicalGradient conicalGradient) {
    int indexLastElement = (m_conicalGradientShapeQueue.size() - 1);
    ConicalGradientShape conicalGradientShape = m_conicalGradientShapeQueue.at(indexLastElement);
    m_conicalGradientShapeQueue.pop_back();
    conicalGradientShape.setConicalGradient(conicalGradient);
    m_conicalGradientShapeQueue.append(conicalGradientShape);
}


void ToolSetHandling::addRadialGradientShapeToQueue(RadialGradientShape radialGradientShape) {
    addLastActionToStack(RADIAL_GRADIENT_SHAPE);
    radialGradientShape.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_radialGradientShapeQueue.append(radialGradientShape);
}

void ToolSetHandling::removeLastRadialGradientShape() {
    m_radialGradientShapeQueue.pop_back();
}

QQueue<RadialGradientShape> ToolSetHandling::getQueueOfRadialGradientShapes() {
    return m_radialGradientShapeQueue;
}

void ToolSetHandling::updateRadialGradient(QRadialGradient radialGradient) {
    int indexLastElement = (m_radialGradientShapeQueue.size() - 1);
    RadialGradientShape radialGradientShape = m_radialGradientShapeQueue.at(indexLastElement);
    m_radialGradientShapeQueue.pop_back();
    radialGradientShape.setRadialGradient(radialGradient);
    m_radialGradientShapeQueue.append(radialGradientShape);
}


void ToolSetHandling::addTextBoxToQueue(TextBox textBox) {
    addLastActionToStack(TEXT_BOX);
    textBox.setPosInOrderOfActions(getPositionOfLastActionAdded());
    m_textBoxQueue.append(textBox);
}

void ToolSetHandling::removeLastTextBox() {
    m_textBoxQueue.pop_back();
}

QQueue<TextBox> ToolSetHandling::getQueueOfTextBoxes() {
    return m_textBoxQueue;
}

void ToolSetHandling::updateTextBox(QString textWritten) {
    int indexLastElement = (m_textBoxQueue.size() - 1);
    TextBox textBox = m_textBoxQueue.at(indexLastElement);
    m_textBoxQueue.pop_back();
    textBox.setWrittenText(textWritten);
    m_textBoxQueue.append(textBox);
}

void ToolSetHandling::addCoords(int x1, int x2, int y1, int y2) {
    int indexLastElement = (m_textBoxQueue.size() - 1);
    TextBox textBox = m_textBoxQueue.at(indexLastElement);
    m_textBoxQueue.pop_back();
    textBox.setCoords(x1, x2, y1, y2);
    m_textBoxQueue.append(textBox);
}

void ToolSetHandling::addFont(QFont font) {
    int indexLastElement = (m_textBoxQueue.size() - 1);
    TextBox textBox = m_textBoxQueue.at(indexLastElement);
    m_textBoxQueue.pop_back();
    textBox.setFont(font);
    m_textBoxQueue.append(textBox);
}

QFont ToolSetHandling::getFont() {
    int indexLastElement = (m_textBoxQueue.size() - 1);
    TextBox textBox = m_textBoxQueue.at(indexLastElement);
    return textBox.getFont();
}
