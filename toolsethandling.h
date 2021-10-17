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

class ToolSetHandling {
    QStack<QString> _orderOfActions;

    QQueue<Rectangle> _rectangleQueue;
    QQueue<Ellipse> _ellipseQueue;
    QQueue<Squircle> _squircleQueue;
    QQueue<FreeHandLine> _freeHandLineQueue;
    QQueue<ConvexPolygon> _convexPolygonQueue;
    QQueue<StraightLine> _straightLineQueue;

    QMap<int /*positionInActions*/,
         int /*posInShapeStack*/> _posMap;

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

};

#endif // TOOLSETHANDLING_H
