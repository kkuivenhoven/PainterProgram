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
    QWidget *m_linearWidget;
    QWidget *m_conicalWidget;
    QWidget *m_radialWidget;

    int m_linearNumColors;
    int m_linear_x1;
    int m_linear_y1;
    int m_linear_x2;
    int m_linear_y2;

    int m_conicalNumColors;
    int m_conical_x1;
    int m_conical_y1;
    int m_conical_x2;
    int m_conical_y2;

    int m_radialNumColors;
    int m_radial_x1;
    int m_radial_y1;
    int m_radial_x2;
    int m_radial_y2;
    int m_widgetHeight;
    int m_widgetWidth;

    QGroupBox *m_groupBox;
    QLinearGradient m_curLinearGradient;
    QRadioButton *m_radioLinearOne;
    QRadioButton *m_radioLinearTwo;
    QRadioButton *m_radioLinearThree;
    QMap<QPushButton*, QMap<int, QColor>> m_mapCurColorChoices;

    QConicalGradient *m_curConicalGradient;
    QSpinBox *m_conicalSpinBox;
    QMap<QPushButton*, QMap<int, QColor>> m_conicalMapCurColorChoices;
    QMap<QString, QPushButton*> m_curConicalGradientStrColorsMap;
    QMap<QString, QPushButton*> m_curGradientStrColorsMap;

    QRadialGradient *m_curRadialGradient;
    QMap<QPushButton*, QMap<int, QColor>> m_radialMapCurColorChoices;
    QMap<QString, QPushButton*> m_curRadialGradientStrColorsMap;

    QGroupBox *m_radialGroupBox;
    QRadioButton *m_radioRadialOne;
    QRadioButton *m_radioRadialTwo;
    QRadioButton *m_radioRadialThree;
    QSpinBox *m_radialSpinBox;
    QSpinBox *m_centerXSpinBox;
    QSpinBox *m_centerYSpinBox;
};

#endif // GRADIENTCOLORINPUTDIALOG_H
