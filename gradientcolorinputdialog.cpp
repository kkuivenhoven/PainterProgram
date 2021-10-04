#include "gradientcolorinputdialog.h"

GradientColorInputDialog::GradientColorInputDialog(QWidget *parent) : QWidget(parent) {
    _setUpRadialGradientWidget();
    _linearNumColors = 0;
    _conicalNumColors = 0;
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


void GradientColorInputDialog::_setUpRadialGradientWidget() {
    _radialWidget = new QWidget();
}

