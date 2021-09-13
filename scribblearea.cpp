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

    m_x1, m_x2, m_y1, m_y2 = 0;
    drawLineBool = false;
}

ScribbleArea::~ScribbleArea() {

}

void ScribbleArea::setDrawLineBool() {
    drawLineBool = true;
}

void ScribbleArea::setDrawTextBool() {
    drawTextBool = true;
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

void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
        if(drawLineBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
        }
        if(drawTextBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
            getUserInput();
            drawTextBool = false;
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::LeftButton) && scribbling) {
        if(drawLineBool == false) {
           drawLineTo(event->pos());
        }
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && scribbling) {
        if(drawLineBool == false) {
            drawLineTo(event->pos());
        }
        scribbling = false;
        if(drawLineBool) {
            drawLineBool = false;
            m_x2 = event->x();
            m_y2 = event->y();
            drawLine();
        }
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect(); // ? was this always here?
    painter.drawImage(dirtyRect, image, dirtyRect); // this too?
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

void ScribbleArea::setTextPointBool() {
    drawTextBool = true;
}

void ScribbleArea::getUserInput() {
    if((m_x1 == 0) && (m_y1 == 0)) {
        qDebug() << "Member x,y values not set for drawing text.";
        return;
    }
    bool okay;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Text to display:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &okay);
    if(okay && !text.isEmpty()) {
        curText = text;
    }
    setTextBlurbBtn();
}

void ScribbleArea::setTextBlurbBtn() {
    if((m_x1 == 0) && (m_y1 == 0)) {
        qDebug() << "Member x,y values not set for drawing text.";
        return;
    }
    QPainter painter(&image);
    QPointF topLeftPos(m_x1, m_y1);
    // const QStaticText staticTxt("Shalom world!");
    const QStaticText staticTxt(curText);
    painter.drawStaticText(topLeftPos, staticTxt);

    update();

    /* QLabel *label = new QLabel(this);
    label->setText("TESTING OF THIS LABEL"); */

    /* QTextEdit *txt = new QTextEdit(this);
    // txt->setGeometry(m_x1, m_y1);
    QPoint pointOne(m_x1, m_y1);
    txt->anchorAt(pointOne);
    txt->setText("Hello, world!");
    txt->append("Appending some text...");
    txt->show(); */
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
