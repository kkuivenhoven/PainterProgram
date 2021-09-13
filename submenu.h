#ifndef SUBMENU_H
#define SUBMENU_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QColor>
#include <QColorDialog>
#include <QInputDialog>

#include "scribblearea.h"

class SubMenu : public QWidget
{
    Q_OBJECT
    ScribbleArea& _scribbleArea;

public:
    SubMenu(ScribbleArea& scribbleArea, QWidget *parent = 0);
    ~SubMenu();

private slots:
    void fillEasel();
    void changePenColor();
    void changePenWidth();
    void clearScreen();
    void createLine();
    void createTextBlurb();
};

#endif // SUBMENU_H
