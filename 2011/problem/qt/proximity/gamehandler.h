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
    int MatchId();
    MapHandler *getMapHandler();
    AIHandler *getAIHandler();
    void ApplyMatch(MatchDialog *dialog);
    bool NotStarted();
    void SilentStep();
    int GetMatchId();
public slots:
    void Init();
    void StepFwd();
    void NextMatch();
    void PrevMatch();
    void Start(bool start);

protected:
    void LoadMatchFromDb();
    AIHandler Handler;
    int match_id;
    GameHandler();
    ~GameHandler();
    MapHandler m;
    static GameHandler *instance;
};

#endif // GAMEHANDLER_H
