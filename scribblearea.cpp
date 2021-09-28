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

// for when want to build "set font" tool
    // curFont = QFontDialog::getFont(
                // &ok, QFont("Helvetica [Cronyx]", 10), this);
    // painter.setFont(curFont);


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
    setUpRoundSquareBool = false;

    secondConvexReadyToDraw = false;
    setUpLinearGradientBool = false;
    setUpGradientColorsBool = false;

    colorDiag = new QColorDialog();

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


void ScribbleArea::setUpRoundSquare() {
    setUpRoundSquareBool = true;
}


void ScribbleArea::setUpLinearGradient() {
    setUpLinearGradientBool = true;
}


void ScribbleArea::setUpGradientPaints(int numColors) {
    userChoseThisNumColors = numColors;
    setUpGradientColorsBool = true;
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


void ScribbleArea::clearImage() {
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
        if((setUpSquareBool || setUpRoundSquareBool) && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
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
        if(setUpLinearGradientBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(setUpGradientColorsBool && this->underMouse()) {
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
            m_x2 = event->x();
            m_y2 = event->y();
            createSquare();
        }
        if(setUpRoundSquareBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            createRoundSquare();
        }
        if(setUpEllipseBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            createEllipse();
        }
        if(setUpLinearGradientBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            createLinearGradient();
        }
        if(setUpGradientColorsBool && this->underMouse()) {
            m_x2 = event->x();
            m_y2 = event->y();
            inputDialogForGradientPaints(userChoseThisNumColors);
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
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rect(m_x1, m_y1, width, height);
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(rect);
    setUpSquareBool = false;
    update();
}


void ScribbleArea::createEllipse() {
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rect(m_x1, m_y1, width, height);
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(rect);
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


void ScribbleArea::createRoundSquare() {
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rect(m_x1, m_y1, width, height);
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawRoundedRect(rect, 20.0, 15.0);
    setUpRoundSquareBool = false;
    update();
}


void ScribbleArea::createLinearGradient() {
    QPainter painter(&image);
    int width = (m_x2 - m_x1);
    int height = (m_y2 - m_y1);
    QRect rectLinear(m_x1, m_y1, width, height);

    QLinearGradient gradient(rectLinear.topLeft(), rectLinear.bottomRight());

    gradient.setColorAt(0, myPenColor);
    gradient.setColorAt(0.5, Qt::green);
    gradient.setColorAt(1, Qt::black);

    painter.fillRect(rectLinear, gradient);
    setUpLinearGradientBool = false;
    update();
}


void ScribbleArea::inputDialogForGradientPaints(int numColors) {
    userInput = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < userChoseThisNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);

        QPushButton *colorButton = new QPushButton(firstColor.name());
        curGradientStrColorsMap.insert(tmpTitle, colorButton);
        mapper.setMapping(colorButton, tmpTitle);
        connect(colorButton, &QPushButton::released, [=] {
            handleButton(tmpTitle, tmpNum);
        });

        QString qss = QString("background-color: %1").arg(firstColor.name());
        colorButton->setStyleSheet(qss);

        gridLayout->addWidget(colorLabel, i, 0, 1, 1);
        gridLayout->addWidget(colorButton, i, 2, 1, 1);
    }

    userInput->setLayout(gridLayout);
    userInput->show();
}


// https://stackoverflow.com/questions/21150890/qt-5-assign-slot-with-parameters-to-a-qpushbutton
// https://stackoverflow.com/questions/5153157/passing-an-argument-to-a-slot
void ScribbleArea::handleButton(QString tmpTitle, int position) {
    QPushButton *curPushButton = curGradientStrColorsMap.value(tmpTitle);

    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(mapCurColorChoices.contains(curPushButton)) {
            mapCurColorChoices.remove(curPushButton);
            mapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            mapCurColorChoices.insert(curPushButton, tmpMap);
        }
        update();
    }

    if(mapCurColorChoices.size() == userChoseThisNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = mapCurColorChoices.begin(); iter != mapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        QPainter painter(&image);
        int width = (m_x2 - m_x1);
        int height = (m_y2 - m_y1);
        QRect rectLinear(m_x1, m_y1, width, height);

        QLinearGradient gradient(rectLinear.topLeft(), rectLinear.bottomRight());
        // float decimal = (1.0/userChoseThisNumColors);
        float decimal = (1.0/(userChoseThisNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt-1));
            qDebug() << " ----- tmpDec: " << tmpDec;
            QColor tmpColor = tmpIter.value();
            gradient.setColorAt(tmpDec, tmpColor);
        }

        painter.fillRect(rectLinear, gradient);
        setUpGradientColorsBool = false;
        userInput->close();
        delete userInput;
        mapCurColorChoices.clear();
        update();
    }
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

