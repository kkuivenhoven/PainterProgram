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

    QPushButton *createLineBtn = new QPushButton();
    createLineBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/lineToolPic.png"));
    connect(createLineBtn, SIGNAL(clicked(bool)), this, SLOT(createLine()));

    QPushButton *createTextBlurbBtnThree = new QPushButton();
    createTextBlurbBtnThree->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/letterPhoto.png"));
    connect(createTextBlurbBtnThree, SIGNAL(clicked(bool)), this, SLOT(createTextThreeBlurb()));

    QPushButton *paintBrush = new QPushButton();
    paintBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/paintBrushTwo.png"));
    connect(paintBrush, SIGNAL(clicked(bool)), this, SLOT(setUpPaintBrush()));

    QPushButton *squareBrush = new QPushButton();
    squareBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/squareIcon.png"));
    connect(squareBrush, SIGNAL(clicked(bool)), this, SLOT(setUpSquare()));

    QPushButton *ellipseBrush = new QPushButton();
    ellipseBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/circleIcon.png"));
    connect(ellipseBrush, SIGNAL(clicked(bool)), this, SLOT(setUpEllipse()));

    gridLayout->addWidget(fillEaselBtn);
    gridLayout->addWidget(penColorBtn);
    gridLayout->addWidget(penWidthBtn);
    gridLayout->addWidget(clearScreenBtn);
    gridLayout->addWidget(createLineBtn);
    gridLayout->addWidget(createTextBlurbBtnThree);
    gridLayout->addWidget(paintBrush);
    gridLayout->addWidget(squareBrush);
    gridLayout->addWidget(ellipseBrush);

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
    bool okay;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width : "),
                                        _scribbleArea.penWidth(),
                                        1, 50, 1, &okay);
    if(okay) {
        _scribbleArea.setPenWidth(newWidth);
    }
}

void SubMenu::clearScreen() {
    _scribbleArea.clearImage();
}

void SubMenu::createLine() {
    _scribbleArea.setDrawLineBool();
}

void SubMenu::createTextThreeBlurb() {
    _scribbleArea.setThirdTextBlurb();
}

void SubMenu::setUpPaintBrush() {
    _scribbleArea.setPenUp();
}

void SubMenu::setUpSquare() {
    _scribbleArea.setUpSquare();
}

void SubMenu::setUpEllipse() {
    _scribbleArea.setUpEllipse();
}
