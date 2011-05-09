#include "aihandler.h"
#include <QProcess>

AIHandler::AIHandler()
{
}

void AIHandler::run()
{

    return;
}

void MapHandler::onLoad()
{
    defaultField.resize(width*height);
    for (int i = 0; i < width*height; i++)
    {
        defaultField[i].influence = field[i].influence;
        defaultField[i].player = field[i].player;
    }
    CurrentStep = 0;
    //mb rezoom -> signal update
    //reset scores, currentplayer etc
}

MapHandler::MapHandler() : PlayerCount(3), CurrentStep(0), CurrentPlayer(1)
{
    connect(this, SIGNAL(Loaded()), this, SLOT(onLoad()));
}

void MapHandler::Step(int x, int y)
{
    if (!Valid(x, y))
        return;
    if (Player(x, y) != 0)
        return;
    setPlayer(x, y, CurrentPlayer);
    setInfluence(x, y, sequences[CurrentPlayer][CurrentStep%SequenceLength]);
    changed.push_back(QPoint(x, y));
    int influ = Influence(x, y);
    Check(x - 1, y, influ);
    Check(x + 1, y, influ);
    Check(x - 1 + y%2, y - 1, influ);
    Check(x + y%2, y - 1, influ);
    Check(x - 1 + y%2, y + 1, influ);
    Check(x + y%2, y + 1, influ);
    CurrentStep++;
    CurrentPlayer = CurrentPlayer%PlayerCount + 1;
    emit Update();
    //calculate scores somehow
}

void MapHandler::Check(int x, int y, int influ)
{
    if (!Valid(x, y))
        return;
    if (Player(x, y) == CurrentPlayer && Influence(x, y) != MAX_INFLUENCE)
    {
        setInfluence(x, y, Influence(x, y) + 1);
        changed.push_back(QPoint(x, y));
    }
    if (Player(x, y) != CurrentPlayer && Player(x, y) != 0 && Influence(x, y) < influ)
    {
        setPlayer(x, y, CurrentPlayer);
        changed.push_back(QPoint(x, y));
    }
}
