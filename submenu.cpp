#include "submenu.h"

SubMenu::SubMenu(ScribbleArea &scribbleArea, QWidget *parent) : _scribbleArea(scribbleArea), QWidget(parent) {
    QWidget *subMenu = new QWidget();

    // subMenu->setStyleSheet("QWidget { background-color:salmon; border-radius:20%; }");
    // subMenu->setStyleSheet("QWidget { background-color: #A9ADB4; } ");
                           // "QPushButton { background-color: #FFFFFF; box-shadow: none; border-radius: 2 px; border-color: #FFFFFF; }");

    QGridLayout *gridLayout = new QGridLayout();

    QPushButton *fillEaselBtn = new QPushButton();
    fillEaselBtn->setToolTip("Fill the canvas with current paint color");
    fillEaselBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/PaintBucket.png"));
    connect(fillEaselBtn, SIGNAL(clicked(bool)), this, SLOT(fillEasel()));

    QPushButton *penColorBtn = new QPushButton();
    penColorBtn->setToolTip("Change paint color");
    penColorBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/PaintBrush.png"));
    connect(penColorBtn, SIGNAL(clicked(bool)), this, SLOT(changePenColor()));

    QPushButton *penWidthBtn = new QPushButton();
    penWidthBtn->setToolTip("Change the width of the pen");
    penWidthBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/3366100-200.png"));
    connect(penWidthBtn, SIGNAL(clicked(bool)), this, SLOT(changePenWidth()));

    QPushButton *clearScreenBtn = new QPushButton();
    clearScreenBtn->setToolTip("Clear the canvas");
    clearScreenBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/clear-screen.jpg"));
    connect(clearScreenBtn, SIGNAL(clicked(bool)), this, SLOT(clearScreen()));

    QPushButton *createLineBtn = new QPushButton();
    createLineBtn->setToolTip("Draw a straight line");
    createLineBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/lineToolPic.png"));
    connect(createLineBtn, SIGNAL(clicked(bool)), this, SLOT(createLine()));

    QPushButton *createTextBlurbBtn = new QPushButton();
    createTextBlurbBtn->setToolTip("Draw text area");
    createTextBlurbBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/letterPhoto.png"));
    connect(createTextBlurbBtn, SIGNAL(clicked(bool)), this, SLOT(createTextBlurb()));

    QPushButton *paintBrush = new QPushButton();
    paintBrush->setToolTip("Paint brush tool");
    paintBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/paintBrushTwo.png"));
    connect(paintBrush, SIGNAL(clicked(bool)), this, SLOT(setUpPaintBrush()));

    QPushButton *squareBrush = new QPushButton();
    squareBrush->setToolTip("Draw a square or rectangle");
    squareBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/squareIcon.png"));
    connect(squareBrush, SIGNAL(clicked(bool)), this, SLOT(setUpSquare()));

    QPushButton *ellipseBrush = new QPushButton();
    ellipseBrush->setToolTip("Draw an ellipse");
    ellipseBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/circleIcon.png"));
    connect(ellipseBrush, SIGNAL(clicked(bool)), this, SLOT(setUpEllipse()));

    QPushButton *convexPolygonBrush = new QPushButton();
    convexPolygonBrush->setToolTip("Draw a convex polygon");
    convexPolygonBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/convexIcon.png"));
    connect(convexPolygonBrush, SIGNAL(clicked(bool)), this, SLOT(setUpConvexPolygon()));

    QPushButton *setUpSquircle = new QPushButton();
    setUpSquircle->setToolTip("Draw a square with a radial gradient");
    setUpSquircle->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/roundedSquareIcon.png"));
    connect(setUpSquircle, SIGNAL(clicked(bool)), this, SLOT(setUpHandDrawnSquircle()));

    /**
     * @brief gradientBrush
     * 		The below button was used primarily for building purposes. It still remains in the
     * code as the program has not been tested thoroughly for bugs and is needed as a solid reference
     * for a basic gradient tool.
     * QPushButton *gradientBrush = new QPushButton();
     * gradientBrush->setToolTip("Draw a square with rounded edges");
     * gradientBrush->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/roundedSquareIcon.png"));
     * connect(gradientBrush, SIGNAL(clicked(bool)), this, SLOT(setUpLinearGradient()));
     * gridLayout->addWidget(gradientBrush);
     */

    QPushButton *setUpLinearGradientPaints = new QPushButton();
    setUpLinearGradientPaints->setToolTip("Draw a square with a linear gradient");
    setUpLinearGradientPaints->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/gradientIcon.png.jpeg"));
    connect(setUpLinearGradientPaints, SIGNAL(clicked(bool)), this, SLOT(userChooseGradientPaintColors()));

    QPushButton *setUpConicalGradientPaints = new QPushButton();
    setUpConicalGradientPaints->setToolTip("Draw a square with a conical gradient");
    setUpConicalGradientPaints->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/conicalGradient.png"));
    connect(setUpConicalGradientPaints, SIGNAL(clicked(bool)), this, SLOT(userChooseGradientPaintColors()));

    QPushButton *setUpRadialGradientPaints = new QPushButton();
    setUpRadialGradientPaints->setToolTip("Draw a square with a radial gradient");
    setUpRadialGradientPaints->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/radialIcon.png"));
    connect(setUpRadialGradientPaints, SIGNAL(clicked(bool)), this, SLOT(userChooseGradientPaintColors()));

    QPushButton *setUpUndoBtn = new QPushButton();
    setUpUndoBtn->setToolTip("Undo last tool action");
    setUpUndoBtn->setIcon(QIcon("/Users/kendrakuivenhoven/PainterProgram/toolIconImages/undoIcon.png"));
    connect(setUpUndoBtn, SIGNAL(clicked(bool)), this, SLOT(setUpUndoBtn()));

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
    gridLayout->addWidget(setUpSquircle);
    gridLayout->addWidget(setUpLinearGradientPaints);
    gridLayout->addWidget(setUpConicalGradientPaints);
    gridLayout->addWidget(setUpRadialGradientPaints);
    gridLayout->addWidget(setUpUndoBtn);

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
    int newWidth = QInputDialog::getInt(this, tr("Changing Pen Width"),
                                        tr("Select pen width : "),
                                        _scribbleArea.penWidth(),
                                        1, 100, 1, &okay);
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
    bool okay;
    QFont font = QFontDialog::getFont(
                &okay, QFont("Helvetica [Cronyx]", 12), this);
    if(okay) {
        _scribbleArea.setTextBlurb(font);
    } else {
        _scribbleArea.setTextBlurb(QFont("Helvetica [Cronyx]", 12));
    }
    // textEdit->setFont(font);
    // _scribbleArea.setTextBlurb(font);
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

void SubMenu::setUpHandDrawnSquircle() {
    _scribbleArea.setUpDrawSquircle();
}


void SubMenu::userChooseGradientPaintColors() {
    bool okay;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Number of colors for gradient:"),
                                         QLineEdit::Normal,
                                         "0", &okay);
    int numColors;
    if(okay && !text.isEmpty()) {
        numColors = text.toInt();
    }
    QPushButton *btnSender = qobject_cast<QPushButton*>(sender());
    QString btnToolTip = btnSender->toolTip();
    if(btnToolTip == "Draw a square with a linear gradient") {
        qDebug() << " ready to draw linear gradient";
        _scribbleArea.setUpLinearGradientPaints(numColors);
    }
    if(btnToolTip == "Draw a square with a conical gradient") {
        qDebug() << " ready to draw conical gradient";
        _scribbleArea.setUpConicalGradientPaints(numColors);
    }
    if(btnToolTip == "Draw a square with a radial gradient") {
        qDebug() << " ready to draw radial gradient";
        _scribbleArea.setUpRadialGradientPaints(numColors);
    }
}


void SubMenu::setUpUndoBtn() {
    _scribbleArea.setUpUndoFunctionality();
}
