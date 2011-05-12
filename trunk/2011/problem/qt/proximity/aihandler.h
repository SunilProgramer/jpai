#ifndef AIHANDLER_H
#define AIHANDLER_H

#include <QMutex>
#include <QThread>
#include <QPoint>
#include "map.h"
#include "game.h"

class MapHandler : public Map
{
    Q_OBJECT
public:
    MapHandler();
    void Reset();
    void Step(int x, int y);
    void Step(const QString &command);
    QList<QPoint> changed;
    void ClearPlayers();
    void AddPlayer(const QString &name, const int &id, bool Player);
    void Export(const QString &filename);
    QString GetPlayer(const int &index);
    bool GetPlayerIdentity(const int &index);
    int PlayersCount();
    int FreeCells();
public slots:
    void onLoad();
protected:
    int freecells;
    int defaultFreecells;
    QVector<QString> Players;
    QVector<int> PlayersId;
    QVector<bool> PlayersIdentitiy;
    int CurrentStep;
    int CurrentPlayer;
    void Check(int x, int y, int influ);
    QVector<Cell> defaultField;
};

class AIHandler : public QThread
{
    Q_OBJECT
public:
    AIHandler();
    void Start();
    void Stop();
    void SetMapHandler(MapHandler *handler);
signals:
    void Finished();
protected:
    MapHandler *Handler;
    bool Stopped;
    QMutex mutex;
    void run();
    QString GetOutput(const QString &filename);
};

#endif // AIHANDLER_H
