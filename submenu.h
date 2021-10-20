#ifndef SUBMENU_H
#define SUBMENU_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QColor>
#include <QColorDialog>
#include <QInputDialog>
#include <QStyle>
#include <QQuickView>
#include <QFontDialog>

#include "scribblearea.h"

class SubMenu : public QWidget
{
    Q_OBJECT
    ScribbleArea& _scribbleArea;

public:
    SubMenu(ScribbleArea& scribbleArea, QWidget *parent = 0);
    ~SubMenu();

private slots:
    void clearScreen();
    void fillEasel();

    void changePenColor();
    void changePenWidth();
    void setUpPaintBrush();

    void createLine();
    void createTextBlurb();

    void setUpSquare();
    void setUpEllipse();
    void setUpConvexPolygon();

    void setUpHandDrawnSquircle();

    void userChooseGradientPaintColors();

    void setUpUndoBtn();
};

#endif // SUBMENU_H
