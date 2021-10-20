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

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    modified = false;
    scribbling = false;
    myPenWidth = 1;
    myPenColor = Qt::black;
    m_x1 = 0;
    m_x2 = 0;
    m_y1 = 0;
    m_y2 = 0;
    drawLineBool = false;
    textSettingSet = false;
    fontSizeSet = false;
    turnBoolOn = false;
    setUpSquareBool = false;
    setUpEllipseBool = false;
    secondConvexReadyToDraw = false;

    _gradientColorInputDialog = new GradientColorInputDialog();
    setUpLinearGradientColorsBool = false;
    setUpConicalGradientColorsBool = false;
    setUpRadialGradientColorsBool = false;

    startDrawingRoundedSquare = false;
    startDrawingSquare = false;
    setUpSquircleBool = false;

    setFocusPolicy(Qt::StrongFocus);
}


ScribbleArea::~ScribbleArea() {

}


void ScribbleArea::setDrawLineBool() {
    drawLineBool = true;
}


void ScribbleArea::setTextBlurb(QFont font) {
    TextBox textBox;
    textBox.setFont(font);
    _toolSetHandling.addTextBoxToQueue(textBox);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfTextBoxQueue = _toolSetHandling.getQueueOfTextBoxes().size();
    int posTextBoxInQueue = (sizeOfTextBoxQueue - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posTextBoxInQueue);

    textBool = true;
}

void ScribbleArea::setPenUp() {
    turnBoolOn = true;
}

void ScribbleArea::setUpSquare() {
    setUpSquareBool = true;
}

void ScribbleArea::setUpEllipse() {
    setUpEllipseBool = true;
}

void ScribbleArea::setReadyToDrawConvexPolygonBool() {
    bool okay;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("Polygon points:"), QLineEdit::Normal,
                                          "enter", &okay);
    secondConvexReadyToDraw = true;
    secondNumberOfPointsDrawn = 0;
    secondTotalNumNeedToDraw = text.toInt();
}


void ScribbleArea::setUpLinearGradientPaints(int numColors) {
    userChoseThisNumColors = numColors;
    setUpLinearGradientColorsBool = true;
}


void ScribbleArea::setUpConicalGradientPaints(int numColors) {
    userChoseThisNumColors = numColors;
    setUpConicalGradientColorsBool = true;
}


void ScribbleArea::setUpRadialGradientPaints(int numColors) {
    userChoseThisNumColors = numColors;
    setUpRadialGradientColorsBool = true;
}


void ScribbleArea::setUpDrawSquircle() {
    setUpSquircleBool = true;
}


bool ScribbleArea::openImage(const QString &fileName) {
    QImage loadedImage;
    if(!loadedImage.load(fileName)) {
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}


bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if(visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}


void ScribbleArea::setPenColor(const QColor &newColor) {
    myPenColor = newColor;
}


void ScribbleArea::setPenWidth(int newWidth) {
    myPenWidth = newWidth;
}


void ScribbleArea::setEasel(const QColor &fillColor) {
    image.fill(fillColor);
    modified = true;
    update();
}


void ScribbleArea::setUpUndoFunctionality() {
    clearImage();
    QStack<QString> orderOfActions = _toolSetHandling.getOrderOfObjectsDrawn();
    if(orderOfActions.size() >= 1) {
        for(int i = (orderOfActions.size()-1); i < orderOfActions.size(); i++) {
            QString action = orderOfActions.at(i);
            if(action == ToolSetHandling::RECTANGLE) {
                _toolSetHandling.removeLastRectangle();
            }
            if(action == ToolSetHandling::ELLIPSE) {
                _toolSetHandling.removeLastEllipse();
            }
            if(action == ToolSetHandling::SQUIRCLE) {
                _toolSetHandling.removeLastSquircle();
            }
            if(action == ToolSetHandling::FREE_HAND_LINE) {
                _toolSetHandling.removeLastFreeHandLine();
            }
            if(action == ToolSetHandling::CONVEX_POLYGON) {
                _toolSetHandling.removeLastConvexPolygon();
            }
            if(action == ToolSetHandling::STRAIGHT_LINE) {
                _toolSetHandling.removeLastStraightLine();
            }
            if(action == ToolSetHandling::LINEAR_GRADIENT_SHAPE) {
                _toolSetHandling.removeLastLinearGradientShape();
            }
            if(action == ToolSetHandling::CONICAL_GRADIENT_SHAPE) {
                _toolSetHandling.removeLastConicalGradientShape();
            }
            if(action == ToolSetHandling::RADIAL_GRADIENT_SHAPE) {
                _toolSetHandling.removeLastRadialGradientShape();
            }
            if(action == ToolSetHandling::TEXT_BOX) {
                _toolSetHandling.removeLastTextBox();
            }
            _toolSetHandling.removeLastPosStored(orderOfActions.size()-1);
            _toolSetHandling.removeLastActionFromStack();
        }
    }
    QQueue<Rectangle> rectangleQueue = _toolSetHandling.getQueueOfRectangles();
    QQueue<Ellipse> ellipseQueue = _toolSetHandling.getQueueOfEllipses();
    QQueue<Squircle> squircleQueue = _toolSetHandling.getQueueOfSquircles();
    QQueue<FreeHandLine> freeHandLineQueue = _toolSetHandling.getQueueOfFreeHandLines();
    QQueue<ConvexPolygon> convexPolygonQueue = _toolSetHandling.getQueueOfConvexPolygons();
    QQueue<StraightLine> straightLineQueue = _toolSetHandling.getQueueOfStraightLines();
    QQueue<LinearGradientShape> linearGradientShapeQueue = _toolSetHandling.getQueueOfLinearGradientShapes();
    QQueue<ConicalGradientShape> conicalGradientShapeQueue = _toolSetHandling.getQueueOfConicalGradientShapes();
    QQueue<RadialGradientShape> radialGradientShapeQueue = _toolSetHandling.getQueueOfRadialGradientShapes();
    QQueue<TextBox> textBoxQueue = _toolSetHandling.getQueueOfTextBoxes();

    QStack<QString> newOrderOfActions = _toolSetHandling.getOrderOfObjectsDrawn();
    QMap<int /*posInActionStack*/,
         int /*posInShapeStack*/> posMap = _toolSetHandling.getPosMap();

    QPainter painter(&image);
    if(newOrderOfActions.size() >= 1) {
        for(int i = 0; i < newOrderOfActions.size(); i++) {
            QString action = newOrderOfActions.at(i);
            int shapePos = posMap.value(i);
            if(action == ToolSetHandling::RECTANGLE) {
                Rectangle rectangle = rectangleQueue.at(shapePos);
                int width = (rectangle.getX2() - rectangle.getX1());
                int height = (rectangle.getY2() - rectangle.getY1());
                QRect rectToDraw(rectangle.getX1(), rectangle.getY1(), width, height);
                painter.drawRect(rectToDraw);
            }
            if(action == ToolSetHandling::ELLIPSE) {
                Ellipse ellipse = ellipseQueue.at(shapePos);
                int width = (ellipse.getX2() - ellipse.getX1());
                int height = (ellipse.getY2() - ellipse.getY1());
                QRect ellipseToDraw(ellipse.getX1(), ellipse.getY1(), width, height);
                painter.drawEllipse(ellipseToDraw);
            }
            if(action == ToolSetHandling::SQUIRCLE) {
                Squircle squircle = squircleQueue.at(shapePos);
                painter.drawPath(squircle.getPainterPath());
            }
            if(action == ToolSetHandling::FREE_HAND_LINE) {
                FreeHandLine freeHandLine = freeHandLineQueue.at(shapePos);
                QQueue<QPoint> allPoints = freeHandLine.getAllSetPoints();
                lastPoint = allPoints.first();
                for(int i = 1; i < allPoints.size(); i++) {
                    redrawLineTo(allPoints.at(i), painter);
                }
            }
            if(action == ToolSetHandling::CONVEX_POLYGON) {
                ConvexPolygon convexPolygon = convexPolygonQueue.at(shapePos);
                QQueue<QPointF> allPoints = convexPolygon.getAllPoints();
                QPointF points[allPoints.size()];
                for(int i = 0; i < allPoints.size(); i++) {
                    points[i] = allPoints.at(i);
                }
                painter.drawConvexPolygon(points, allPoints.size());
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
                /* qDebug() << "width: " << QString::number(width);
                qDebug() << "height: " << QString::number(height);
                const QRect rect(textBox.getX1(), textBox.getY1(), width, height);
                QRect boundingRect;
                QFont serifFont("Times", 12, QFont::Bold);
                painter.setFont(serifFont);
                painter.drawText(rect, Qt::TextWordWrap, tr(textBox.getTextWritten().toUtf8().data()), &boundingRect);
                painter.drawRect(boundingRect.adjusted(0, 0, width, height));
                painter.drawRect(rect.adjusted(0, 0, width, height)); */
                QTextEdit *textEdit = new QTextEdit(this);
                textEdit->setFrameStyle(QFrame::NoFrame);
                textEdit->viewport()->setAutoFillBackground(false);
                // QFont serifFont("Times", 12, QFont::Bold);
                textEdit->setFont(textBox.getFont());

                textEdit->setGeometry(textBox.getX1(), textBox.getY1(), width, height);
                textEdit->setText(textBox.getTextWritten());
                textEdit->show();
                textEditList.append(textEdit);
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
    QPainter painter(&image);
    for(int i = 0; i < drawnRectList.size(); i++) {
        painter.eraseRect(drawnRectList.at(i));
        drawnRectList.takeAt(i);
    }
    for(int i = 0; i < drawnRectPointerList.size(); i++) {
        painter.eraseRect(*drawnRectPointerList.at(i));
        delete drawnRectPointerList.at(i);
        drawnRectPointerList.removeAt(i);
    }
    image.fill(qRgb(255,255,255));
    modified = true;
    while(!textEditList.isEmpty()) {
        QTextEdit *textEdit = textEditList.dequeue();
        delete textEdit;
    }
    update();
}


void ScribbleArea::keyPressEvent(QKeyEvent *event) {
    if(currentlyTyping && (event->key() != Qt::Key_Escape)) {
        QTextEdit *textEdit = textEditList.last();
        QString tmpCurText = textEdit->toPlainText();
        tmpCurText += event->text();
        textEdit->setText(tmpCurText);
        textEdit->show();
        update();
    }
    if(currentlyTyping && (event->key() == Qt::Key_Backspace)) {
        QTextEdit *textEdit = textEditList.last();
        QString tmpCurText = textEdit->toPlainText();
        tmpCurText.remove(tmpCurText.length()-2, 2);
        textEdit->setText(tmpCurText);
        textEdit->show();
        update();
    }
    if(currentlyTyping && (event->key() == Qt::Key_Escape)) {
        QTextEdit *textEdit = textEditList.last();
        currentlyTyping = false;
        textEdit->setReadOnly(true);
        _toolSetHandling.updateTextBox(textEdit->toPlainText());
    }
}


void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(turnBoolOn) {
            lastPoint = event->pos();
            FreeHandLine freeHandLine;
            freeHandLine.setNewPoint(lastPoint);
            freeHandLine.setPenColor(myPenColor);
            freeHandLine.setPenWidth(myPenWidth);
            _toolSetHandling.addFreeHandLineToQueue(freeHandLine);
            int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
            int sizeOfFreeHandLineQueue = _toolSetHandling.getQueueOfFreeHandLines().size();
            int posFreeHandLineInQueue = (sizeOfFreeHandLineQueue -1);
            _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posFreeHandLineInQueue);
        }
        if(drawLineBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(textBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(setUpSquareBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
            drawingSquare.setX(m_x1);
            drawingSquare.setY(m_y1);
            startDrawingSquare = true;
        }
        if(setUpEllipseBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(secondConvexReadyToDraw && this->underMouse()) {
            if(secondNumberOfPointsDrawn < secondTotalNumNeedToDraw) {
                m_x1 = event->x();
                m_y1 = event->y();
                QPointF tmpPoint(m_x1, m_y1);
                secondCoordSet.append(tmpPoint);
                secondNumberOfPointsDrawn++;
                if(secondNumberOfPointsDrawn < secondTotalNumNeedToDraw) {
                    return;
                }
            }
            if(secondNumberOfPointsDrawn >= secondTotalNumNeedToDraw) {
                secondDrawConvexPolygon();
            }
        }
        if(setUpLinearGradientColorsBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(setUpConicalGradientColorsBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(setUpRadialGradientColorsBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(setUpSquircleBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
    }
}


void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        if(turnBoolOn) {
            drawLineTo(event->pos());
            FreeHandLine &curFreeHandLine = _toolSetHandling.obtainCurFreeHandLineInstance();
            curFreeHandLine.setNewPoint(event->pos());
        }
        if(setUpSquareBool && this->underMouse()) {
            QPainter painter(&image);
            if(startDrawingSquare == false) {
                painter.eraseRect(drawingSquare);
            }
            startDrawingSquare = false;
            int dx = event->x();
            int dy = event->y();
            int width = (dx - m_x1);
            int height = (dy - m_y1);

            drawingSquare.setWidth(width);
            drawingSquare.setHeight(height);
            painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
            painter.drawRect(drawingSquare);
            update();
        }
        if(setUpSquircleBool && this->underMouse()) {
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

            QPainter painter(&image);
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
            painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
            Qt::RoundCap, Qt::RoundJoin));
            painter.drawPath(path);
            update();
        }
    }
}


void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(turnBoolOn) {
            drawLineTo(event->pos());
            FreeHandLine curFreeHandLine = _toolSetHandling.obtainCurFreeHandLineInstance();
            curFreeHandLine.setNewPoint(event->pos());
            turnBoolOn = false;
        }
        if(drawLineBool) {
            drawLineBool = false;
            m_x2 = event->x();
            m_y2 = event->y();
            drawLine();
        }
        if(textBool) {
            m_x2 = event->x();
            m_y2 = event->y();
            createTextBlurb();
        }
        if(setUpSquareBool && this->underMouse()) {
            QPainter painter(&image);
            painter.eraseRect(drawingSquare);
            painter.end();
            m_x2 = event->x();
            m_y2 = event->y();
            createSquare();
        }
        if(setUpEllipseBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            createEllipse();
        }
        if(setUpLinearGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            linearGradientColorSelection(userChoseThisNumColors);
        }
        if(setUpConicalGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            conicalGradientColorSelection(userChoseThisNumColors);
        }
        if(setUpRadialGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            radialGradientColorSelection(userChoseThisNumColors);
        }
        if(setUpSquircleBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            drawSquircle();
        }
    }
}


void ScribbleArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}


void ScribbleArea::resizeEvent(QResizeEvent *event) {
    if(width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void ScribbleArea::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}


void ScribbleArea::redrawLineTo(const QPoint &endPoint, QPainter &curPainter) {
    curPainter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
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

    QPainter painter(&image);
    QPoint pointOne(m_x1, m_y1);
    QPoint pointTwo(m_x2, m_y2);

    StraightLine straightLine;
    straightLine.setPoints(pointOne, pointTwo);

    _toolSetHandling.addStraightLineToQueue(straightLine);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfStraightLineQueue = _toolSetHandling.getQueueOfStraightLines().size();
    int posStraightLineInQueue = (sizeOfStraightLineQueue - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posStraightLineInQueue);

    painter.drawEllipse(pointOne, 1, 1);
    painter.drawEllipse(pointTwo, 1, 1);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));

    painter.drawLine(pointOne, pointTwo);

    update();
}


void ScribbleArea::createTextBlurb() {
    currentlyTyping = true;

    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setFrameStyle(QFrame::NoFrame);
    textEdit->viewport()->setAutoFillBackground(false);
    int diff_x = (m_x2 - m_x1);
    int diff_y = (m_y2 - m_y1);

    _toolSetHandling.updateTextBox(textEdit->toPlainText());
    _toolSetHandling.addCoords(m_x1, m_x2, m_y1, m_y2);

    textEdit->setFont(_toolSetHandling.getFont());
    textEdit->setGeometry(m_x1, m_y1, diff_x, diff_y);
    textEdit->show();
    textEditList.append(textEdit);
}


void ScribbleArea::createSquare() {
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rect(m_x1, m_y1, width, height);
    Rectangle rectangle;
    rectangle.setCoords(m_x1, m_x2, m_y1, m_y2);
    rectangle.setPenColor(myPenColor);
    rectangle.setPenWidth(myPenWidth);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(rect);
    painter.end();

    _toolSetHandling.addRectangleToQueue(rectangle);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfRectangleQueue = _toolSetHandling.getQueueOfRectangles().size();
    int posSquareInQueue = (sizeOfRectangleQueue - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquareInQueue);

    drawnRectList << rect;
    setUpSquareBool = false;
    update();
}


void ScribbleArea::createEllipse() {
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rect(m_x1, m_y1, width, height);
    QPainter painter(&image);
    Ellipse ellipse;
    ellipse.setCoords(m_x1, m_x2, m_y1, m_y2);
    ellipse.setPenColor(myPenColor);
    ellipse.setPenWidth(myPenWidth);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(rect);

    _toolSetHandling.addEllipseToQueue(ellipse);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfEllipsesQueue = _toolSetHandling.getQueueOfEllipses().size();
    int posSquareInQueue = (sizeOfEllipsesQueue - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquareInQueue);

    drawnRectList << rect;
    setUpEllipseBool = false;
    update();
}


void ScribbleArea::secondDrawConvexPolygon() {
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    int arrSize = secondCoordSet.size();
    QPointF points[arrSize];
    ConvexPolygon convexPolygon;
    for(int i = 0; i < secondCoordSet.size(); i++) {
        points[i] = secondCoordSet.at(i);
        convexPolygon.addPointToQueue(secondCoordSet.at(i));
    }
    painter.drawConvexPolygon(points, arrSize);

    _toolSetHandling.addConvexPolygonToQueue(convexPolygon);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfConvexPolygonQueue = _toolSetHandling.getQueueOfConvexPolygons().size();
    int posConvexPolygonInQueue = (sizeOfConvexPolygonQueue - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posConvexPolygonInQueue);

    secondConvexReadyToDraw = false;
    secondCoordSet.clear();
    update();
}


void ScribbleArea::linearGradientColorSelection(int numColors) {
    _gradientColorInputDialog->showLinearGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors);
    connect(_gradientColorInputDialog, SIGNAL(linearGradientToolsSet()), this, SLOT(readyToDrawLinearGradient()));

    LinearGradientShape linearGradientShape;
    linearGradientShape.setCoords(m_x1, m_x2, m_y1, m_y2);

    _toolSetHandling.addLinearGradientShapeToQueue(linearGradientShape);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfLinearGradientShapes = _toolSetHandling.getQueueOfLinearGradientShapes().size();
    int posLinearGradientShapeInQueue = (sizeOfLinearGradientShapes - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posLinearGradientShapeInQueue);
}


void ScribbleArea::readyToDrawLinearGradient() {
    QLinearGradient curLinearGradient = _gradientColorInputDialog->getLinearGradientTools();
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectLinear(m_x1, m_y1, width, height);

    _toolSetHandling.updateLinearGradient(curLinearGradient);

    painter.fillRect(rectLinear, curLinearGradient);
    setUpLinearGradientColorsBool = false;
    drawnRectList << rectLinear;

    update();
}



void ScribbleArea::drawSquircle() {
    QPainter painter(&image);
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
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);

    Squircle squircle;
    squircle.setPainterPath(path);
    squircle.setPenWidth(myPenWidth);
    squircle.setPenColor(myPenColor);

    _toolSetHandling.addSquircleToQueue(squircle);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfSquircleQueue = _toolSetHandling.getQueueOfSquircles().size();
    int posSquircleInQueue = (sizeOfSquircleQueue - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquircleInQueue);

    setUpSquircleBool = false;
    update();
}


void ScribbleArea::conicalGradientColorSelection(int numColors) {
    _gradientColorInputDialog->showConicalGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors);
    connect(_gradientColorInputDialog, SIGNAL(conicalGradientToolsSet()), this, SLOT(readyToDrawConicalGradient()));

    ConicalGradientShape conicalGradientShape;
    conicalGradientShape.setCoords(m_x1, m_x2, m_y1, m_y2);

    _toolSetHandling.addConicalGradientShapeToQueue(conicalGradientShape);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfConicalGradientShapes = _toolSetHandling.getQueueOfConicalGradientShapes().size();
    int posConicalGradientShapeInQueue = (sizeOfConicalGradientShapes - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posConicalGradientShapeInQueue);
}


void ScribbleArea::readyToDrawConicalGradient() {
    QConicalGradient* curConicalGradient = _gradientColorInputDialog->getConicalGradientTools();
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectConical(m_x1, m_y1, width, height);

    _toolSetHandling.updateConicalGradient(*curConicalGradient);

    painter.fillRect(rectConical, *curConicalGradient);
    setUpConicalGradientColorsBool = false;
    drawnRectList << rectConical;

    update();
}


void ScribbleArea::radialGradientColorSelection(int numColors) {
    _gradientColorInputDialog->showRadialGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors,
                                                        image.width(), image.height());
    connect(_gradientColorInputDialog, SIGNAL(radialGradientToolsSet()), this, SLOT(readyToDrawRadialGradient()));

    RadialGradientShape radialGradientShape;
    radialGradientShape.setCoords(m_x1, m_x2, m_y1, m_y2);

    _toolSetHandling.addRadialGradientShapeToQueue(radialGradientShape);
    int posLastActionAdded = _toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfRadialGradientShapes = _toolSetHandling.getQueueOfRadialGradientShapes().size();
    int posRadialGradientShapeInQueue = (sizeOfRadialGradientShapes - 1);
    _toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posRadialGradientShapeInQueue);
}


void ScribbleArea::readyToDrawRadialGradient() {
    QRadialGradient* curRadialGradient = _gradientColorInputDialog->getRadialGradientTools();
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectRadial(m_x1, m_y1, width, height);

    _toolSetHandling.updateRadialGradient(*curRadialGradient);

    painter.fillRect(rectRadial, *curRadialGradient);
    setUpRadialGradientColorsBool = false;
    drawnRectList << rectRadial;

    update();
}


void ScribbleArea::print() {
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0,0,image);
    }
#endif // QT_CONFIG(printdialog)
}

// for when want to build "set font" tool
    // curFont = QFontDialog::getFont(
                // &ok, QFont("Helvetica [Cronyx]", 10), this);
    // painter.setFont(curFont);
