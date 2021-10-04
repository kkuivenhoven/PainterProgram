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


class GradientColorInputDialog : public QWidget
{
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

signals:
    void linearGradientToolsSet();
    void conicalGradientToolsSet();
    void twoConicalGradientToolsSet();

public slots:
    void drawTheGradientShape();
    void clearOutLinearColorMap();

    void conicalDrawTheGradientShape();
    void clearOutConicalColorMap();

private:
    QMap<QString, QPushButton*> curGradientStrColorsMap;

    QWidget *_linearWidget;
    QWidget *_conicalWidget;
    QWidget *_radialWidget;

    QGroupBox *groupBox;

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

    QLinearGradient _curLinearGradient;
    QRadioButton *_radioLinearOne;
    QRadioButton *_radioLinearTwo;
    QRadioButton *_radioLinearThree;
    QMap<QPushButton*, QMap<int, QColor>> mapCurColorChoices;

    QConicalGradient *_curConicalGradient;
    QSpinBox *_conicalSpinBox;
    QMap<QPushButton*, QMap<int, QColor>> conicalMapCurColorChoices;
    QMap<QString, QPushButton*> curConicalGradientStrColorsMap;
};

#endif // GRADIENTCOLORINPUTDIALOG_H
