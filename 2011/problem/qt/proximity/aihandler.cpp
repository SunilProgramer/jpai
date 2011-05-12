#include "aihandler.h"
#include "directorymanager.h"
#include "definition.h"
#include "executor.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QSqlQuery>
#include <QVariant>

AIHandler::AIHandler()
{
}

void AIHandler::run()
{
    while (true)
    {
        //executing
        for (int i = 0; i < Handler->PlayersCount(); i++)
        {
            if (!Handler->FreeCells())
            {
                Stop();
            }
            else
            {
                QDir d = DirectoryManager::Instance()->Directory(DIRECTORY_RUN);
                Executor e(MAX_RUNNING_TIME);
                if (!Handler->GetPlayerIdentity(i))
                {
                    d.mkdir("Player" + QString::number(i));
                    d.cd("Player" + QString::number(i));
                    QStringList l = d.entryList(QDir::Files);
                    while (!l.isEmpty())
                    {
                        if (l.front() != TEMP_FILE)
                            d.remove(l.front());
                        l.pop_front();
                    }
                    Handler->Export(d.filePath(INPUT_FILE));
                    e.run(d.absolutePath(), DirectoryManager::Instance()->Directory(DIRECTORY_AIS).filePath(Handler->GetPlayer(i)));
                    Handler->Step(GetOutput(d.filePath(OUTPUT_FILE)));
                    //process output
                    d.cdUp();
                }
            }
        }
        Handler->CalculateScores();
        emit StepFinished();
        mutex.lock();
        if (Stopped)
        {
            mutex.unlock();
            emit Finished();
            quit();
            return;
        }
        mutex.unlock();
    }
    return;
}

void AIHandler::Start()
{
    //
    mutex.lock();
    Stopped = false;
    mutex.unlock();
    if (!isRunning())
        start(QThread::HighestPriority);
}

void AIHandler::Stop()
{
    mutex.lock();
    Stopped = true;
    mutex.unlock();
}

void AIHandler::SetMapHandler(MapHandler *handler)
{
    Handler = handler;
}

QString AIHandler::GetOutput(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }

    QTextStream stream(&file);
    return stream.readAll();
}

void MapHandler::onLoad()
{
    lock();
    defaultField.resize(width*height);
    for (int i = 0; i < width*height; i++)
    {
        defaultField[i].influence = field[i].influence;
        defaultField[i].player = field[i].player;
    }
    CurrentStep = 0;
    CurrentPlayer = 1;
    defaultFreecells = freecells = width*height;
    stepped = 0;
    unlock();
    //mb rezoom -> signal update
    //reset scores, currentplayer etc
}
int MapHandler::FreeCells()
{
    return freecells;
}

MapHandler::MapHandler() : Map(), CurrentStep(0), CurrentPlayer(1), defaultFreecells(0)
{
    connect(this, SIGNAL(Loaded()), this, SLOT(onLoad()));
}

void MapHandler::Step(int x, int y)
{
    if (!Valid(x, y))
    {
        //qDebug("0!");
        CurrentStep += CurrentPlayer==(Players.size());
        CurrentPlayer = CurrentPlayer%Players.size() + 1;
        emit Update();
        return;
    }
    if (Player(x, y) != 0)
    {
        //qDebug("1!");
        CurrentStep += CurrentPlayer==(Players.size());
        CurrentPlayer = CurrentPlayer%Players.size() + 1;
        emit Update();
        return;
    }
    lock();
    setPlayer(x, y, CurrentPlayer);
    freecells--;
    stepped++;
    setInfluence(x, y, sequences[CurrentPlayer - 1][CurrentStep%SequenceLength]);
    changed.push_back(QPoint(x, y));
    int influ = Influence(x, y);
    Check(x - 1, y, influ);
    Check(x + 1, y, influ);
    Check(x - 1 + y%2, y - 1, influ);
    Check(x + y%2, y - 1, influ);
    Check(x - 1 + y%2, y + 1, influ);
    Check(x + y%2, y + 1, influ);
    /*QSqlQuery query;
    query.prepare("insert into player_moves (player_competition_matches_id, player, influence, step, current_step, x, y)"
                  "values (:player_competition_matches_id, :player, :influence, :step, :current_step, :x, :y)");
    query.bindValue(":player_competition_matches_id", QVariant(GetPlayerId(CurrentPlayer - 1)));
    query.bindValue(":player", CurrentPlayer);
    query.bindValue(":influence", Influence(x, y));
    query.bindValue(":step", QVariant(stepped));
    query.bindValue(":current_step", QVariant(CurrentStep));
    query.bindValue(":x", QVariant(x));
    query.bindValue(":y", QVariant(y));
    query.exec();*/
    CurrentStep += CurrentPlayer==(Players.size());
    CurrentPlayer = CurrentPlayer%Players.size() + 1;
    unlock();
    emit Update();
    //calculate scores somehow
}
void MapHandler::silentStep(int x, int y, int Player, int influence)
{
    setPlayer(x, y, Player);
    freecells--;
    stepped++;
    setInfluence(x, y, influence);
    changed.push_back(QPoint(x, y));
    int influ = Influence(x, y);
    Check(x - 1, y, influence);
    Check(x + 1, y, influence);
    Check(x - 1 + y%2, y - 1, influence);
    Check(x + y%2, y - 1, influence);
    Check(x - 1 + y%2, y + 1, influence);
    Check(x + y%2, y + 1, influence);
    CurrentPlayer = 1;
}

int MapHandler::Check(int x, int y, int influ)
{
    if (!Valid(x, y))
        return 0;
    if (Player(x, y) == CurrentPlayer && Influence(x, y) != MAX_INFLUENCE)
    {
        setInfluence(x, y, Influence(x, y) + 1);
        changed.push_back(QPoint(x, y));
    }
    if (Player(x, y) != CurrentPlayer && Player(x, y) != 0 && Influence(x, y) < influ)
    {
        setPlayer(x, y, CurrentPlayer);
        changed.push_back(QPoint(x, y));
        return 1;
    }
    return 0;
}

void MapHandler::Reset()
{
    for (int i = 0; i < width*height; i++)
    {
        field[i].influence = defaultField[i].influence;
        field[i].player = defaultField[i].player;
    }
    CurrentStep = 0;
    CurrentPlayer = 1;
    freecells = defaultFreecells;
    stepped = 0;
    //mb rezoom -> signal update
    //reset scores, currentplayer etc

    emit Update();
    emit Reseted();
}

void MapHandler::ClearPlayers()
{
    Players.clear();
    PlayersId.clear();
    PlayersIdentitiy.clear();
    scores.clear();
    Reset();
}

void MapHandler::AddPlayer(const QString &name, const int &id, bool Player)
{
    Players.push_back(name);
    PlayersId.push_back(id);
    PlayersIdentitiy.push_back(Player);
    scores.push_back(0);
    emit Update();
}

QString MapHandler::GetPlayer(const int &index)
{
    return Players[std::max(std::min(index, Players.size()), 0)];
}

bool MapHandler::GetPlayerIdentity(const int &index)
{
    return PlayersIdentitiy[std::max(std::min(index, Players.size()), 0)];
}

int MapHandler::GetPlayerId(const int &index)
{
    return PlayersId[std::max(std::min(index, Players.size()), 0)];
}
int MapHandler::GetPlayerNum(const int &id)
{
    return PlayersId.indexOf(id) + 1;
}

int MapHandler::PlayersCount()
{
    return Players.size();
}

int MapHandler::Stepped()
{
    return stepped;
}
void MapHandler::SetCurrentStep(int step)
{
    CurrentStep = step;
    emit Update();
}

void MapHandler::Export(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return; // throw!
    }

    QTextStream stream(&file);
    stream << width << " " << height << " " << SequenceOutLength << " " << PlayersCount() << "\n";
    for (int i = 0; i < SequenceOutLength; i++)
        stream << sequences[CurrentPlayer - 1][(i+CurrentStep)%SequenceLength] << " ";
    for (int i = 0; i < width*height; i++)
    {
        if (i%width == 0)
            stream << "\n";
        stream << (field[i].player?((field[i].player - CurrentPlayer + PlayersCount())%PlayersCount() + 1):field[i].player) << " " << field[i].influence << " ";
    }
}

void MapHandler::Step(const QString &command)
{
    QRegExp r("^(-?\\d+)\\s+(-?\\d+)");
    if (!r.exactMatch(command))
    {
        Step(-1, -1);
        return;
    }
    QStringList l = r.capturedTexts();
    Step(l[1].toInt() - 1, l[2].toInt() - 1);
}
void MapHandler::CalculateScores()
{
    lock();
    for (int i = 0; i < scores.size(); i++)
        scores[i] = 0;
    for (int i = 0; i < width*height; i++)
        if (field[i].player != 0)
            scores[field[i].player - 1]++;
    QSqlQuery query;
    for (int i = 0; i < PlayersCount(); i++)
    {
        query.prepare("select id from player_scores where player_competition_matches_id = :player_id");
        query.bindValue(":player_id", GetPlayerId(i));
        query.exec();
        query.first();
        if (query.isValid())
            query.prepare("update player_scores set scores = :scores where player_competition_matches_id = :player_id");
        else
            query.prepare("insert into player_scores (scores, player_competition_matches_id) values(:scores, :player_id)");
        query.bindValue(":scores", scores[i]);
        query.bindValue(":player_id", GetPlayerId(i));
        query.exec();
    }
    unlock();
    //submit it to db
}
