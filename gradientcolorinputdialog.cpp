#include "gradientcolorinputdialog.h"


GradientColorInputDialog::GradientColorInputDialog(QWidget *parent) : QWidget(parent) {
    _linearNumColors = 0;
    _conicalNumColors = 0;
    _radialNumColors = 0;
}


GradientColorInputDialog::~GradientColorInputDialog() {

}


void GradientColorInputDialog::showLinearGradientWidget(int x1, int y1, int x2, int y2, int numColors) {
    _linear_x1 = x1;
    _linear_y1 = y1;
    _linear_x2 = x2;
    _linear_y2 = y2;
    _linearNumColors = numColors;

    _setUpLinearGradientWidget();
    _linearWidget->show();
}


void GradientColorInputDialog::_setUpLinearGradientWidget() {
    _linearWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < _linearNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color #" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);
        QLabel *colorName = new QLabel(tr(firstColor.name().toUtf8()));

        QPushButton *colorButton = new QPushButton(firstColor.name());
        curGradientStrColorsMap.insert(tmpTitle, colorButton);
        connect(colorButton, &QPushButton::released, [=] {
            handleButton(tmpTitle, tmpNum, colorName);
        });

        QString qss = QString("background-color: %1").arg(firstColor.name());
        colorButton->setStyleSheet(qss);

        gridLayout->addWidget(colorLabel, i, 0, 1, 1);
        gridLayout->addWidget(colorButton, i, 1, 1, 2);
        gridLayout->addWidget(colorName, i, 3, 1, 1);
    }

    groupBox = new QGroupBox(tr("Gradient Direction Options:"));
    _radioLinearOne = new QRadioButton("Left to Right (straight across)");
    _radioLinearTwo = new QRadioButton("Top Left to Bottom Right (diagonal)");
    _radioLinearThree = new QRadioButton("Bottom Left to Top Right (diagonal)");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(_radioLinearOne);
    vbox->addWidget(_radioLinearTwo);
    vbox->addWidget(_radioLinearThree);
    groupBox->setLayout(vbox);
    gridLayout->addWidget(groupBox, _linearNumColors, 0, 1, 3);

    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient shape?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(drawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, _linearNumColors+1, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, _linearNumColors+1, 3, 1, 1);

    QLabel *closeWidgetLabel = new QLabel("Are you done with this gradient?");
    QPushButton *closeWidgetBtn = new QPushButton("Yes!");
    connect(closeWidgetBtn, SIGNAL(clicked(bool)), this, SLOT(clearOutLinearColorMap()));
    gridLayout->addWidget(closeWidgetLabel, _linearNumColors+2, 0, 1, 3);
    gridLayout->addWidget(closeWidgetBtn, _linearNumColors+2, 3, 1, 1);

    _linearWidget->setLayout(gridLayout);
}


void GradientColorInputDialog::handleButton(QString tmpTitle, int position, QLabel *colorName) {
    QPushButton *curPushButton = curGradientStrColorsMap.value(tmpTitle);
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(mapCurColorChoices.contains(curPushButton)) {
            mapCurColorChoices.remove(curPushButton);
            mapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            mapCurColorChoices.insert(curPushButton, tmpMap);
        }
        colorName->setText(color.name());
        update();
    }
}


void GradientColorInputDialog::drawTheGradientShape() {
    if(mapCurColorChoices.size() == _linearNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = mapCurColorChoices.begin(); iter != mapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        int width = (_linear_x2 - _linear_x1);
        int height = (_linear_y2 - _linear_y1);
        QRect rectLinear(_linear_x1, _linear_y1, width, height);

        if(_radioLinearOne->isChecked()) {
            _curLinearGradient.setStart(rectLinear.bottomLeft());
            _curLinearGradient.setFinalStop(rectLinear.bottomRight());
        }
        if(_radioLinearTwo->isChecked()) {
            _curLinearGradient.setStart(rectLinear.topLeft());
            _curLinearGradient.setFinalStop(rectLinear.bottomRight());
        }
        if(_radioLinearThree->isChecked()) {
            _curLinearGradient.setStart(rectLinear.topRight());
            _curLinearGradient.setFinalStop(rectLinear.bottomLeft());
        }
        float decimal = (1.0/(_linearNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt - 1));
            QColor tmpColor = tmpIter.value();
            _curLinearGradient.setColorAt(tmpDec, tmpColor);
        }

        emit linearGradientToolsSet();
    }
}


void GradientColorInputDialog::clearOutLinearColorMap() {
    QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
    for(iter = mapCurColorChoices.begin(); iter != mapCurColorChoices.end();) {
        mapCurColorChoices.erase(iter);
        iter++;
    }
    _linearWidget->close();
}


QLinearGradient GradientColorInputDialog::getLinearGradientTools() {
    return _curLinearGradient;
}


void GradientColorInputDialog::showConicalGradientWidget(int x1, int y1, int x2, int y2, int numColors) {
    _conical_x1 = x1;
    _conical_y1 = y1;
    _conical_x2 = x2;
    _conical_y2 = y2;
    _conicalNumColors = numColors;

    _setUpConicalGradientWidget();
}


void GradientColorInputDialog::_setUpConicalGradientWidget() {
    _conicalWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < _conicalNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color #" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);
        QLabel *colorName = new QLabel(tr(firstColor.name().toUtf8()));

        QPushButton *colorButton = new QPushButton(firstColor.name());
        curConicalGradientStrColorsMap.insert(tmpTitle, colorButton);
        connect(colorButton, &QPushButton::released, [=] {
            handleConicalButton(tmpTitle, tmpNum, colorName);
        });

        QString qss = QString("background-color: %1").arg(firstColor.name());
        colorButton->setStyleSheet(qss);

        gridLayout->addWidget(colorLabel, i, 0, 1, 1);
        gridLayout->addWidget(colorButton, i, 1, 1, 2);
        gridLayout->addWidget(colorName, i, 3, 1, 1);
    }

    QLabel *angleLabel = new QLabel("Angle at which to draw the conical gradient:");
    gridLayout->addWidget(angleLabel, _conicalNumColors+1, 0, 1, 3);

    _conicalSpinBox = new QSpinBox();
    _conicalSpinBox->setRange(0, 360);
    connect(_conicalSpinBox, SIGNAL(valueChanged(int)), _conicalSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(_conicalSpinBox, _conicalNumColors+1, 3, 1, 1);

    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient shape?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(conicalDrawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, _conicalNumColors+2, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, _conicalNumColors+2, 3, 1, 1);

    QLabel *closeWidgetLabel = new QLabel("Are you done with this gradient?");
    QPushButton *closeWidgetBtn = new QPushButton("Yes!");
    connect(closeWidgetBtn, SIGNAL(clicked(bool)), this, SLOT(clearOutConicalColorMap()));
    gridLayout->addWidget(closeWidgetLabel, _conicalNumColors+3, 0, 1, 3);
    gridLayout->addWidget(closeWidgetBtn, _conicalNumColors+3, 3, 1, 1);

    _conicalWidget->setLayout(gridLayout);
    _conicalWidget->show();
}


void GradientColorInputDialog::handleConicalButton(QString tmpTitle, int position, QLabel *colorName) {
    QPushButton *curPushButton = curConicalGradientStrColorsMap.value(tmpTitle);
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(conicalMapCurColorChoices.contains(curPushButton)) {
            conicalMapCurColorChoices.remove(curPushButton);
            conicalMapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            conicalMapCurColorChoices.insert(curPushButton, tmpMap);
        }
        colorName->setText(color.name());
        update();
    }
}


void GradientColorInputDialog::conicalDrawTheGradientShape() {
    if(conicalMapCurColorChoices.size() == _conicalNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = conicalMapCurColorChoices.begin(); iter != conicalMapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        int width = (_conical_x2 - _conical_x1);
        int height = (_conical_y2 - _conical_y1);
        QRect rectConical(_conical_x1, _conical_y1, width, height);

        _curConicalGradient = new QConicalGradient();
        qreal angle = _conicalSpinBox->value();
        _curConicalGradient->setCenter(rectConical.center());
        _curConicalGradient->setAngle(angle);
        float decimal = (1.0/(_conicalNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt - 1));
            QColor tmpColor = tmpIter.value();
            _curConicalGradient->setColorAt(tmpDec, tmpColor);
        }

        emit conicalGradientToolsSet();
    }
}


QConicalGradient* GradientColorInputDialog::getConicalGradientTools() {
    return _curConicalGradient;
}


void GradientColorInputDialog::clearOutConicalColorMap() {
    QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
    for(iter = conicalMapCurColorChoices.begin(); iter != conicalMapCurColorChoices.end();) {
        conicalMapCurColorChoices.erase(iter);
        iter++;
    }
    _conicalWidget->close();
    delete _curConicalGradient;
}


void GradientColorInputDialog::showRadialGradientWidget(int x1, int y1, int x2, int y2,
                                                        int numColors, int widgetHeight, int widgetWidth) {
    _radial_x1 = x1;
    _radial_y1 = y1;
    _radial_x2 = x2;
    _radial_y2 = y2;
    _widgetHeight = widgetHeight;
    _widgetWidth = widgetWidth;

    _radialNumColors = numColors;
    _setUpRadialGradientWidget();
}


void GradientColorInputDialog::_setUpRadialGradientWidget() {
    _radialWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < _radialNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color #" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);
        QLabel *colorName = new QLabel(tr(firstColor.name().toUtf8()));

        QPushButton *colorButton = new QPushButton(firstColor.name());
        curRadialGradientStrColorsMap.insert(tmpTitle, colorButton);
        connect(colorButton, &QPushButton::released, [=] {
            handleRadialButton(tmpTitle, tmpNum, colorName);
        });

        QString qss = QString("background-color: %1").arg(firstColor.name());
        colorButton->setStyleSheet(qss);

        gridLayout->addWidget(colorLabel, i, 0, 1, 1);
        gridLayout->addWidget(colorButton, i, 1, 1, 2);
        gridLayout->addWidget(colorName, i, 3, 1, 1);
    }

    _radialGroupBox = new QGroupBox(tr("Spread Direction Options:"));
    _radioRadialOne = new QRadioButton("Pad Spread");
    _radioRadialTwo = new QRadioButton("Reflect Spread");
    _radioRadialThree = new QRadioButton("Repeat Spread");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(_radioRadialOne);
    vbox->addWidget(_radioRadialTwo);
    vbox->addWidget(_radioRadialThree);
    _radialGroupBox->setLayout(vbox);
    gridLayout->addWidget(_radialGroupBox, _radialNumColors+1, 0, 1, 3);

    QLabel *radiusLabel = new QLabel("Radius for radial spread:");
    gridLayout->addWidget(radiusLabel, _radialNumColors+2, 0, 1, 3);
    _radialSpinBox = new QSpinBox();
    _radialSpinBox->setRange(0, 360);
    connect(_radialSpinBox, SIGNAL(valueChanged(int)), _radialSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(_radialSpinBox, _radialNumColors+2, 3, 1, 1);

    QLabel *firstXYset = new QLabel("First set: (x, y): -- (" + QString::number(_radial_x1) + ", " + QString::number(_radial_y1) + "):");
    gridLayout->addWidget(firstXYset, _radialNumColors+3, 0, 1, 3);

    QLabel *centerXLabel = new QLabel("X value for center of radial:");
    gridLayout->addWidget(centerXLabel, _radialNumColors+4, 0, 1, 3);
    _centerXSpinBox = new QSpinBox();
    _centerXSpinBox->setRange(0, _widgetWidth);
    connect(_centerXSpinBox, SIGNAL(valueChanged(int)), _centerXSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(_centerXSpinBox, _radialNumColors+4, 3, 1, 1);

    QLabel *secondXYset = new QLabel("Second set: (x, y): -- (" + QString::number(_radial_x2) + ", " + QString::number(_radial_y2) + "):");
    gridLayout->addWidget(secondXYset, _radialNumColors+5, 0, 1, 3);

    QLabel *centerYLabel = new QLabel("Y value for center of radial:");
    gridLayout->addWidget(centerYLabel, _radialNumColors+6, 0, 1, 3);
    _centerYSpinBox = new QSpinBox();
    _centerYSpinBox->setRange(0, _widgetHeight);
    connect(_centerYSpinBox, SIGNAL(valueChanged(int)), _centerYSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(_centerYSpinBox, _radialNumColors+6, 3, 1, 1);

    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(radialDrawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, _radialNumColors+7, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, _radialNumColors+7 ,3, 1, 1);

    QLabel *closeWidgetLabel = new QLabel("Are you done with this gradient?");
    QPushButton *closeWidgetBtn = new QPushButton("Yes!");
    connect(closeWidgetBtn, SIGNAL(clicked(bool)), this, SLOT(clearOutRadialColorMap()));
    gridLayout->addWidget(closeWidgetLabel, _radialNumColors+8, 0, 1, 3);
    gridLayout->addWidget(closeWidgetBtn, _radialNumColors+8, 3, 1, 1);

    _radialWidget->setLayout(gridLayout);
    _radialWidget->show();
}


void GradientColorInputDialog::handleRadialButton(QString tmpTitle, int position, QLabel *colorName) {
    QPushButton *curPushButton = curRadialGradientStrColorsMap.value(tmpTitle);
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(radialMapCurColorChoices.contains(curPushButton)) {
            radialMapCurColorChoices.remove(curPushButton);
            radialMapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            radialMapCurColorChoices.insert(curPushButton, tmpMap);
        }
        colorName->setText(color.name());
        update();
    }
}


void GradientColorInputDialog::radialDrawTheGradientShape() {
    if(radialMapCurColorChoices.size() == _radialNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = radialMapCurColorChoices.begin(); iter != radialMapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        int width = (_radial_x2 - _radial_x1);
        int height = (_radial_y2 - _radial_y1);
        QRect rectRadial(_radial_x1, _radial_y1, width, height);

        qreal radius = _radialSpinBox->value();
        _curRadialGradient = new QRadialGradient();
        QPointF set;
        set.setX(_centerXSpinBox->value());
        set.setY(_centerYSpinBox->value());
        _curRadialGradient->setCenter(set);
        _curRadialGradient->setCenterRadius(radius);

        if(_radioRadialOne->isChecked()) {
            _curRadialGradient->setSpread(QGradient::PadSpread);
        }
        if(_radioRadialTwo->isChecked()) {
            _curRadialGradient->setSpread(QGradient::ReflectSpread);
        }
        if(_radioRadialThree->isChecked()) {
            _curRadialGradient->setSpread(QGradient::RepeatSpread);
        }

        float decimal = (1.0/(_radialNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt - 1));
            QColor tmpColor = tmpIter.value();
            _curRadialGradient->setColorAt(tmpDec, tmpColor);
        }

        emit radialGradientToolsSet();
    }
}


void GradientColorInputDialog::clearOutRadialColorMap() {
    QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
    for(iter = radialMapCurColorChoices.begin(); iter != radialMapCurColorChoices.end();) {
        radialMapCurColorChoices.erase(iter);
        iter++;
    }
    _radialWidget->close();
    delete _curRadialGradient;
}


QRadialGradient* GradientColorInputDialog::getRadialGradientTools() {
    return _curRadialGradient;
}

