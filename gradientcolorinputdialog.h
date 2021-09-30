#ifndef GRADIENTCOLORINPUTDIALOG_H
#define GRADIENTCOLORINPUTDIALOG_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QGroupBox>
#include <QCheckBox>
#include <QDebug>
#include <QColorDialog>
#include <QPainter>

// #include "scribblearea.h"

class GradientColorInputDialog : public QWidget
{
    Q_OBJECT

    void _setUpLinearGradientWidget();
    void _setUpConicalGradientWidget();
    void _setUpRadialGradientWidget();

public:
    // GradientColorInputDialog(QImage& image, QWidget *parent = 0);
    GradientColorInputDialog(QWidget *parent = 0);
    ~GradientColorInputDialog();

    void showLinearGradientWidget(int x1, int y1, int x2, int y2);
    void handleButton(QString tmpTitle, int position, QLabel *colorName);
    // void getLinearRectGradientMap();
    // void getLinearGradientTools();
    // QMap<QRect, QLinearGradient> getLinearGradientTools();
    QLinearGradient getLinearGradientTools();

signals:
    void linearGradientToolsSet();

public slots:
    void drawTheGradientShape();

private:
    QMap<QString, QPushButton*> curGradientStrColorsMap;
    QMap<QPushButton*, QMap<int, QColor>> mapCurColorChoices;
    QList<QCheckBox *> checkBoxBtnsList;

    QWidget *_linearWidget;
    QWidget *_conicalWidget;
    QWidget *_radialWidget;

    QGroupBox *groupBox;

    int _linear_x1;
    int _linear_y1;

    int _linear_x2;
    int _linear_y2;
    QMap<QRect, QLinearGradient> rectGradientList;

    QLinearGradient _curLinearGradient;
};

#endif // GRADIENTCOLORINPUTDIALOG_H
