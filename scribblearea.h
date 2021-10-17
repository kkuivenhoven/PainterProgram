#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QLinearGradient>
#include <QPen>
#include <QRectF>
#include <QRect>
#include <QPainterPath>
#include <QKeyEvent>
#include <QTextEdit>
#include <QInputDialog>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QPair>
#include <QList>
#include <QPointF>
#include <QGradient>
#include <QPixmap>
#include <QMap>
#include <QSignalMapper>
#include <QColorDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QPainterPath>

#include "gradientcolorinputdialog.h"
#include "toolsethandling.h"

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

    void setTextBlurb();
    void createTextBlurb();

    void setPenUp();

    void setUpSquare();
    void createSquare();

    void setUpEllipse();
    void createEllipse();

    void setReadyToDrawConvexPolygonBool();
    void secondDrawConvexPolygon();

    void setUpLinearGradientPaints(int numColors);
    void linearGradientColorSelection(int numColors);

    void setUpConicalGradientPaints(int numColors);
    void conicalGradientColorSelection(int numColors);

    void setUpRadialGradientPaints(int numColors);
    void radialGradientColorSelection(int numColors);

    void setUpDrawSquircle();
    void drawSquircle();

    void setUpUndoFunctionality();

public slots:
    void clearImage();
    void print();
    void readyToDrawLinearGradient();
    void readyToDrawConicalGradient();
    void readyToDrawRadialGradient();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void redrawLineTo(const QPoint &endPoint, QPainter &curPainter);
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

    QTextEdit *textEdit;
    bool textBool;
    bool currentlyTyping;

    bool turnBoolOn;
    bool setUpSquareBool;
    bool setUpEllipseBool;

    bool secondConvexReadyToDraw;
    int secondNumberOfPointsDrawn;
    int secondTotalNumNeedToDraw;
    QList<QPointF> secondCoordSet;

    QWidget *userInput;
    int userChoseThisNumColors;

    QSignalMapper mapper;
    QMap<QString, QPushButton*> curGradientStrColorsMap;
    QMap<QPushButton*, QMap<int, QColor>> mapCurColorChoices;

    QGroupBox *groupBox;
    QList<QCheckBox *> checkBoxBtnsList;

    GradientColorInputDialog *_gradientColorInputDialog;

    bool setUpLinearGradientColorsBool;
    bool setUpConicalGradientColorsBool;
    bool setUpRadialGradientColorsBool;

    QList<QRectF> drawnRectList;
    QList<QRectF*> drawnRectPointerList;

    QRectF drawingSquare;
    bool startDrawingSquare;

    QRectF drawingRoundedSquare;
    bool startDrawingRoundedSquare;

    bool setUpSquircleBool;
    int initialDiff;
    int xIncrement;
    ToolSetHandling toolSetHandling;
};

#endif // SCRIBBLEAREA_H
