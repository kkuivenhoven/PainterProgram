#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QLinearGradient>
#include <QPen>
#include <QRectF>
#include <QPainterPath>
#include <QKeyEvent>
#include <QTextEdit>
#include <QInputDialog>
#include <QDir>
#include <QLabel>
#include <QLineEdit>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);
    ~ScribbleArea();
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setEasel(const QColor &fillColor);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

    void setMx1(int x1);
    void setMy1(int x1);
    void setMx2(int x1);
    void setMy2(int x1);

    void setDrawLineBool();
    void drawLine();

    void setTextBlurbBtn();
    void getUserInput();

    void setThirdTextBlurb();
    void createThirdTextBlurb();

    void setPenUp();

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;

    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;

    bool drawLineBool;
    int m_x1;
    int m_x2;
    int m_y1;
    int m_y2;

    QString curText;
    int inputDiagFontSize;
    bool textSettingSet;

    bool fontSizeSet;
    QFont curFont;

    QTextEdit *textEditThree;
    bool thirdTextBool;
    bool currentlyTypingThree;
};

#endif // SCRIBBLEAREA_H
