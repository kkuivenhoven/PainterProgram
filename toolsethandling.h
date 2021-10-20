#ifndef TOOLSETHANDLING_H
#define TOOLSETHANDLING_H

#include <QStack>
#include <QQueue>
#include <QMap>
#include <QDebug>

#include "shapes/rectangle.h"
#include "shapes/ellipse.h"
#include "shapes/squircle.h"
#include "shapes/freehandline.h"
#include "shapes/convexpolygon.h"
#include "shapes/straightline.h"
#include "shapes/lineargradientshape.h"
#include "shapes/conicalgradientshape.h"
#include "shapes/radialgradientshape.h"
#include "shapes/textbox.h"

class ToolSetHandling {
    QStack<QString> m_orderOfActions;

    QQueue<Rectangle> m_rectangleQueue;
    QQueue<Ellipse> m_ellipseQueue;
    QQueue<Squircle> m_squircleQueue;
    QQueue<FreeHandLine> m_freeHandLineQueue;
    QQueue<ConvexPolygon> m_convexPolygonQueue;
    QQueue<StraightLine> m_straightLineQueue;
    QQueue<LinearGradientShape> m_linearGradientShapeQueue;
    QQueue<ConicalGradientShape> m_conicalGradientShapeQueue;
    QQueue<RadialGradientShape> m_radialGradientShapeQueue;
    QQueue<TextBox> m_textBoxQueue;

    QMap<int /*positionInActions*/,
         int /*posInShapeStack*/> m_posMap;

public:
    ToolSetHandling();
    QStack<QString> verifyLastAction();
    void addLastActionToStack(QString objType);
    QStack<QString> getOrderOfObjectsDrawn();
    int getPositionOfLastActionAdded();
    void removeLastActionFromStack();

    void setActionPosAndShapePos(int actionPos, int shapePos);
    QMap<int, int> getPosMap();
    void removeLastPosStored(int keyValueToRemove);

    static const QString RECTANGLE;
    void addRectangleToQueue(Rectangle rectangle);
    void removeLastRectangle();
    QQueue<Rectangle> getQueueOfRectangles();
    void removeFrontRectFromQueue();

    static const QString ELLIPSE;
    void addEllipseToQueue(Ellipse ellipse);
    void removeLastEllipse();
    QQueue<Ellipse> getQueueOfEllipses();
    void removeFrontEllipseFromQueue();

    static const QString SQUIRCLE;
    void addSquircleToQueue(Squircle squircle);
    void removeLastSquircle();
    QQueue<Squircle> getQueueOfSquircles();
    void removeFrontSquircleFromQueue();

    static const QString FREE_HAND_LINE;
    void addFreeHandLineToQueue(FreeHandLine freeHandLine);
    void removeLastFreeHandLine();
    QQueue<FreeHandLine> getQueueOfFreeHandLines();
    void removeFrontFreeHandLineFromQueue();
    FreeHandLine& obtainCurFreeHandLineInstance();

    static const QString CONVEX_POLYGON;
    void addConvexPolygonToQueue(ConvexPolygon convexPolygon);
    void removeLastConvexPolygon();
    QQueue<ConvexPolygon> getQueueOfConvexPolygons();

    static const QString STRAIGHT_LINE;
    void addStraightLineToQueue(StraightLine straightLine);
    void removeLastStraightLine();
    QQueue<StraightLine> getQueueOfStraightLines();

    static const QString LINEAR_GRADIENT_SHAPE;
    void addLinearGradientShapeToQueue(LinearGradientShape linearGradientShape);
    void removeLastLinearGradientShape();
    QQueue<LinearGradientShape> getQueueOfLinearGradientShapes();
    void updateLinearGradient(QLinearGradient linearGradient);

    static const QString CONICAL_GRADIENT_SHAPE;
    void addConicalGradientShapeToQueue(ConicalGradientShape conicalGradientShape);
    void removeLastConicalGradientShape();
    QQueue<ConicalGradientShape> getQueueOfConicalGradientShapes();
    void updateConicalGradient(QConicalGradient conicalGradient);

    static const QString RADIAL_GRADIENT_SHAPE;
    void addRadialGradientShapeToQueue(RadialGradientShape radialGradientShape);
    void removeLastRadialGradientShape();
    QQueue<RadialGradientShape> getQueueOfRadialGradientShapes();
    void updateRadialGradient(QRadialGradient radialGradient);

    static const QString TEXT_BOX;
    void addTextBoxToQueue(TextBox textBox);
    void removeLastTextBox();
    QQueue<TextBox> getQueueOfTextBoxes();
    void updateTextBox(QString textWritten);
    void addCoords(int x1, int x2, int y1, int y2);
    void addFont(QFont font);
    QFont getFont();

};

#endif // TOOLSETHANDLING_H
