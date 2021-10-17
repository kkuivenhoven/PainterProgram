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


void ScribbleArea::setTextBlurb() {
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
    QStack<QString> orderOfActions = toolSetHandling.getOrderOfObjectsDrawn();
    if(orderOfActions.size() >= 1) {
        for(int i = (orderOfActions.size()-1); i < orderOfActions.size(); i++) {
            QString action = orderOfActions.at(i);
            if(action == ToolSetHandling::RECTANGLE) {
                toolSetHandling.removeLastRectangle();
            }
            if(action == ToolSetHandling::ELLIPSE) {
                toolSetHandling.removeLastEllipse();
            }
            if(action == ToolSetHandling::SQUIRCLE) {
                toolSetHandling.removeLastSquircle();
            }
            toolSetHandling.removeLastPosStored(orderOfActions.size()-1);
            toolSetHandling.removeLastActionFromStack();
        }
    }
    QQueue<Rectangle> rectangleQueue = toolSetHandling.getQueueOfRectangles();
    QQueue<Ellipse> ellipseQueue = toolSetHandling.getQueueOfEllipses();
    QQueue<Squircle> squircleQueue = toolSetHandling.getQueueOfSquircles();
    QStack<QString> newOrderOfActions = toolSetHandling.getOrderOfObjectsDrawn();
    QMap<int /*posInActionStack*/,
         int /*posInShapeStack*/> posMap = toolSetHandling.getPosMap();

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
    update();
}


void ScribbleArea::keyPressEvent(QKeyEvent *event) {
    if(currentlyTyping && (event->key() != Qt::Key_Escape)) {
        QString tmpCurText = textEdit->toPlainText();
        tmpCurText += event->text();
        textEdit->setText(tmpCurText);
        textEdit->show();
        update();
    }
    if(currentlyTyping && (event->key() == Qt::Key_Backspace)) {
        QString tmpCurText = textEdit->toPlainText();
        tmpCurText.remove(tmpCurText.length()-2, 2);
        textEdit->setText(tmpCurText);
        textEdit->show();
        update();
    }
}


void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(turnBoolOn) {
            lastPoint = event->pos();
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

    painter.drawEllipse(pointOne, 1, 1);
    painter.drawEllipse(pointTwo, 1, 1);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));

    painter.drawLine(pointOne, pointTwo);

    update();
}


void ScribbleArea::createTextBlurb() {
    currentlyTyping = true;
    textEdit = new QTextEdit(this);
    textEdit->setFrameStyle(QFrame::NoFrame);
    textEdit->viewport()->setAutoFillBackground(false);
    int diff_x = (m_x2 - m_x1);
    int diff_y = (m_y2 - m_y1);
    textEdit->setGeometry(m_x1, m_y1, diff_x, diff_y);
    textEdit->show();
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

    toolSetHandling.addRectangleToQueue(rectangle);
    int posLastActionAdded = toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfRectangleQueue = toolSetHandling.getQueueOfRectangles().size();
    int posSquareInQueue = (sizeOfRectangleQueue - 1);
    toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquareInQueue);

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

    toolSetHandling.addEllipseToQueue(ellipse);
    int posLastActionAdded = toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfEllipsesQueue = toolSetHandling.getQueueOfEllipses().size();
    int posSquareInQueue = (sizeOfEllipsesQueue - 1);
    toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquareInQueue);

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
    for(int i = 0; i < secondCoordSet.size(); i++) {
        points[i] = secondCoordSet.at(i);
    }
    painter.drawConvexPolygon(points, arrSize);
    secondConvexReadyToDraw = false;
    secondCoordSet.clear();
    update();
}


void ScribbleArea::linearGradientColorSelection(int numColors) {
    _gradientColorInputDialog->showLinearGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors);
    connect(_gradientColorInputDialog, SIGNAL(linearGradientToolsSet()), this, SLOT(readyToDrawLinearGradient()));
}


void ScribbleArea::readyToDrawLinearGradient() {
    QLinearGradient curLinearGradient = _gradientColorInputDialog->getLinearGradientTools();
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectLinear(m_x1, m_y1, width, height);

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

    toolSetHandling.addSquircleToQueue(squircle);
    int posLastActionAdded = toolSetHandling.getPositionOfLastActionAdded();
    int sizeOfSquircleQueue = toolSetHandling.getQueueOfSquircles().size();
    int posSquircleInQueue = (sizeOfSquircleQueue - 1);
    toolSetHandling.setActionPosAndShapePos(posLastActionAdded, posSquircleInQueue);

    setUpSquircleBool = false;
    update();
}


void ScribbleArea::conicalGradientColorSelection(int numColors) {
    _gradientColorInputDialog->showConicalGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors);
    connect(_gradientColorInputDialog, SIGNAL(conicalGradientToolsSet()), this, SLOT(readyToDrawConicalGradient()));
}


void ScribbleArea::readyToDrawConicalGradient() {
    QConicalGradient* curConicalGradient = _gradientColorInputDialog->getConicalGradientTools();
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectConical(m_x1, m_y1, width, height);

    painter.fillRect(rectConical, *curConicalGradient);
    setUpConicalGradientColorsBool = false;
    drawnRectList << rectConical;

    update();
}


void ScribbleArea::radialGradientColorSelection(int numColors) {
    _gradientColorInputDialog->showRadialGradientWidget(m_x1, m_y1, m_x2, m_y2, numColors,
                                                        image.width(), image.height());
    connect(_gradientColorInputDialog, SIGNAL(radialGradientToolsSet()), this, SLOT(readyToDrawRadialGradient()));
}


void ScribbleArea::readyToDrawRadialGradient() {
    QRadialGradient* curRadialGradient = _gradientColorInputDialog->getRadialGradientTools();
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectRadial(m_x1, m_y1, width, height);

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
