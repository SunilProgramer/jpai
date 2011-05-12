#include "map.h"
#include <QFile>
#include <QTextStream>


Map::Map() : field(), width(0), height(0), MaxPlayersCount(0), SequenceOutLength(0), SequenceLength(0)
{
}

Map::~Map()
{
}

Map::Cell &Map::Cells(const int &x, const int &y)
{
    if (!Valid(x, y))
        throw " ";// bad
    return field[x + y*width];
}

int Map::Influence(const int &x, const int &y)
{
    return Cells(x, y).influence;
}

int Map::Player(const int &x, const int &y)
{
    return Cells(x, y).player;
}

void Map::setPlayer(const int &x, const int &y, const int &Player)
{
    Cells(x, y).player = Player;
}

void Map::setInfluence(const int &x, const int &y, const int &Influence)
{
    Cells(x, y).influence = Influence;
}

bool Map::Load(const QString &filename)
{
    lock();
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream stream(&file);

    stream >> width >> height >> MaxPlayersCount >> SequenceOutLength >> SequenceLength;
    MaxPlayersCount = 10;// comment this line

    sequences.resize(MaxPlayersCount);
    field.resize(width*height);

    for (int i = 0; i < MaxPlayersCount; i++)
    {
        sequences[i].resize(SequenceLength);
        for (int j = 0; j < SequenceLength; j++)
            stream >> sequences[i][j];
    }

    for (int i = 0; i < width*height; i++)
        stream >> field[i].player >> field[i].influence;
    unlock();
    emit Loaded();
    emit Update();
    return true;
}

int Map::Width()
{
    return width;
}
int Map::Height()
{
    return height;
}

bool Map::Valid(const int &x, const int &y)
{
    return !(x < 0 || x >= width || y < 0 || y >= height);
}

QVector<int> Map::Preprocess(const QString &filename)
{
    QVector<int> res; // player limit ?!!!!
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw "";// some error here!
    }

    QTextStream stream(&file);

    for (int i = 0; i < 5; i++)
    {
        int k;
        stream >> k;
        res.push_back(k);
    }
    return res;
}

void Map::lock()
{
    mutex.lock();
}
void Map::unlock()
{
    mutex.unlock();
}


