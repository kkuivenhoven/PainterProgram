#include "gradientcolorinputdialog.h"


GradientColorInputDialog::GradientColorInputDialog(QWidget *parent) : QWidget(parent) {
    m_linearNumColors = 0;
    m_conicalNumColors = 0;
    m_radialNumColors = 0;
}


GradientColorInputDialog::~GradientColorInputDialog() {

}


void GradientColorInputDialog::showLinearGradientWidget(int x1, int y1, int x2, int y2, int numColors) {
    m_linear_x1 = x1;
    m_linear_y1 = y1;
    m_linear_x2 = x2;
    m_linear_y2 = y2;
    m_linearNumColors = numColors;

    _setUpLinearGradientWidget();
    m_linearWidget->show();
}


void GradientColorInputDialog::_setUpLinearGradientWidget() {
    m_linearWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < m_linearNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color #" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);
        QLabel *colorName = new QLabel(tr(firstColor.name().toUtf8()));

        QPushButton *colorButton = new QPushButton(firstColor.name());
        m_curGradientStrColorsMap.insert(tmpTitle, colorButton);
        connect(colorButton, &QPushButton::released, [=] {
            handleButton(tmpTitle, tmpNum, colorName);
        });

        QString qss = QString("background-color: %1").arg(firstColor.name());
        colorButton->setStyleSheet(qss);

        gridLayout->addWidget(colorLabel, i, 0, 1, 1);
        gridLayout->addWidget(colorButton, i, 1, 1, 2);
        gridLayout->addWidget(colorName, i, 3, 1, 1);
    }

    m_groupBox = new QGroupBox(tr("Gradient Direction Options:"));
    m_radioLinearOne = new QRadioButton("Left to Right (straight across)");
    m_radioLinearTwo = new QRadioButton("Top Left to Bottom Right (diagonal)");
    m_radioLinearThree = new QRadioButton("Bottom Left to Top Right (diagonal)");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_radioLinearOne);
    vbox->addWidget(m_radioLinearTwo);
    vbox->addWidget(m_radioLinearThree);
    m_groupBox->setLayout(vbox);
    gridLayout->addWidget(m_groupBox, m_linearNumColors, 0, 1, 3);

    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient shape?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(drawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, m_linearNumColors+1, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, m_linearNumColors+1, 3, 1, 1);

    QLabel *closeWidgetLabel = new QLabel("Are you done with this gradient?");
    QPushButton *closeWidgetBtn = new QPushButton("Yes!");
    connect(closeWidgetBtn, SIGNAL(clicked(bool)), this, SLOT(clearOutLinearColorMap()));
    gridLayout->addWidget(closeWidgetLabel, m_linearNumColors+2, 0, 1, 3);
    gridLayout->addWidget(closeWidgetBtn, m_linearNumColors+2, 3, 1, 1);

    m_linearWidget->setLayout(gridLayout);
}


void GradientColorInputDialog::handleButton(QString tmpTitle, int position, QLabel *colorName) {
    QPushButton *curPushButton = m_curGradientStrColorsMap.value(tmpTitle);
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(m_mapCurColorChoices.contains(curPushButton)) {
            m_mapCurColorChoices.remove(curPushButton);
            m_mapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            m_mapCurColorChoices.insert(curPushButton, tmpMap);
        }
        colorName->setText(color.name());
        update();
    }
}


void GradientColorInputDialog::drawTheGradientShape() {
    if(m_mapCurColorChoices.size() == m_linearNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = m_mapCurColorChoices.begin(); iter != m_mapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        int width = (m_linear_x2 - m_linear_x1);
        int height = (m_linear_y2 - m_linear_y1);
        QRect rectLinear(m_linear_x1, m_linear_y1, width, height);

        if(m_radioLinearOne->isChecked()) {
            m_curLinearGradient.setStart(rectLinear.bottomLeft());
            m_curLinearGradient.setFinalStop(rectLinear.bottomRight());
        }
        if(m_radioLinearTwo->isChecked()) {
            m_curLinearGradient.setStart(rectLinear.topLeft());
            m_curLinearGradient.setFinalStop(rectLinear.bottomRight());
        }
        if(m_radioLinearThree->isChecked()) {
            m_curLinearGradient.setStart(rectLinear.topRight());
            m_curLinearGradient.setFinalStop(rectLinear.bottomLeft());
        }
        float decimal = (1.0/(m_linearNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt - 1));
            QColor tmpColor = tmpIter.value();
            m_curLinearGradient.setColorAt(tmpDec, tmpColor);
        }

        emit linearGradientToolsSet();
    }
}


void GradientColorInputDialog::clearOutLinearColorMap() {
    QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
    for(iter = m_mapCurColorChoices.begin(); iter != m_mapCurColorChoices.end();) {
        m_mapCurColorChoices.erase(iter);
        iter++;
    }
    m_linearWidget->close();
}


QLinearGradient GradientColorInputDialog::getLinearGradientTools() {
    return m_curLinearGradient;
}


void GradientColorInputDialog::showConicalGradientWidget(int x1, int y1, int x2, int y2, int numColors) {
    m_conical_x1 = x1;
    m_conical_y1 = y1;
    m_conical_x2 = x2;
    m_conical_y2 = y2;
    m_conicalNumColors = numColors;

    _setUpConicalGradientWidget();
}


void GradientColorInputDialog::_setUpConicalGradientWidget() {
    m_conicalWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < m_conicalNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color #" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);
        QLabel *colorName = new QLabel(tr(firstColor.name().toUtf8()));

        QPushButton *colorButton = new QPushButton(firstColor.name());
        m_curConicalGradientStrColorsMap.insert(tmpTitle, colorButton);
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
    gridLayout->addWidget(angleLabel, m_conicalNumColors+1, 0, 1, 3);

    m_conicalSpinBox = new QSpinBox();
    m_conicalSpinBox->setRange(0, 360);
    connect(m_conicalSpinBox, SIGNAL(valueChanged(int)), m_conicalSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(m_conicalSpinBox, m_conicalNumColors+1, 3, 1, 1);

    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient shape?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(conicalDrawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, m_conicalNumColors+2, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, m_conicalNumColors+2, 3, 1, 1);

    QLabel *closeWidgetLabel = new QLabel("Are you done with this gradient?");
    QPushButton *closeWidgetBtn = new QPushButton("Yes!");
    connect(closeWidgetBtn, SIGNAL(clicked(bool)), this, SLOT(clearOutConicalColorMap()));
    gridLayout->addWidget(closeWidgetLabel, m_conicalNumColors+3, 0, 1, 3);
    gridLayout->addWidget(closeWidgetBtn, m_conicalNumColors+3, 3, 1, 1);

    m_conicalWidget->setLayout(gridLayout);
    m_conicalWidget->show();
}


void GradientColorInputDialog::handleConicalButton(QString tmpTitle, int position, QLabel *colorName) {
    QPushButton *curPushButton = m_curConicalGradientStrColorsMap.value(tmpTitle);
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(m_conicalMapCurColorChoices.contains(curPushButton)) {
            m_conicalMapCurColorChoices.remove(curPushButton);
            m_conicalMapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            m_conicalMapCurColorChoices.insert(curPushButton, tmpMap);
        }
        colorName->setText(color.name());
        update();
    }
}


void GradientColorInputDialog::conicalDrawTheGradientShape() {
    if(m_conicalMapCurColorChoices.size() == m_conicalNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = m_conicalMapCurColorChoices.begin(); iter != m_conicalMapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        int width = (m_conical_x2 - m_conical_x1);
        int height = (m_conical_y2 - m_conical_y1);
        QRect rectConical(m_conical_x1, m_conical_y1, width, height);

        m_curConicalGradient = new QConicalGradient();
        qreal angle = m_conicalSpinBox->value();
        m_curConicalGradient->setCenter(rectConical.center());
        m_curConicalGradient->setAngle(angle);
        float decimal = (1.0/(m_conicalNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt - 1));
            QColor tmpColor = tmpIter.value();
            m_curConicalGradient->setColorAt(tmpDec, tmpColor);
        }

        emit conicalGradientToolsSet();
    }
}


QConicalGradient* GradientColorInputDialog::getConicalGradientTools() {
    return m_curConicalGradient;
}


void GradientColorInputDialog::clearOutConicalColorMap() {
    QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
    for(iter = m_conicalMapCurColorChoices.begin(); iter != m_conicalMapCurColorChoices.end();) {
        m_conicalMapCurColorChoices.erase(iter);
        iter++;
    }
    m_conicalWidget->close();
    delete m_curConicalGradient;
}


void GradientColorInputDialog::showRadialGradientWidget(int x1, int y1, int x2, int y2,
                                                        int numColors, int widgetHeight, int widgetWidth) {
    m_radial_x1 = x1;
    m_radial_y1 = y1;
    m_radial_x2 = x2;
    m_radial_y2 = y2;
    m_widgetHeight = widgetHeight;
    m_widgetWidth = widgetWidth;

    m_radialNumColors = numColors;
    _setUpRadialGradientWidget();
}


void GradientColorInputDialog::_setUpRadialGradientWidget() {
    m_radialWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout();

    for(int i = 0; i < m_radialNumColors; i++) {
        int tmpNum = (i+1);
        QString tmpTitle = "Color #" + QString::number(tmpNum);
        QLabel *colorLabel = new QLabel(tr(tmpTitle.toStdString().c_str()));
        QColor firstColor = QColor(Qt::black);
        QLabel *colorName = new QLabel(tr(firstColor.name().toUtf8()));

        QPushButton *colorButton = new QPushButton(firstColor.name());
        m_curRadialGradientStrColorsMap.insert(tmpTitle, colorButton);
        connect(colorButton, &QPushButton::released, [=] {
            handleRadialButton(tmpTitle, tmpNum, colorName);
        });

        QString qss = QString("background-color: %1").arg(firstColor.name());
        colorButton->setStyleSheet(qss);

        gridLayout->addWidget(colorLabel, i, 0, 1, 1);
        gridLayout->addWidget(colorButton, i, 1, 1, 2);
        gridLayout->addWidget(colorName, i, 3, 1, 1);
    }

    m_radialGroupBox = new QGroupBox(tr("Spread Direction Options:"));
    m_radioRadialOne = new QRadioButton("Pad Spread");
    m_radioRadialTwo = new QRadioButton("Reflect Spread");
    m_radioRadialThree = new QRadioButton("Repeat Spread");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_radioRadialOne);
    vbox->addWidget(m_radioRadialTwo);
    vbox->addWidget(m_radioRadialThree);
    m_radialGroupBox->setLayout(vbox);
    gridLayout->addWidget(m_radialGroupBox, m_radialNumColors+1, 0, 1, 3);

    QLabel *radiusLabel = new QLabel("Radius for radial spread:");
    gridLayout->addWidget(radiusLabel, m_radialNumColors+2, 0, 1, 3);
    m_radialSpinBox = new QSpinBox();
    m_radialSpinBox->setRange(0, 360);
    connect(m_radialSpinBox, SIGNAL(valueChanged(int)), m_radialSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(m_radialSpinBox, m_radialNumColors+2, 3, 1, 1);

    QLabel *firstXYset = new QLabel("First set: (x, y): -- (" + QString::number(m_radial_x1) + ", " + QString::number(m_radial_y1) + "):");
    gridLayout->addWidget(firstXYset, m_radialNumColors+3, 0, 1, 3);

    QLabel *centerXLabel = new QLabel("X value for center of radial:");
    gridLayout->addWidget(centerXLabel, m_radialNumColors+4, 0, 1, 3);
    m_centerXSpinBox = new QSpinBox();
    m_centerXSpinBox->setRange(0, m_widgetWidth);
    connect(m_centerXSpinBox, SIGNAL(valueChanged(int)), m_centerXSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(m_centerXSpinBox, m_radialNumColors+4, 3, 1, 1);

    QLabel *secondXYset = new QLabel("Second set: (x, y): -- (" + QString::number(m_radial_x2) + ", " + QString::number(m_radial_y2) + "):");
    gridLayout->addWidget(secondXYset, m_radialNumColors+5, 0, 1, 3);

    QLabel *centerYLabel = new QLabel("Y value for center of radial:");
    gridLayout->addWidget(centerYLabel, m_radialNumColors+6, 0, 1, 3);
    m_centerYSpinBox = new QSpinBox();
    m_centerYSpinBox->setRange(0, m_widgetHeight);
    connect(m_centerYSpinBox, SIGNAL(valueChanged(int)), m_centerYSpinBox, SLOT(setValue(int)));
    gridLayout->addWidget(m_centerYSpinBox, m_radialNumColors+6, 3, 1, 1);

    QLabel *drawSquareLabel = new QLabel("Is the user ready to draw gradient?");
    QPushButton *userDrawSquare = new QPushButton("Yes!");
    connect(userDrawSquare, SIGNAL(clicked(bool)), this, SLOT(radialDrawTheGradientShape()));
    gridLayout->addWidget(drawSquareLabel, m_radialNumColors+7, 0, 1, 3);
    gridLayout->addWidget(userDrawSquare, m_radialNumColors+7 ,3, 1, 1);

    QLabel *closeWidgetLabel = new QLabel("Are you done with this gradient?");
    QPushButton *closeWidgetBtn = new QPushButton("Yes!");
    connect(closeWidgetBtn, SIGNAL(clicked(bool)), this, SLOT(clearOutRadialColorMap()));
    gridLayout->addWidget(closeWidgetLabel, m_radialNumColors+8, 0, 1, 3);
    gridLayout->addWidget(closeWidgetBtn, m_radialNumColors+8, 3, 1, 1);

    m_radialWidget->setLayout(gridLayout);
    m_radialWidget->show();
}


void GradientColorInputDialog::handleRadialButton(QString tmpTitle, int position, QLabel *colorName) {
    QPushButton *curPushButton = m_curRadialGradientStrColorsMap.value(tmpTitle);
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if(color.isValid()) {
        curPushButton->setStyleSheet("background-color:" + color.name() + ";");
        curPushButton->setText(color.name());
        QMap<int, QColor> tmpMap;
        tmpMap.insert(position, color);
        if(m_radialMapCurColorChoices.contains(curPushButton)) {
            m_radialMapCurColorChoices.remove(curPushButton);
            m_radialMapCurColorChoices.insert(curPushButton, tmpMap);
        } else {
            m_radialMapCurColorChoices.insert(curPushButton, tmpMap);
        }
        colorName->setText(color.name());
        update();
    }
}


void GradientColorInputDialog::radialDrawTheGradientShape() {
    if(m_radialMapCurColorChoices.size() == m_radialNumColors) {
        QMap<int, QColor> gradientColors;
        QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
        for(iter = m_radialMapCurColorChoices.begin(); iter != m_radialMapCurColorChoices.end(); iter++) {
            QMap<int, QColor> tmpMap = iter.value();
            int tmpPosition = tmpMap.firstKey();
            QColor tmpColor = tmpMap.value(tmpPosition);
            gradientColors.insert(tmpPosition, tmpColor);
        }

        int width = (m_radial_x2 - m_radial_x1);
        int height = (m_radial_y2 - m_radial_y1);
        QRect rectRadial(m_radial_x1, m_radial_y1, width, height);

        qreal radius = m_radialSpinBox->value();
        m_curRadialGradient = new QRadialGradient();
        QPointF set;
        set.setX(m_centerXSpinBox->value());
        set.setY(m_centerYSpinBox->value());
        m_curRadialGradient->setCenter(set);
        m_curRadialGradient->setCenterRadius(radius);

        if(m_radioRadialOne->isChecked()) {
            m_curRadialGradient->setSpread(QGradient::PadSpread);
        }
        if(m_radioRadialTwo->isChecked()) {
            m_curRadialGradient->setSpread(QGradient::ReflectSpread);
        }
        if(m_radioRadialThree->isChecked()) {
            m_curRadialGradient->setSpread(QGradient::RepeatSpread);
        }

        float decimal = (1.0/(m_radialNumColors-1));
        QMap<int, QColor>::iterator tmpIter;
        for(tmpIter = gradientColors.begin(); tmpIter != gradientColors.end(); tmpIter++) {
            int tmpInt = tmpIter.key();
            float tmpDec = (decimal * (tmpInt - 1));
            QColor tmpColor = tmpIter.value();
            m_curRadialGradient->setColorAt(tmpDec, tmpColor);
        }

        emit radialGradientToolsSet();
    }
}


void GradientColorInputDialog::clearOutRadialColorMap() {
    QMap<QPushButton *, QMap<int, QColor>>::iterator iter;
    for(iter = m_radialMapCurColorChoices.begin(); iter != m_radialMapCurColorChoices.end();) {
        m_radialMapCurColorChoices.erase(iter);
        iter++;
    }
    m_radialWidget->close();
    delete m_curRadialGradient;
}


QRadialGradient* GradientColorInputDialog::getRadialGradientTools() {
    return m_curRadialGradient;
}

