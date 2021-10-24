#include "scribblearea.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);

    m_modified = false;
    m_scribbling = false;
    m_myPenWidth = 1;
    m_myPenColor = Qt::black;
    m_x1 = 0;
    m_x2 = 0;
    m_y1 = 0;
    m_y2 = 0;
    m_drawLineBool = false;
    m_textSettingSet = false;
    m_fontSizeSet = false;
    m_turnBoolOn = false;
    m_setUpSquareBool = false;
    m_convexReadyToDraw = false;
    m_textBool = false;

    m_gradientColorInputDialog = new GradientColorInputDialog();
    m_setUpLinearGradientColorsBool = false;
    m_setUpConicalGradientColorsBool = false;
    m_setUpRadialGradientColorsBool = false;

    m_startDrawingRoundedSquare = false;
    m_startDrawingSquare = false;
    m_setUpSquircleBool = false;

    m_setUpEllipseBool = false;

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}


ScribbleArea::~ScribbleArea() {

}


void ScribbleArea::about() {
    qDebug() << " In about!";
}


void ScribbleArea::setDrawLineBool() {
    m_drawLineBool = true;
}


void ScribbleArea::setTextBlurb(QFont font) {
    TextBox textBox;
    textBox.setFont(font);
    m_toolSetHandling.addTextBoxToQueue(textBox);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfTextBoxQueue = m_toolSetHandling.getQueueOfTextBoxes().size();
    int posTextBoxInQueue = (sizeOfTextBoxQueue - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posTextBoxInQueue);

    m_textBool = true;
}

void ScribbleArea::setPenUp() {
    m_turnBoolOn = true;
}

void ScribbleArea::setUpSquare() {
    m_setUpSquareBool = true;
}

void ScribbleArea::setUpEllipse() {
    m_setUpEllipseBool = true;
}

void ScribbleArea::setReadyToDrawConvexPolygonBool() {
    bool okay;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("Polygon points:"), QLineEdit::Normal,
                                          "enter", &okay);
    m_convexReadyToDraw = true;
    m_numberOfPointsDrawn = 0;
    m_totalNumNeedToDraw = text.toInt();
}


void ScribbleArea::setUpLinearGradientPaints(int numColors) {
    m_userChoseThisNumColors = numColors;
    m_setUpLinearGradientColorsBool = true;
}


void ScribbleArea::setUpConicalGradientPaints(int numColors) {
    m_userChoseThisNumColors = numColors;
    m_setUpConicalGradientColorsBool = true;
}


void ScribbleArea::setUpRadialGradientPaints(int numColors) {
    m_userChoseThisNumColors = numColors;
    m_setUpRadialGradientColorsBool = true;
}


void ScribbleArea::setUpDrawSquircle() {
    m_setUpSquircleBool = true;
}


bool ScribbleArea::openImage(const QString &fileName) {
    QImage loadedImage;
    if(!loadedImage.load(fileName)) {
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    m_image = loadedImage;
    m_modified = false;
    update();
    return true;
}


bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = m_image;
    resizeImage(&visibleImage, size());

    if(visibleImage.save(fileName, fileFormat)) {
        m_modified = false;
        return true;
    } else {
        return false;
    }
}


void ScribbleArea::setPenColor(const QColor &newColor) {
    m_myPenColor = newColor;
}


void ScribbleArea::setPenWidth(int newWidth) {
    m_myPenWidth = newWidth;
}


void ScribbleArea::setEasel(const QColor &fillColor) {
    m_image.fill(fillColor);
    m_modified = true;
    update();
}


void ScribbleArea::setUpUndoFunctionality() {
    clearImage();
    QStack<QString> orderOfActions = m_toolSetHandling.getOrderOfObjectsDrawn();
    if(orderOfActions.size() >= 1) {
        for(int i = (orderOfActions.size()-1); i < orderOfActions.size(); i++) {
            QString action = orderOfActions.at(i);
            if(action == ToolSetHandling::RECTANGLE) {
                m_toolSetHandling.removeLastRectangle();
            }
            if(action == ToolSetHandling::ELLIPSE) {
                m_toolSetHandling.removeLastEllipse();
            }
            if(action == ToolSetHandling::SQUIRCLE) {
                m_toolSetHandling.removeLastSquircle();
            }
            if(action == ToolSetHandling::FREE_HAND_LINE) {
                m_toolSetHandling.removeLastFreeHandLine();
            }
            if(action == ToolSetHandling::CONVEX_POLYGON) {
                m_toolSetHandling.removeLastConvexPolygon();
            }
            if(action == ToolSetHandling::STRAIGHT_LINE) {
                m_toolSetHandling.removeLastStraightLine();
            }
            if(action == ToolSetHandling::LINEAR_GRADIENT_SHAPE) {
                m_toolSetHandling.removeLastLinearGradientShape();
            }
            if(action == ToolSetHandling::CONICAL_GRADIENT_SHAPE) {
                m_toolSetHandling.removeLastConicalGradientShape();
            }
            if(action == ToolSetHandling::RADIAL_GRADIENT_SHAPE) {
                m_toolSetHandling.removeLastRadialGradientShape();
            }
            if(action == ToolSetHandling::TEXT_BOX) {
                m_toolSetHandling.removeLastTextBox();
            }
            m_toolSetHandling.removeLastPosStored(orderOfActions.size()-1);
            m_toolSetHandling.removeLastActionFromStack();
        }
    }
    QQueue<Rectangle> rectangleQueue = m_toolSetHandling.getQueueOfRectangles();
    QQueue<Ellipse> ellipseQueue = m_toolSetHandling.getQueueOfEllipses();
    QQueue<Squircle> squircleQueue = m_toolSetHandling.getQueueOfSquircles();
    QQueue<FreeHandLine> freeHandLineQueue = m_toolSetHandling.getQueueOfFreeHandLines();
    QQueue<ConvexPolygon> convexPolygonQueue = m_toolSetHandling.getQueueOfConvexPolygons();
    QQueue<StraightLine> straightLineQueue = m_toolSetHandling.getQueueOfStraightLines();
    QQueue<LinearGradientShape> linearGradientShapeQueue = m_toolSetHandling.getQueueOfLinearGradientShapes();
    QQueue<ConicalGradientShape> conicalGradientShapeQueue = m_toolSetHandling.getQueueOfConicalGradientShapes();
    QQueue<RadialGradientShape> radialGradientShapeQueue = m_toolSetHandling.getQueueOfRadialGradientShapes();
    QQueue<TextBox> textBoxQueue = m_toolSetHandling.getQueueOfTextBoxes();

    QStack<QString> newOrderOfActions = m_toolSetHandling.getOrderOfObjectsDrawn();
    QMap<int /*posInActionStack*/,
         int /*posInShapeStack*/> posMap = m_toolSetHandling.getPosMap();

    QPainter painter(&m_image);
    if(newOrderOfActions.size() >= 1) {
        for(int i = 0; i < newOrderOfActions.size(); i++) {
            QString action = newOrderOfActions.at(i);
            int shapePos = posMap.value(i);
            if(action == ToolSetHandling::RECTANGLE) {
                Rectangle rectangle = rectangleQueue.at(shapePos);
                QPainterPath rectPath;
                rectPath.moveTo(rectangle.getX1(), rectangle.getY1());
                rectPath.lineTo(rectangle.getX2(), rectangle.getY1());
                rectPath.lineTo(rectangle.getX2(), rectangle.getY2());
                rectPath.lineTo(rectangle.getX1(), rectangle.getY2());
                rectPath.closeSubpath();
                painter.fillPath(rectPath, Qt::white);
                painter.drawPath(rectPath);
            }
            if(action == ToolSetHandling::ELLIPSE) {
                Ellipse ellipse = ellipseQueue.at(shapePos);
                int width = (ellipse.getX2() - ellipse.getX1());
                int height = (ellipse.getY2() - ellipse.getY1());
                QPainterPath path;
                path.moveTo(ellipse.getX2(), ellipse.getY1() + (height/2));
                path.arcTo(ellipse.getX1(), ellipse.getY1(), width, height, 0.0, 360.0);
                painter.fillPath(path, Qt::white);
                painter.drawPath(path);
            }
            if(action == ToolSetHandling::SQUIRCLE) {
                Squircle squircle = squircleQueue.at(shapePos);
                QPainterPath squirclePainterPath = squircle.getPainterPath();
                painter.fillPath(squirclePainterPath, Qt::white);
                painter.drawPath(squirclePainterPath);
            }
            if(action == ToolSetHandling::FREE_HAND_LINE) {
                FreeHandLine freeHandLine = freeHandLineQueue.at(shapePos);
                QQueue<QPoint> allPoints = freeHandLine.getAllSetPoints();
                m_lastPoint = allPoints.first();
                for(int i = 1; i < allPoints.size(); i++) {
                    redrawLineTo(allPoints.at(i), painter);
                }
            }
            if(action == ToolSetHandling::CONVEX_POLYGON) {
                ConvexPolygon convexPolygon = convexPolygonQueue.at(shapePos);
                QQueue<QPointF> allPoints = convexPolygon.getAllPoints();
                QPainterPath path;
                for(int i = 0; i < allPoints.size(); i++) {
                    if(i == 0) {
                        path.moveTo(allPoints.at(i));
                    } else {
                        path.lineTo(allPoints.at(i));
                    }
                }
                path.closeSubpath();
                painter.fillPath(path, Qt::white);
                painter.drawPath(path);
            }
            if(action == ToolSetHandling::STRAIGHT_LINE) {
                StraightLine straightLine = straightLineQueue.at(shapePos);
                QPoint pointOne = straightLine.getPointOne();
                QPoint pointTwo = straightLine.getPointTwo();
                painter.drawEllipse(pointOne, 1, 1);
                painter.drawEllipse(pointTwo, 1, 1);
                painter.drawLine(pointOne, pointTwo);
            }
            if(action == ToolSetHandling::LINEAR_GRADIENT_SHAPE) {
                LinearGradientShape linearGradientShape = linearGradientShapeQueue.at(shapePos);
                int width = (linearGradientShape.getX2() - linearGradientShape.getX1());
                int height = (linearGradientShape.getY2() - linearGradientShape.getY1());
                QRect rectLinear(linearGradientShape.getX1(), linearGradientShape.getY1(), width, height);
                painter.fillRect(rectLinear, linearGradientShape.getLinearGradient());
            }
            if(action == ToolSetHandling::CONICAL_GRADIENT_SHAPE) {
                ConicalGradientShape conicalGradientShape = conicalGradientShapeQueue.at(shapePos);
                int width = (conicalGradientShape.getX2() - conicalGradientShape.getX1());
                int height = (conicalGradientShape.getY2() - conicalGradientShape.getY1());
                QRect rectLinear(conicalGradientShape.getX1(), conicalGradientShape.getY1(), width, height);
                painter.fillRect(rectLinear, conicalGradientShape.getConicalGradient());
            }
            if(action == ToolSetHandling::RADIAL_GRADIENT_SHAPE) {
                RadialGradientShape radialGradientShape = radialGradientShapeQueue.at(shapePos);
                int width = (radialGradientShape.getX2() - radialGradientShape.getX1());
                int height = (radialGradientShape.getY2() - radialGradientShape.getY1());
                QRect rectLinear(radialGradientShape.getX1(), radialGradientShape.getY1(), width, height);
                painter.fillRect(rectLinear, radialGradientShape.getRadialGradient());
            }
            if(action == ToolSetHandling::TEXT_BOX) {
                TextBox textBox = textBoxQueue.at(shapePos);
                int width = (textBox.getX2() - textBox.getX1());
                int height = (textBox.getY2() - textBox.getY1());
                QTextEdit *textEdit = new QTextEdit(this);
                textEdit->setFrameStyle(QFrame::NoFrame);
                textEdit->viewport()->setAutoFillBackground(false);
                textEdit->setFont(textBox.getFont());

                textEdit->setGeometry(textBox.getX1(), textBox.getY1(), width, height);
                textEdit->setText(textBox.getTextWritten());
                textEdit->show();
                m_textEditList.append(textEdit);
            }
        }
    }
    update();
}


void ScribbleArea::restoreImage() {
    clearImage();
    QQueue<Rectangle> rectangleQueue = m_toolSetHandling.getQueueOfRectangles();
    QQueue<Ellipse> ellipseQueue = m_toolSetHandling.getQueueOfEllipses();
    QQueue<Squircle> squircleQueue = m_toolSetHandling.getQueueOfSquircles();
    QQueue<FreeHandLine> freeHandLineQueue = m_toolSetHandling.getQueueOfFreeHandLines();
    QQueue<ConvexPolygon> convexPolygonQueue = m_toolSetHandling.getQueueOfConvexPolygons();
    QQueue<StraightLine> straightLineQueue = m_toolSetHandling.getQueueOfStraightLines();
    QQueue<LinearGradientShape> linearGradientShapeQueue = m_toolSetHandling.getQueueOfLinearGradientShapes();
    QQueue<ConicalGradientShape> conicalGradientShapeQueue = m_toolSetHandling.getQueueOfConicalGradientShapes();
    QQueue<RadialGradientShape> radialGradientShapeQueue = m_toolSetHandling.getQueueOfRadialGradientShapes();
    QQueue<TextBox> textBoxQueue = m_toolSetHandling.getQueueOfTextBoxes();

    QStack<QString> newOrderOfActions = m_toolSetHandling.getOrderOfObjectsDrawn();
    QMap<int /*posInActionStack*/,
         int /*posInShapeStack*/> posMap = m_toolSetHandling.getPosMap();

    QPainter painter(&m_image);
    if(newOrderOfActions.size() >= 1) {
        for(int i = 0; i < newOrderOfActions.size(); i++) {
            QString action = newOrderOfActions.at(i);
            int shapePos = posMap.value(i);
            if(action == ToolSetHandling::RECTANGLE) {
                Rectangle rectangle = rectangleQueue.at(shapePos);
                QPainterPath rectPath;
                rectPath.moveTo(rectangle.getX1(), rectangle.getY1());
                rectPath.lineTo(rectangle.getX2(), rectangle.getY1());
                rectPath.lineTo(rectangle.getX2(), rectangle.getY2());
                rectPath.lineTo(rectangle.getX1(), rectangle.getY2());
                rectPath.closeSubpath();
                painter.fillPath(rectPath, Qt::white);
                painter.drawPath(rectPath);
            }
            if(action == ToolSetHandling::ELLIPSE) {
                Ellipse ellipse = ellipseQueue.at(shapePos);
                int width = (ellipse.getX2() - ellipse.getX1());
                int height = (ellipse.getY2() - ellipse.getY1());
                QPainterPath path;
                path.moveTo(ellipse.getX2(), ellipse.getY1() + (height/2));
                path.arcTo(ellipse.getX1(), ellipse.getY1(), width, height, 0.0, 360.0);
                painter.fillPath(path, Qt::white);
                painter.drawPath(path);
            }
            if(action == ToolSetHandling::SQUIRCLE) {
                Squircle squircle = squircleQueue.at(shapePos);
                QPainterPath squirclePainterPath = squircle.getPainterPath();
                painter.fillPath(squirclePainterPath, Qt::white);
                painter.drawPath(squirclePainterPath);
            }
            if(action == ToolSetHandling::FREE_HAND_LINE) {
                FreeHandLine freeHandLine = freeHandLineQueue.at(shapePos);
                QQueue<QPoint> allPoints = freeHandLine.getAllSetPoints();
                m_lastPoint = allPoints.first();
                for(int i = 1; i < allPoints.size(); i++) {
                    redrawLineTo(allPoints.at(i), painter);
                }
            }
            if(action == ToolSetHandling::CONVEX_POLYGON) {
                ConvexPolygon convexPolygon = convexPolygonQueue.at(shapePos);
                QQueue<QPointF> allPoints = convexPolygon.getAllPoints();
                QPainterPath path;
                for(int i = 0; i < allPoints.size(); i++) {
                    if(i == 0) {
                        path.moveTo(allPoints.at(i));
                    } else {
                        path.lineTo(allPoints.at(i));
                    }
                }
                path.closeSubpath();
                painter.fillPath(path, Qt::white);
                painter.drawPath(path);
            }
            if(action == ToolSetHandling::STRAIGHT_LINE) {
                StraightLine straightLine = straightLineQueue.at(shapePos);
                QPoint pointOne = straightLine.getPointOne();
                QPoint pointTwo = straightLine.getPointTwo();
                painter.drawEllipse(pointOne, 1, 1);
                painter.drawEllipse(pointTwo, 1, 1);
                painter.drawLine(pointOne, pointTwo);
            }
            if(action == ToolSetHandling::LINEAR_GRADIENT_SHAPE) {
                LinearGradientShape linearGradientShape = linearGradientShapeQueue.at(shapePos);
                int width = (linearGradientShape.getX2() - linearGradientShape.getX1());
                int height = (linearGradientShape.getY2() - linearGradientShape.getY1());
                QRect rectLinear(linearGradientShape.getX1(), linearGradientShape.getY1(), width, height);
                painter.fillRect(rectLinear, linearGradientShape.getLinearGradient());
            }
            if(action == ToolSetHandling::CONICAL_GRADIENT_SHAPE) {
                ConicalGradientShape conicalGradientShape = conicalGradientShapeQueue.at(shapePos);
                int width = (conicalGradientShape.getX2() - conicalGradientShape.getX1());
                int height = (conicalGradientShape.getY2() - conicalGradientShape.getY1());
                QRect rectLinear(conicalGradientShape.getX1(), conicalGradientShape.getY1(), width, height);
                painter.fillRect(rectLinear, conicalGradientShape.getConicalGradient());
            }
            if(action == ToolSetHandling::RADIAL_GRADIENT_SHAPE) {
                RadialGradientShape radialGradientShape = radialGradientShapeQueue.at(shapePos);
                int width = (radialGradientShape.getX2() - radialGradientShape.getX1());
                int height = (radialGradientShape.getY2() - radialGradientShape.getY1());
                QRect rectLinear(radialGradientShape.getX1(), radialGradientShape.getY1(), width, height);
                painter.fillRect(rectLinear, radialGradientShape.getRadialGradient());
            }
            if(action == ToolSetHandling::TEXT_BOX) {
                TextBox textBox = textBoxQueue.at(shapePos);
                int width = (textBox.getX2() - textBox.getX1());
                int height = (textBox.getY2() - textBox.getY1());
                QTextEdit *textEdit = new QTextEdit(this);
                textEdit->setFrameStyle(QFrame::NoFrame);
                textEdit->viewport()->setAutoFillBackground(false);
                textEdit->setFont(textBox.getFont());

                textEdit->setGeometry(textBox.getX1(), textBox.getY1(), width, height);
                textEdit->setText(textBox.getTextWritten());
                textEdit->show();
                m_textEditList.append(textEdit);
            }
        }
    }
    update();
}


void ScribbleArea::clearImage() {
    // image.fill(qRgb(255,255,255));
    /* QImage newImage;
    image = newImage;
    image.fill(qRgb(255,255,255)); */
    QPainter painter(&m_image);
    for(int i = 0; i < m_drawnRectList.size(); i++) {
        painter.eraseRect(m_drawnRectList.at(i));
        m_drawnRectList.takeAt(i);
    }
    for(int i = 0; i < m_drawnRectPointerList.size(); i++) {
        painter.eraseRect(*m_drawnRectPointerList.at(i));
        delete m_drawnRectPointerList.at(i);
        m_drawnRectPointerList.removeAt(i);
    }
    m_image.fill(qRgb(255,255,255));
    m_modified = true;
    while(!m_textEditList.isEmpty()) {
        QTextEdit *textEdit = m_textEditList.dequeue();
        delete textEdit;
    }
    update();
}


void ScribbleArea::keyPressEvent(QKeyEvent *event) {
    if(m_currentlyTyping && (event->key() != Qt::Key_Escape)) {
        QTextEdit *textEdit = m_textEditList.last();
        QString tmpCurText = textEdit->toPlainText();
        tmpCurText += event->text();
        textEdit->setText(tmpCurText);
        textEdit->show();
        update();
    }
    if(m_currentlyTyping && (event->key() == Qt::Key_Backspace)) {
        QTextEdit *textEdit = m_textEditList.last();
        QString tmpCurText = textEdit->toPlainText();
        tmpCurText.remove(tmpCurText.length()-2, 2);
        textEdit->setText(tmpCurText);
        textEdit->show();
        update();
    }
    if(m_currentlyTyping && (event->key() == Qt::Key_Escape)) {
        QTextEdit *textEdit = m_textEditList.last();
        m_currentlyTyping = false;
        textEdit->setReadOnly(true);
        m_toolSetHandling.updateTextBox(textEdit->toPlainText());
    }
}


void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(m_turnBoolOn) {
            m_lastPoint = event->pos();
            FreeHandLine freeHandLine;
            freeHandLine.setNewPoint(m_lastPoint);
            freeHandLine.setPenColor(m_myPenColor);
            freeHandLine.setPenWidth(m_myPenWidth);
            m_toolSetHandling.addFreeHandLineToQueue(freeHandLine);
            int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
            int sizeOfFreeHandLineQueue = m_toolSetHandling.getQueueOfFreeHandLines().size();
            int posFreeHandLineInQueue = (sizeOfFreeHandLineQueue -1);
            m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posFreeHandLineInQueue);
        }
        if(m_drawLineBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(m_textBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(m_setUpSquareBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
            m_drawingSquare.setX(m_x1);
            m_drawingSquare.setY(m_y1);
            m_startDrawingSquare = true;
        }
        if(m_setUpEllipseBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(m_convexReadyToDraw && this->underMouse()) {
            if(m_numberOfPointsDrawn < m_totalNumNeedToDraw) {
                m_x1 = event->x();
                m_y1 = event->y();
                QPointF tmpPoint(m_x1, m_y1);
                m_coordSet.append(tmpPoint);
                m_numberOfPointsDrawn++;
                if(m_numberOfPointsDrawn < m_totalNumNeedToDraw) {
                    return;
                }
            }
            if(m_numberOfPointsDrawn >= m_totalNumNeedToDraw) {
                drawConvexPolygon();
            }
        }
        if(m_setUpLinearGradientColorsBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(m_setUpConicalGradientColorsBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(m_setUpRadialGradientColorsBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(m_setUpSquircleBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
    }
}


void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
    if(event->button() == Qt::NoButton) {
        if(m_convexReadyToDraw && this->underMouse()) {
            if(m_coordSet.size() > 0) {
                restoreImage();
                QPainter painter(&m_image);
                QPainterPath path;
                if(m_coordSet.size() > 1) {
                    path.moveTo(m_coordSet.at(0));
                    for(int i = 0; i <= (m_coordSet.size()-1); i++) {
                        if(i == 0) {
                            painter.drawLine(m_coordSet.at(i), m_coordSet.at(i+1));
                        } else {
                            painter.drawLine(m_coordSet.at(i-1), m_coordSet.at(i));
                            path.lineTo(m_coordSet.at(i));
                        }
                    }
                }
                QPointF pointOne = m_coordSet.last();
                painter.drawLine(pointOne, event->pos());

                path.lineTo(pointOne);
                painter.fillPath(path, Qt::white);
                if(m_coordSet.size() > 1) {
                    painter.drawPath(path);

                    QPen pen;
                    pen.setColor(Qt::white);
                    painter.setPen(pen);
                    QPainterPath pathTwo;
                    pathTwo.moveTo(pointOne);
                    pathTwo.lineTo(m_coordSet.at(0));
                }

                update();
            }
        }
    }
    if(event->buttons() & Qt::LeftButton) {
        if(m_turnBoolOn) {
            drawLineTo(event->pos());
            FreeHandLine &curFreeHandLine = m_toolSetHandling.obtainCurFreeHandLineInstance();
            curFreeHandLine.setNewPoint(event->pos());
        }
        if(m_setUpSquareBool && this->underMouse()) {
            QPainter painter(&m_image);
            if(m_startDrawingSquare == false) {
                painter.eraseRect(m_drawingSquare);
            }
            m_startDrawingSquare = false;
            int dx = event->x();
            int dy = event->y();
            int width = (dx - m_x1);
            int height = (dy - m_y1);

            m_drawingSquare.setWidth(width);
            m_drawingSquare.setHeight(height);
            painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
            // painter.setBrush(QColor(0, 0, 255, 127));
            painter.drawRect(m_drawingSquare);
            update();
            /* QPainter painter(&m_image);
            int dx = event->x();
            int dy = event->y();

            QPainterPath rectPath;
            rectPath.moveTo(m_x1, m_y1);
            rectPath.moveTo(dy, m_y1);
            rectPath.moveTo(dx, dy);
            rectPath.moveTo(m_x1, m_y1);
            rectPath.closeSubpath();

            painter.drawPath(rectPath);
            painter.end();
            update(); */
        }
        if(m_setUpSquircleBool && this->underMouse()) {
            int dx = event->x();
            int dy = event->y();
            int width = (dx - m_x1);
            int squareWidth;
            if(width > 25) {
                dx = event->x();
                dy = event->y();
                squareWidth = 20;
            } else {
                dx = (m_x1 + 25);
                dy = (m_y1 + 25);
                squareWidth = 20;
            }

            QPainter painter(&m_image);
            QPainterPath path;

            path.moveTo(m_x1 + squareWidth, m_y1);
            path.lineTo(dx - squareWidth, m_y1);
            path.arcTo(dx - squareWidth, m_y1, squareWidth, squareWidth, 90.0, -90.0);
            path.lineTo(dx, dy - squareWidth);
            path.arcTo(dx - squareWidth, dy - squareWidth, squareWidth, squareWidth, 0.0, -90.0);
            path.lineTo(m_x1 + squareWidth, dy);
            path.arcTo(m_x1, dy - squareWidth, squareWidth, squareWidth, 270.0, -90.0);
            path.lineTo(m_x1, m_y1 + squareWidth);
            path.arcTo(m_x1, m_y1, squareWidth, squareWidth, 180.0, -90.0);
            path.closeSubpath();

            painter.setBrush(Qt::white);
            painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
            Qt::RoundCap, Qt::RoundJoin));
            painter.drawPath(path);
            update();
        }
        if(m_setUpEllipseBool && this->underMouse()) {
            restoreImage();
            QPainter painter(&m_image);

            QPainterPath curEllipse;
            int dx = event->x();
            int dy = event->y();
            int width = (dx - m_x1);
            int height = (dy - m_y1);

            curEllipse.moveTo(dx, m_y1 + (height/2));
            curEllipse.arcTo(m_x1, m_y1, width, height, 0.0, 360.0);

            painter.fillPath(curEllipse, Qt::white);
            painter.drawPath(curEllipse);

            update();
        }
    }
}


void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(m_turnBoolOn) {
            drawLineTo(event->pos());
            FreeHandLine curFreeHandLine = m_toolSetHandling.obtainCurFreeHandLineInstance();
            curFreeHandLine.setNewPoint(event->pos());
            m_turnBoolOn = false;
        }
        if(m_convexReadyToDraw && this->underMouse()) {
            if(m_numberOfPointsDrawn < m_totalNumNeedToDraw) {
                // just draw straight line from last point to current mouse position
                QPainter painter(&m_image);
                QPointF pointOne = m_coordSet.last();
                QPointF pointTwo(event->x(), event->y());
                painter.drawLine(pointOne, pointTwo);
                update();
            }
        }
        if(m_drawLineBool) {
            m_drawLineBool = false;
            m_x2 = event->x();
            m_y2 = event->y();
            drawLine();
        }
        if(m_textBool) {
            m_x2 = event->x();
            m_y2 = event->y();
            createTextBlurb();
        }
        if(m_setUpSquareBool && this->underMouse()) {
            QPainter painter(&m_image);
            painter.eraseRect(m_drawingSquare);
            painter.end();
            m_x2 = event->x();
            m_y2 = event->y();
            createSquare();
        }
        if(m_setUpEllipseBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            createEllipse();
        }
        if(m_setUpLinearGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            linearGradientColorSelection(m_userChoseThisNumColors);
        }
        if(m_setUpConicalGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            conicalGradientColorSelection(m_userChoseThisNumColors);
        }
        if(m_setUpRadialGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            radialGradientColorSelection(m_userChoseThisNumColors);
        }
        if(m_setUpSquircleBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            drawSquircle();
        }
    }
}


void ScribbleArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
}


void ScribbleArea::resizeEvent(QResizeEvent *event) {
    if(width() > m_image.width() || height() > m_image.height()) {
        int newWidth = qMax(width() + 128, m_image.width());
        int newHeight = qMax(height() + 128, m_image.height());
        resizeImage(&m_image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void ScribbleArea::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&m_image);
    painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_lastPoint, endPoint);
    m_modified = true;
    int rad = (m_myPenWidth / 2) + 2;
    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    m_lastPoint = endPoint;
}


void ScribbleArea::redrawLineTo(const QPoint &endPoint, QPainter &curPainter) {
    curPainter.drawLine(m_lastPoint, endPoint);
    m_modified = true;
    int rad = (m_myPenWidth / 2) + 2;
    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    m_lastPoint = endPoint;
}


void ScribbleArea::resizeImage(QImage *image, const QSize &newSize) {
    if(image->size() == newSize) { return; }
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}


void ScribbleArea::drawLine() {
    if((m_x1 == 0 ) && (m_x2 == 0) && (m_y1 == 0) && (m_y2 == 0)) {
        qDebug() << "member x,y values not set for drawing a line.";
        return;
    }

    QPainter painter(&m_image);
    QPoint pointOne(m_x1, m_y1);
    QPoint pointTwo(m_x2, m_y2);

    StraightLine straightLine;
    straightLine.setPoints(pointOne, pointTwo);

    m_toolSetHandling.addStraightLineToQueue(straightLine);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfStraightLineQueue = m_toolSetHandling.getQueueOfStraightLines().size();
    int posStraightLineInQueue = (sizeOfStraightLineQueue - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posStraightLineInQueue);

    painter.drawEllipse(pointOne, 1, 1);
    painter.drawEllipse(pointTwo, 1, 1);

    painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));

    painter.drawLine(pointOne, pointTwo);

    update();
}


void ScribbleArea::createTextBlurb() {
    m_currentlyTyping = true;

    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setFrameStyle(QFrame::NoFrame);
    textEdit->viewport()->setAutoFillBackground(false);
    int diff_x = (m_x2 - m_x1);
    int diff_y = (m_y2 - m_y1);

    m_toolSetHandling.updateTextBox(textEdit->toPlainText());
    m_toolSetHandling.addCoords(m_x1, m_x2, m_y1, m_y2);

    textEdit->setFont(m_toolSetHandling.getFont());
    textEdit->setGeometry(m_x1, m_y1, diff_x, diff_y);
    textEdit->show();
    m_textEditList.append(textEdit);
}


void ScribbleArea::createSquare() {
    QPainter painter(&m_image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    // QRect rect(m_x1, m_y1, width, height);
    QPainterPath rectPath;
    rectPath.moveTo(m_x1, m_y1);
    rectPath.lineTo(m_x2, m_y1);
    rectPath.lineTo(m_x2, m_y2);
    rectPath.lineTo(m_x1, m_y2);
    rectPath.closeSubpath();
    Rectangle rectangle;
    rectangle.setCoords(m_x1, m_x2, m_y1, m_y2);
    rectangle.setPenColor(m_myPenColor);
    rectangle.setPenWidth(m_myPenWidth);
    painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    // painter.fillRect(rect, QBrush(QColor(0, 0, 255, 127)));
    // painter.setBrush(QColor(0, 0, 255, 127));
    // painter.drawRect(rect);
    painter.drawPath(rectPath);
    painter.end();

    m_toolSetHandling.addRectangleToQueue(rectangle);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfRectangleQueue = m_toolSetHandling.getQueueOfRectangles().size();
    int posSquareInQueue = (sizeOfRectangleQueue - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquareInQueue);

    // m_drawnRectList << rect;
    m_setUpSquareBool = false;
    update();
}


void ScribbleArea::createEllipse() {
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rect(m_x1, m_y1, width, height);
    QPainter painter(&m_image);
    Ellipse ellipse;
    ellipse.setCoords(m_x1, m_x2, m_y1, m_y2);
    ellipse.setPenColor(m_myPenColor);
    ellipse.setPenWidth(m_myPenWidth);
    painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    // painter.fillRect(rect, QBrush(Qt::white, Qt::SolidPattern));
    // painter.drawEllipse(rect);

    QPainterPath path;
    path.moveTo(m_x2, m_y1 + (height/2));
    path.arcTo(m_x1, m_y1, width, height, 0.0, 360.0);
    painter.fillPath(path, Qt::white);
    painter.drawPath(path);

    m_toolSetHandling.addEllipseToQueue(ellipse);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfEllipsesQueue = m_toolSetHandling.getQueueOfEllipses().size();
    int posSquareInQueue = (sizeOfEllipsesQueue - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquareInQueue);

    m_drawnRectList << rect;
    m_setUpEllipseBool = false;
    update();
}


void ScribbleArea::drawConvexPolygon() {
    QPainter painter(&m_image);
    painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    int arrSize = m_coordSet.size();
    QPointF points[arrSize];
    ConvexPolygon convexPolygon;
    for(int i = 0; i < m_coordSet.size(); i++) {
        points[i] = m_coordSet.at(i);
        convexPolygon.addPointToQueue(m_coordSet.at(i));
    }
    // painter.drawConvexPolygon(points, arrSize);
    QPainterPath path;
    for(int i = 0; i < m_coordSet.size(); i++) {
        if(i == 0) {
            path.moveTo(m_coordSet.at(i));
        } else {
            path.lineTo(m_coordSet.at(i));
        }
    }
    path.closeSubpath();
    painter.fillPath(path, Qt::white);
    painter.drawPath(path);

    m_toolSetHandling.addConvexPolygonToQueue(convexPolygon);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfConvexPolygonQueue = m_toolSetHandling.getQueueOfConvexPolygons().size();
    int posConvexPolygonInQueue = (sizeOfConvexPolygonQueue - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posConvexPolygonInQueue);

    m_convexReadyToDraw = false;
    m_coordSet.clear();
    update();
}


void ScribbleArea::linearGradientColorSelection(int numColors) {
    m_gradientColorInputDialog->showLinearGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors);
    connect(m_gradientColorInputDialog, SIGNAL(linearGradientToolsSet()), this, SLOT(readyToDrawLinearGradient()));

    LinearGradientShape linearGradientShape;
    linearGradientShape.setCoords(m_x1, m_x2, m_y1, m_y2);

    m_toolSetHandling.addLinearGradientShapeToQueue(linearGradientShape);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfLinearGradientShapes = m_toolSetHandling.getQueueOfLinearGradientShapes().size();
    int posLinearGradientShapeInQueue = (sizeOfLinearGradientShapes - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posLinearGradientShapeInQueue);
}


void ScribbleArea::readyToDrawLinearGradient() {
    QLinearGradient curLinearGradient = m_gradientColorInputDialog->getLinearGradientTools();
    QPainter painter(&m_image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectLinear(m_x1, m_y1, width, height);

    m_toolSetHandling.updateLinearGradient(curLinearGradient);

    painter.fillRect(rectLinear, curLinearGradient);
    m_setUpLinearGradientColorsBool = false;
    m_drawnRectList << rectLinear;

    update();
}



void ScribbleArea::drawSquircle() {
    QPainter painter(&m_image);
    QPainterPath path;

    path.moveTo(m_x1 + 20, m_y1);
    path.lineTo(m_x2 - 20, m_y1);
    path.arcTo(m_x2 - 20, m_y1, 20, 20, 90.0, -90.0);
    path.lineTo(m_x2, m_y2 - 20);
    path.arcTo(m_x2 - 20, m_y2 - 20, 20, 20, 0.0, -90.0);
    path.lineTo(m_x1 + 20, m_y2);
    path.arcTo(m_x1, m_y2 - 20, 20, 20, 270.0, -90.0);
    path.lineTo(m_x1, m_y1 + 20);
    path.arcTo(m_x1, m_y1, 20, 20, 180.0, -90.0);
    path.closeSubpath();

    painter.setBrush(Qt::white);
    painter.setPen(QPen(m_myPenColor, m_myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);

    Squircle squircle;
    squircle.setPainterPath(path);
    squircle.setPenWidth(m_myPenWidth);
    squircle.setPenColor(m_myPenColor);

    m_toolSetHandling.addSquircleToQueue(squircle);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfSquircleQueue = m_toolSetHandling.getQueueOfSquircles().size();
    int posSquircleInQueue = (sizeOfSquircleQueue - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquircleInQueue);

    m_setUpSquircleBool = false;
    update();
}


void ScribbleArea::conicalGradientColorSelection(int numColors) {
    m_gradientColorInputDialog->showConicalGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors);
    connect(m_gradientColorInputDialog, SIGNAL(conicalGradientToolsSet()), this, SLOT(readyToDrawConicalGradient()));

    ConicalGradientShape conicalGradientShape;
    conicalGradientShape.setCoords(m_x1, m_x2, m_y1, m_y2);

    m_toolSetHandling.addConicalGradientShapeToQueue(conicalGradientShape);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfConicalGradientShapes = m_toolSetHandling.getQueueOfConicalGradientShapes().size();
    int posConicalGradientShapeInQueue = (sizeOfConicalGradientShapes - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posConicalGradientShapeInQueue);
}


void ScribbleArea::readyToDrawConicalGradient() {
    QConicalGradient* curConicalGradient = m_gradientColorInputDialog->getConicalGradientTools();
    QPainter painter(&m_image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectConical(m_x1, m_y1, width, height);

    m_toolSetHandling.updateConicalGradient(*curConicalGradient);

    painter.fillRect(rectConical, *curConicalGradient);
    m_setUpConicalGradientColorsBool = false;
    m_drawnRectList << rectConical;

    update();
}


void ScribbleArea::radialGradientColorSelection(int numColors) {
    m_gradientColorInputDialog->showRadialGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors,
                                                        m_image.width(), m_image.height());
    connect(m_gradientColorInputDialog, SIGNAL(radialGradientToolsSet()), this, SLOT(readyToDrawRadialGradient()));

    RadialGradientShape radialGradientShape;
    radialGradientShape.setCoords(m_x1, m_x2, m_y1, m_y2);

    m_toolSetHandling.addRadialGradientShapeToQueue(radialGradientShape);
    int posLastActionAdded = m_toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfRadialGradientShapes = m_toolSetHandling.getQueueOfRadialGradientShapes().size();
    int posRadialGradientShapeInQueue = (sizeOfRadialGradientShapes - 1);
    m_toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posRadialGradientShapeInQueue);
}


void ScribbleArea::readyToDrawRadialGradient() {
    QRadialGradient* curRadialGradient = m_gradientColorInputDialog->getRadialGradientTools();
    QPainter painter(&m_image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectRadial(m_x1, m_y1, width, height);

    m_toolSetHandling.updateRadialGradient(*curRadialGradient);

    painter.fillRect(rectRadial, *curRadialGradient);
    m_setUpRadialGradientColorsBool = false;
    m_drawnRectList << rectRadial;

    update();
}


void ScribbleArea::print() {
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = m_image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_image.rect());
        painter.drawImage(0,0,m_image);
    }
#endif // QT_CONFIG(printdialog)
}

