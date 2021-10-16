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
#include <QRadioButton>
#include <QLinearGradient>
#include <QConicalGradient>
#include <QSpinBox>
#include <QPointF>


class GradientColorInputDialog : public QWidget {
    Q_OBJECT

    void _setUpLinearGradientWidget();
    void _setUpConicalGradientWidget();
    void _setUpRadialGradientWidget();

    void _twoSetUpConicalGradientWidget();

public:
    GradientColorInputDialog(QWidget *parent = 0);
    ~GradientColorInputDialog();

    void showLinearGradientWidget(int x1, int y1, int x2, int y2, int numColors);
    void handleButton(QString tmpTitle, int position, QLabel *colorName);
    QLinearGradient getLinearGradientTools();

    void showConicalGradientWidget(int x1, int y1, int x2, int y2, int numColors);
    void handleConicalButton(QString tmpTitle, int position, QLabel *colorName);
    QConicalGradient* getConicalGradientTools();

    void showRadialGradientWidget(int x1, int y1, int x2, int y2, int numColors,
                                  int widgetHeight, int widgetWidth);
    void handleRadialButton(QString tmpTitle, int position, QLabel *colorName);
    QRadialGradient* getRadialGradientTools();

signals:
    void linearGradientToolsSet();
    void conicalGradientToolsSet();
    void radialGradientToolsSet();

public slots:
    void drawTheGradientShape();
    void clearOutLinearColorMap();

    void conicalDrawTheGradientShape();
    void clearOutConicalColorMap();

    void radialDrawTheGradientShape();
    void clearOutRadialColorMap();

private:
    QWidget *_linearWidget;
    QWidget *_conicalWidget;
    QWidget *_radialWidget;

    int _linearNumColors;
    int _linear_x1;
    int _linear_y1;
    int _linear_x2;
    int _linear_y2;

    int _conicalNumColors;
    int _conical_x1;
    int _conical_y1;
    int _conical_x2;
    int _conical_y2;

    int _radialNumColors;
    int _radial_x1;
    int _radial_y1;
    int _radial_x2;
    int _radial_y2;
    int _widgetHeight;
    int _widgetWidth;

    QGroupBox *groupBox;
    QLinearGradient _curLinearGradient;
    QRadioButton *_radioLinearOne;
    QRadioButton *_radioLinearTwo;
    QRadioButton *_radioLinearThree;
    QMap<QPushButton*, QMap<int, QColor>> mapCurColorChoices;

    QConicalGradient *_curConicalGradient;
    QSpinBox *_conicalSpinBox;
    QMap<QPushButton*, QMap<int, QColor>> conicalMapCurColorChoices;
    QMap<QString, QPushButton*> curConicalGradientStrColorsMap;
    QMap<QString, QPushButton*> curGradientStrColorsMap;

    QRadialGradient *_curRadialGradient;
    QMap<QPushButton*, QMap<int, QColor>> radialMapCurColorChoices;
    QMap<QString, QPushButton*> curRadialGradientStrColorsMap;

    QGroupBox *_radialGroupBox;
    QRadioButton *_radioRadialOne;
    QRadioButton *_radioRadialTwo;
    QRadioButton *_radioRadialThree;
    QSpinBox *_radialSpinBox;
    QSpinBox *_centerXSpinBox;
    QSpinBox *_centerYSpinBox;
};

#endif // GRADIENTCOLORINPUTDIALOG_H
