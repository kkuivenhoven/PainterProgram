#include "submenu.h"

SubMenu::SubMenu(ScribbleArea &scribbleArea, QWidget *parent) : _scribbleArea(scribbleArea), QWidget(parent) {
    QWidget *subMenu = new QWidget();

    QGridLayout *gridLayout = new QGridLayout();

    QPushButton *fillEaselBtn = new QPushButton();
    fillEaselBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/PaintBucket.png"));
    connect(fillEaselBtn, SIGNAL(clicked(bool)), this, SLOT(fillEasel()));

    QPushButton *penColorBtn = new QPushButton();
    penColorBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/PaintBrush.png"));
    connect(penColorBtn, SIGNAL(clicked(bool)), this, SLOT(changePenColor()));

    QPushButton *penWidthBtn = new QPushButton();
    penWidthBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/3366100-200.png"));
    connect(penWidthBtn, SIGNAL(clicked(bool)), this, SLOT(changePenWidth()));

    QPushButton *clearScreenBtn = new QPushButton();
    clearScreenBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/clear-screen.jpg"));
    connect(clearScreenBtn, SIGNAL(clicked(bool)), this, SLOT(clearScreen()));

    QPushButton *createRectBtn = new QPushButton();
    createRectBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/PaintBrush.png"));
    connect(createRectBtn, SIGNAL(clicked(bool)), this, SLOT(createRectangle()));

    gridLayout->addWidget(fillEaselBtn);
    gridLayout->addWidget(penColorBtn);
    gridLayout->addWidget(penWidthBtn);
    gridLayout->addWidget(clearScreenBtn);
    gridLayout->addWidget(createRectBtn);

    subMenu->setLayout(gridLayout);
    subMenu->show();
}

SubMenu::~SubMenu() {

}

void SubMenu::fillEasel() {
    _scribbleArea.setEasel(_scribbleArea.penColor());
}

void SubMenu::changePenColor() {
    QColor newColor = QColorDialog::getColor(_scribbleArea.penColor());
    if(newColor.isValid()) {
        _scribbleArea.setPenColor(newColor);
    }
}

void SubMenu::changePenWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width : "),
                                        _scribbleArea.penWidth(),
                                        1, 50, 1, &ok);
    if(ok) {
        _scribbleArea.setPenWidth(newWidth);
    }
}

void SubMenu::clearScreen() {
    _scribbleArea.clearImage();
}

void SubMenu::createRectangle() {
    // _scribbleArea.addRectangle();
    _scribbleArea.setDrawLineBool();
}
