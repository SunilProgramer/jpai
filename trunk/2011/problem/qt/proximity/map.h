#ifndef MAP_H
#define MAP_H

//#include "utils.h"
#include <QObject>
#include <QVector>

class Map : public QObject
{
    Q_OBJECT
public:
    struct Cell
    {
        short player;
        short influence;
    };
    Map();
    ~Map();
    bool Load(const QString &filename);
    void Export(const QString &filename);
    Cell &Cells (const int &x, const int &y);
    int Player(const int &x, const int &y);
    int Influence(const int &x, const int &y);
    void setPlayer(const int &x, const int &y, const int &Player);
    void setInfluence(const int &x, const int &y, const int &Influence);
    int Width();
    int Height();
signals:
    void Loaded();
    void Update();
protected:
    bool Valid(const int &x, const int &y);
    QVector<Cell> field; // **field <>
    QVector<QVector<short> > sequences;
    int width, height, MaxPlayersCount, SequenceOutLength, SequenceLength;
};


#endif // MAP_H
