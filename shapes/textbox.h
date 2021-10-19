#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QFont>
#include <QString>

class TextBox {
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    QString m_writtenText;
    QFont m_fontType;
    int m_fontSize;
    int m_posInOrderOfActions;

public:
    TextBox();
    void setCoords(int x1, int x2, int y1, int y2);
    // void setFontType(QFont fontType);
    // void setFontSize(int fontSize);
    void setWrittenText(QString writtenText);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();

    QString getTextWritten();
    QFont getFontType();
    int getFontSize();
};

#endif // TEXTBOX_H
