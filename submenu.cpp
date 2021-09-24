#include "submenu.h"

SubMenu::SubMenu(ScribbleArea &scribbleArea, QWidget *parent) : _scribbleArea(scribbleArea), QWidget(parent) {
    QWidget *subMenu = new QWidget();

    // subMenu->setStyleSheet("QWidget { background-color:salmon; border-radius:20%; }");
    // subMenu->setStyleSheet("QWidget { background-color: #A9ADB4; } ");
                           // "QPushButton { background-color: #FFFFFF; box-shadow: none; border-radius: 2 px; border-color: #FFFFFF; }");

    QGridLayout *gridLayout = new QGridLayout();

    QPushButton *fillEaselBtn = new QPushButton();
    fillEaselBtn->setToolTip("Fill the canvas with current paint color");
    fillEaselBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/PaintBucket.png"));
    connect(fillEaselBtn, SIGNAL(clicked(bool)), this, SLOT(fillEasel()));

    QPushButton *penColorBtn = new QPushButton();
    penColorBtn->setToolTip("Change paint color");
    penColorBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/PaintBrush.png"));
    connect(penColorBtn, SIGNAL(clicked(bool)), this, SLOT(changePenColor()));

    QPushButton *penWidthBtn = new QPushButton();
    penWidthBtn->setToolTip("Change the width of the pen");
    penWidthBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/3366100-200.png"));
    connect(penWidthBtn, SIGNAL(clicked(bool)), this, SLOT(changePenWidth()));

    QPushButton *clearScreenBtn = new QPushButton();
    clearScreenBtn->setToolTip("Clear the canvas");
    clearScreenBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/clear-screen.jpg"));
    connect(clearScreenBtn, SIGNAL(clicked(bool)), this, SLOT(clearScreen()));

    QPushButton *createLineBtn = new QPushButton();
    createLineBtn->setToolTip("Draw a straight line");
    createLineBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/lineToolPic.png"));
    connect(createLineBtn, SIGNAL(clicked(bool)), this, SLOT(createLine()));

    QPushButton *createTextBlurbBtn = new QPushButton();
    createTextBlurbBtn->setToolTip("Draw text area");
    createTextBlurbBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/letterPhoto.png"));
    connect(createTextBlurbBtn, SIGNAL(clicked(bool)), this, SLOT(createTextBlurb()));

    QPushButton *paintBrush = new QPushButton();
    paintBrush->setToolTip("Paint brush tool");
    paintBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/paintBrushTwo.png"));
    connect(paintBrush, SIGNAL(clicked(bool)), this, SLOT(setUpPaintBrush()));

    QPushButton *squareBrush = new QPushButton();
    squareBrush->setToolTip("Draw a square or rectangle");
    squareBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/squareIcon.png"));
    connect(squareBrush, SIGNAL(clicked(bool)), this, SLOT(setUpSquare()));

    QPushButton *ellipseBrush = new QPushButton();
    ellipseBrush->setToolTip("Draw an ellipse");
    ellipseBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/circleIcon.png"));
    connect(ellipseBrush, SIGNAL(clicked(bool)), this, SLOT(setUpEllipse()));

    QPushButton *convexPolygonBrush = new QPushButton();
    convexPolygonBrush->setToolTip("Draw a convex polygon");
    convexPolygonBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/convexIcon.png"));
    connect(convexPolygonBrush, SIGNAL(clicked(bool)), this, SLOT(setUpConvexPolygon()));

    QPushButton *roundSquareBrush = new QPushButton();
    roundSquareBrush->setToolTip("Draw a square with rounded edges");
    roundSquareBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/roundedSquareIcon.png"));
    connect(roundSquareBrush, SIGNAL(clicked(bool)), this, SLOT(setUpRoundEdgeSquare()));

    QPushButton *gradientBrush = new QPushButton();
    gradientBrush->setToolTip("Draw a square with rounded edges");
    gradientBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/roundedSquareIcon.png"));
    connect(gradientBrush, SIGNAL(clicked(bool)), this, SLOT(setUpLinearGradient()));

    QPushButton *setUpGradientPaints = new QPushButton();
    setUpGradientPaints->setToolTip("Draw a square with rounded edges");
    setUpGradientPaints->setIcon(QIcon("/Users/kendrakuivenhoven/PaintProgram/roundedSquareIcon.png"));
    connect(setUpGradientPaints, SIGNAL(clicked(bool)), this, SLOT(userChooseGradientPaintColors()));

    gridLayout->addWidget(clearScreenBtn);
    gridLayout->addWidget(penColorBtn);
    gridLayout->addWidget(penWidthBtn);
    gridLayout->addWidget(fillEaselBtn);
    gridLayout->addWidget(createLineBtn);
    gridLayout->addWidget(createTextBlurbBtn);
    gridLayout->addWidget(paintBrush);
    gridLayout->addWidget(squareBrush);
    gridLayout->addWidget(ellipseBrush);
    gridLayout->addWidget(convexPolygonBrush);
    gridLayout->addWidget(roundSquareBrush);
    gridLayout->addWidget(gradientBrush);
    gridLayout->addWidget(setUpGradientPaints);

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

void SubMenu::createTextBlurb() {
    _scribbleArea.setTextBlurb();
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

void SubMenu::setUpConvexPolygon() {
    _scribbleArea.setReadyToDrawConvexPolygonBool();
}

void SubMenu::setUpRoundEdgeSquare() {
    _scribbleArea.setUpRoundSquare();
}

void SubMenu::setUpLinearGradient() {
    qDebug() << "SubMenu::setUpGradient()";
    _scribbleArea.setUpLinearGradient();
}

void SubMenu::userChooseGradientPaintColors() {
    _scribbleArea.setUpGradientPaints();
}
