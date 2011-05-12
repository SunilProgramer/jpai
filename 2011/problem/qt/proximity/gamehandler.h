#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include "aihandler.h"
#include "matchdialog.h"

class GameHandler : public QObject
{
    Q_OBJECT
public:
    struct PlayerClass
    {
        QString name;
        bool player;
    };

    static GameHandler *Instance();
    static void Cleanup();
    int CompetitionId();
    MapHandler *getMapHandler();
    void ApplyMatch(MatchDialog *dialog);
public slots:
    void StepFwd();
    void Start(bool start);

protected:
    AIHandler Handler;
    int match_id;
    GameHandler();
    ~GameHandler();
    MapHandler m;
    static GameHandler *instance;
};

#endif // GAMEHANDLER_H
