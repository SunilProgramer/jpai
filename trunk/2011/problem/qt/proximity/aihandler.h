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
    void Step(int x, int y);
    void Step(const QString &command);
    QList<QPoint> changed;
    void ClearPlayers();
    void AddPlayer(const QString &name, const int &id, bool Player);
    void Export(const QString &filename);
    QString GetPlayer(const int &index);
    bool GetPlayerIdentity(const int &index);
    int GetPlayerId(const int &index);
    int GetPlayerNum(const int &id);
    int PlayersCount();
    int FreeCells();
    int Stepped();
    void silentStep(int x, int y, int Player, int influence);
    void SetCurrentStep(int step);
    void CalculateScores();
signals:
    void Reseted();
public slots:
    void onLoad();
    void Reset();
protected:
    int freecells;
    int stepped;
    int defaultFreecells;
    QVector<QString> Players;
    QVector<int> PlayersId;
    QVector<bool> PlayersIdentitiy;
    QVector<int> scores;
    int CurrentStep;
    int CurrentPlayer;
    int Check(int x, int y, int influ);
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
    void StepFinished();
    void Finished();
protected:
    MapHandler *Handler;
    bool Stopped;
    QMutex mutex;
    void run();
    QString GetOutput(const QString &filename);
};

#endif // AIHANDLER_H
