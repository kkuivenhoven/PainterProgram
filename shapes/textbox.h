#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QFont>
#include <QString>
#include <QColor>

class TextBox {
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    QFont m_font;
    QString m_writtenText;
    int m_posInOrderOfActions;

public:
    TextBox();
    void setCoords(int x1, int x2, int y1, int y2);
    void setFont(QFont font);
    void setWrittenText(QString writtenText);
    void setPosInOrderOfActions(int posOfLastAction);

    int getX1();
    int getX2();
    int getY1();
    int getY2();

    QFont getFont();
    QString getTextWritten();
};

#endif // TEXTBOX_H
