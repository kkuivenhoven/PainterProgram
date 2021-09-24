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
    qDebug() << "ScribbleArea::setUpLinearGradient()";
    setUpLinearGradientBool = true;
}


void ScribbleArea::setUpGradientPaints() {
    inputDialogForGradientPaints();
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
    // gradient.setColorAt(0, Qt::white);
    // gradient.setColorAt(0.5, Qt::green);
    // gradient.setColorAt(1, Qt::black);

    // gradient.setColorAt(0, Qt::gray);
    gradient.setColorAt(0, myPenColor);
    gradient.setColorAt(1, Qt::black);

    painter.fillRect(rectLinear, gradient);
    setUpLinearGradientBool = false;
    update();
}

void ScribbleArea::inputDialogForGradientPaints() {
    QDialog dialog(this);
    // the below line allows there to be a label next to each field
    QFormLayout form(&dialog);

    form.addRow(new QLabel("Pick two colors for your gradients, please."));
    // QList<QLineEdit *> fields;
    QList<QPixmap *> fields;
    for(int i = 0; i < 2; i++) {
        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("Value %1").arg(i + 1);
        // form.addRow(label, lineEdit);
        QPixmap *pixMap = new QPixmap(16, 16);
        pixMap->fill("#ffff45");
        /* form.addRow(label, pixMap);

        // fields << lineEdit;
        fields << pixMap;
        */
    }

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    /* if(dialog.exec() == QDialog::Accepted) {
        foreach(QLineEdit *lineEdit, fields) {
            qDebug() << lineEdit->text();
        }
    } */
    QList<QPair<QString, QColor>> list;
    list << QPair<QString,QColor>(tr("Color #1"), QColor("green")) <<
            QPair<QString,QColor>(tr("Color #2"), QColor("red"));

    QTableWidget *table = new QTableWidget(3, 2);
    table->setHorizontalHeaderLabels(QStringList() << tr("Color placement")
                                                   << tr("Color chosen"));
    table->verticalHeader()->setVisible(false);
    table->resize(150,150);
    QTableWidgetItem *nameItem = new QTableWidgetItem("1");
    table->setItem(0, 0, nameItem);
    QTableWidgetItem *colorItem = new QTableWidgetItem(0);
    table->setItem(0, 1, colorItem);

    QTableWidgetItem *nameItemTwo = new QTableWidgetItem("2");
    table->setItem(1, 0, nameItemTwo);
    QTableWidgetItem *colorItemTwo = new QTableWidgetItem(0);
    table->setItem(1, 1, colorItemTwo);

    QWidget* newWidget = new QWidget();

    table->resizeColumnsToContents();
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table, 0, 0);

    newWidget->setLayout(layout);
    newWidget->setWindowTitle(tr("Color Edit"));
    newWidget->show();

        // QPair<QString, QColor> pair = list.takeAt(i);
        // QTableWidgetItem *colorNum = new QTableWidgetItem(pair.first);
        // QTableWidgetItem *colorItem = new QTableWidgetItem;
        // colorItem->setData(Qt::DisplayRole, pair.second);

        // table->setItem(i, 0, colorNum);
        // table->setItem(i, 1, colorItem);
    // table->resizeColumnsToContents(0);
    /* table->resizeColumnsToContents();
    table->horizontalHeader()->setStretchLastSection(true);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table, 0, 0);

    setLayout(layout);

    setWindowTitle(tr("Color Edit"));
    */
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

