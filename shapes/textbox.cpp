#include "textbox.h"

TextBox::TextBox() {

}

void TextBox::setCoords(int x1, int x2, int y1, int y2) {
    m_x1 = x1;
    m_x2 = x2;
    m_y1 = y1;
    m_y2 = y2;
}

void TextBox::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

void TextBox::setWrittenText(QString writtenText) {
    m_writtenText = writtenText;
}

void TextBox::setFont(QFont font) {
    m_font = font;
}

int TextBox::getX1() {
    return m_x1;
}

int TextBox::getX2() {
    return m_x2;
}

int TextBox::getY1() {
    return m_y1;
}

int TextBox::getY2() {
    return m_y2;
}

QString TextBox::getTextWritten() {
    return m_writtenText;
}

QFont TextBox::getFont() {
    return m_font;
}
