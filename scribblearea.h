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

#include <assert.h>

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

    bool isModified() const { return m_modified; }
    QColor penColor() const { return m_myPenColor; }
    int penWidth() const { return m_myPenWidth; }

    void setMx1(int x1);
    void setMy1(int x1);
    void setMx2(int x1);
    void setMy2(int x1);

    void setDrawLineBool();
    void drawLine();

    void setTextBlurb(QFont font);
    void createTextBlurb();

    void setPenUp();

    void setUpSquare();
    void createSquare();

    void setUpEllipse();
    void createEllipse();

    void setReadyToDrawConvexPolygonBool();
    void drawConvexPolygon();

    void setUpLinearGradientPaints(int numColors);
    void linearGradientColorSelection(int numColors);

    void setUpConicalGradientPaints(int numColors);
    void conicalGradientColorSelection(int numColors);

    void setUpRadialGradientPaints(int numColors);
    void radialGradientColorSelection(int numColors);

    void setUpDrawSquircle();
    void drawSquircle();

    void setUpUndoFunctionality();
    void restoreImage();

public slots:
    void about();
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

    bool m_modified;
    bool m_scribbling;

    int m_myPenWidth;
    QColor m_myPenColor;
    QImage m_image;
    QPoint m_lastPoint;
    ToolSetHandling m_toolSetHandling;

    bool m_drawLineBool;

    int m_x1;
    int m_x2;
    int m_y1;
    int m_y2;

    QString m_curText;
    int m_inputDiagFontSize;
    bool m_textSettingSet;

    bool m_fontSizeSet;
    QFont m_curFont;

    QQueue<QTextEdit *> m_textEditList;

    bool m_textBool;
    bool m_currentlyTyping;

    bool m_turnBoolOn;
    bool m_setUpSquareBool;

    bool m_setUpEllipseBool;
    QQueue<QPainterPath *> m_mouseMovingEllipses;

    bool m_convexReadyToDraw;
    int m_numberOfPointsDrawn;
    int m_totalNumNeedToDraw;
    QList<QPointF> m_coordSet;

    QWidget *m_userInput;
    int m_userChoseThisNumColors;

    QSignalMapper m_mapper;
    QMap<QString, QPushButton*> m_curGradientStrColorsMap;
    QMap<QPushButton*, QMap<int, QColor>> m_mapCurColorChoices;

    QGroupBox *m_groupBox;
    QList<QCheckBox *> m_checkBoxBtnsList;

    GradientColorInputDialog *m_gradientColorInputDialog;

    bool m_setUpLinearGradientColorsBool;
    bool m_setUpConicalGradientColorsBool;
    bool m_setUpRadialGradientColorsBool;

    QList<QRectF> m_drawnRectList;
    QList<QRectF*> m_drawnRectPointerList;

    QRectF m_drawingSquare;
    bool m_startDrawingSquare;

    QRectF m_drawingRoundedSquare;
    bool m_startDrawingRoundedSquare;

    bool m_setUpSquircleBool;
    int m_initialDiff;
    int m_xIncrement;

};

#endif // SCRIBBLEAREA_H
