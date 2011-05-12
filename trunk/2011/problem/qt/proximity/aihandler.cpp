#include "aihandler.h"
#include "directorymanager.h"
#include "definition.h"
#include "executor.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

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
    defaultField.resize(width*height);
    for (int i = 0; i < width*height; i++)
    {
        defaultField[i].influence = field[i].influence;
        defaultField[i].player = field[i].player;
        defaultFreecells += field[i].player==0;
    }
    CurrentStep = 0;
    CurrentPlayer = 1;
    freecells = defaultFreecells;
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
    setInfluence(x, y, sequences[CurrentPlayer - 1][CurrentStep%SequenceLength]);
    changed.push_back(QPoint(x, y));
    int influ = Influence(x, y);
    Check(x - 1, y, influ);
    Check(x + 1, y, influ);
    Check(x - 1 + y%2, y - 1, influ);
    Check(x + y%2, y - 1, influ);
    Check(x - 1 + y%2, y + 1, influ);
    Check(x + y%2, y + 1, influ);
    CurrentStep += CurrentPlayer==(Players.size());
    CurrentPlayer = CurrentPlayer%Players.size() + 1;
    unlock();
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
    //mb rezoom -> signal update
    //reset scores, currentplayer etc

    emit Update();
}

void MapHandler::ClearPlayers()
{
    Players.clear();
    PlayersId.clear();
    PlayersIdentitiy.clear();
    Reset();
}

void MapHandler::AddPlayer(const QString &name, const int &id, bool Player)
{
    Players.push_back(name);
    PlayersId.push_back(id);
    PlayersIdentitiy.push_back(Player);
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

int MapHandler::PlayersCount()
{
    return Players.size();
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
        stream << field[i].player << " " << field[i].influence << " ";
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
