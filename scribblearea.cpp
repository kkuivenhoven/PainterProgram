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
    textSettingSet = false;

    fontSizeSet = false;
    currentlyTyping = false;
    secondTextBool = false;

    setFocusPolicy(Qt::StrongFocus);
}

ScribbleArea::~ScribbleArea() {

}

void ScribbleArea::setDrawLineBool() {
    drawLineBool = true;
}

void ScribbleArea::setDrawTextBool() {
    drawTextBool = true;
}

void ScribbleArea::setSecondTextBlurb() {
    secondTextBool = true;
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
    qDebug() << "inside keyPressEvent()";
    if(currentlyTyping && (event->key() != Qt::Key_Escape)) {
        qDebug() << "Key press event -- currently typing\n";
        /* QString tmpCurText = labelOne->text();
        // tmpCurText += QString::number(event->key());
        tmpCurText += event->text();
        labelOne->setText(tmpCurText);
        labelOne->show(); */

        QString tmpCurTextTwo = textEditOne->toPlainText();
        tmpCurTextTwo += event->text();
        textEditOne->setText(tmpCurTextTwo);
        textEditOne->show();

        update();
    }
    if(currentlyTyping && (event->key() == Qt::Key_Escape)) {
        currentlyTyping = false;
        fontSizeSet = false;
    }
}

void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
        if(drawLineBool && this->underMouse()) {
            qDebug() << " drawLineBool is true and under mouse";
            m_x1 = event->x();
            m_y1 = event->y();
        }
        /* if(drawTextBool && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
            getUserInput();
            drawTextBool = false;
        } */
        /* if(textSettingSet && this->underMouse()) {
            m_x1 = event->x();
            m_y1 = event->y();
            setTextBlurbBtn();
            textSettingSet = false;
            drawTextBool = false;
        } */
        if(fontSizeSet && this->underMouse()) {
            qDebug() << " font size set and under mouse";
            m_x1 = event->x();
            m_y1 = event->y();
            setUpActiveText();
        }
        if(secondTextBool && this->underMouse()) {
            qDebug() << " second bool is true ";
            m_x1 = event->x();
            m_y1 = event->y();
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::LeftButton) && scribbling) {
        if(drawLineBool == false) {
           drawLineTo(event->pos());
        }
        // can add functionality for straight line tool here
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
        if(secondTextBool) {
            qDebug() << "inside second text bool";
            m_x2 = event->x();
            m_y2 = event->y();
            createSecondTextBlurb();
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
    QStringList fontSizes;
    for(int i = 8; i < 85; i++) {
        fontSizes << QString::number(i);
    }

    bool okay;
    QDialog *d = new QDialog();
    QVBoxLayout *vbox = new QVBoxLayout();
    QLabel *labelA = new QLabel("Select font size:");
    QComboBox *textSizeOptions = new QComboBox();
    // textSizeOptions->addItems(QStringList() << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22");
    textSizeOptions->addItems(fontSizes);
    QLabel *labelB = new QLabel("Enter text:");
    QLineEdit *lineEditB = new QLineEdit();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                       QDialogButtonBox::Cancel);
    QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

    vbox->addWidget(labelA);
    vbox->addWidget(textSizeOptions);
    vbox->addWidget(labelB);
    vbox->addWidget(lineEditB);
    vbox->addWidget(buttonBox);

    d->setLayout(vbox);

    int result = d->exec();
    if(result == QDialog::Accepted) {
        // handle values from d
        inputDiagFontSize = textSizeOptions->currentText().toInt();
        curText = lineEditB->text();
    }
    textSettingSet = true;
}


void ScribbleArea::setTextBlurbBtn() {
    bool ok;
    // QFont font = QFontDialog::getFont(
    curFont = QFontDialog::getFont(
                &ok, QFont("Helvetica [Cronyx]", 10), this);
    QPainter painter(&image);
    // QPointF topLeftPos(m_x1, m_y1);
    // QFont font("times", inputDiagFontSize);
    painter.setFont(curFont);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));

    fontSizeSet = true;
    qDebug() << " font size set is true";
}


void ScribbleArea::setUpActiveText() {
    currentlyTyping = true;
    /* labelOne = new QLabel(this);
    labelOne->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    labelOne->setText("");
    labelOne->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    labelOne->setGeometry(m_x1,m_y1,100,100);
    labelOne->show(); */

    textEditOne = new QTextEdit(this);
    textEditOne->setCurrentFont(curFont);
    textEditOne->setText("");
    textEditOne->setGeometry(m_x1, m_y1, 200, 200);
    textEditOne->show();
}


void ScribbleArea::createSecondTextBlurb() {
    qDebug() << "inside createSecondTextBlurb()";
    textEditTwo = new QTextEdit(this);
    textEditTwo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    textEditTwo->setCurrentFont(curFont);
    textEditTwo->setText("testing");
    int diff_x = (m_x2 - m_x1);
    int diff_y;//  = (m_y2 - m_y1);
    if(m_y1 > m_y2) {
        diff_y = (m_y1 - m_y2);
    }
    if(m_y2 > m_y1) {
        diff_y = (m_y2 - m_y1);
    }
    if(m_y2 == m_y1) {
        diff_y = 1;
    }
    qDebug() << "m_x1: " << QString::number(m_x1);
    qDebug() << "m_y1: " << QString::number(m_y1);
    qDebug() << "m_x2: " << QString::number(m_x2);
    qDebug() << "m_y2: " << QString::number(m_y2);
    qDebug() << "diff_x: " << QString::number(diff_x);
    qDebug() << "diff_y: " << QString::number(diff_y);
    if(m_y1 > m_y2) {
        textEditTwo->setGeometry(m_x1, m_y2, diff_x, diff_y);
    } else {
        textEditTwo->setGeometry(m_x1, m_y1, diff_x, diff_y);
    }
    textEditTwo->show();
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

