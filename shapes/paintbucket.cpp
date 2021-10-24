#include "paintbucket.h"

PaintBucket::PaintBucket() {

}

void PaintBucket::setPenColor(QColor penColor) {
    m_penColor = penColor;
}

QColor PaintBucket::getPenColor() {
    return m_penColor;
}

void PaintBucket::setPosInOrderOfActions(int posOfLastAction) {
    m_posInOrderOfActions = posOfLastAction;
}

