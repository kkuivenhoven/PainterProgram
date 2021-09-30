#include "gradientcolorinputdialog.h"

GradientColorInputDialog::GradientColorInputDialog(QWidget *parent) : QWidget(parent) {
    _setUpLinearGradientWidget();
    _setUpConicalGradientWidget();
    _setUpRadialGradientWidget();
}


GradientColorInputDialog::~GradientColorInputDialog() {

}


void GradientColorInputDialog::showLinearGradientWidget(int x1, int y1, int x2, int y2) {
    _linear_x1 = x1;
    _linear_y1 = y1;
    _linear_x2 = x2;
    _linear_y2 = y2;

    _linearWidget->show();
}


void GradientColorInputDialog::_setUpLinearGradientWidget() {
    _linearWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < 3; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color" + QString::number(tmpNum);
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
    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient shape?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(drawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, 3, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, 3, 3, 1, 1);

    groupBox = new QGroupBox(tr("Gradient Direction Options:"));
    groupBox->setCheckable(true);
    groupBox->setChecked(true);
    _radioOne = new QCheckBox("Left to Right (straight across)");
    _radioTwo = new QCheckBox("Top Left to Bottom Right (diagonal)");
    _radioThree = new QCheckBox("Bottom Left to Top Right (diagonal)");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(_radioOne);
    vbox->addWidget(_radioTwo);
    vbox->addWidget(_radioThree);
    groupBox->setLayout(vbox);
    gridLayout->addWidget(groupBox, 3+1, 0, 1, 3);

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
        if(mapCurColorChoices.remove(curPushButton)) {
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
    if(mapCurColorChoices.size() == 3) {
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

        if(_radioOne->checkState() == Qt::Checked) {
            _curLinearGradient.setStart(rectLinear.bottomLeft());
            _curLinearGradient.setFinalStop(rectLinear.bottomRight());
        }
        if(_radioTwo->checkState() == Qt::Checked) {
            _curLinearGradient.setStart(rectLinear.topLeft());
            _curLinearGradient.setFinalStop(rectLinear.bottomRight());
        }
        if(_radioThree->checkState() == Qt::Checked) {
            _curLinearGradient.setStart(rectLinear.topRight());
            _curLinearGradient.setFinalStop(rectLinear.bottomLeft());
        }
        float decimal = (1.0/(3-1));
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


QLinearGradient GradientColorInputDialog::getLinearGradientTools() {
    return _curLinearGradient;
}


void GradientColorInputDialog::_setUpConicalGradientWidget() {
    _conicalWidget = new QWidget();
}


void GradientColorInputDialog::_setUpRadialGradientWidget() {
    _radialWidget = new QWidget();
}

