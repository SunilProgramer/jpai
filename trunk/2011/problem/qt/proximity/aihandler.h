#ifndef AIHANDLER_H
#define AIHANDLER_H

#include <QThread>
#include <QPoint>
#include "map.h"
#include "game.h"

class MapHandler : public Map
{
public:
    MapHandler();
    void Reset();
    void Step(int x, int y);
    QList<QPoint> changed;
public slots:
    void onLoad();
protected:
    int PlayerCount;
    int CurrentStep;
    int CurrentPlayer;
    void Check(int x, int y, int influ);
    QVector<Cell> defaultField;
};

class AIHandler : public QThread
{
public:
    AIHandler();
protected:
    void run();
};

#endif // AIHANDLER_H
