#ifndef MAP_H
#define MAP_H

//#include "utils.h"
#include <QObject>
#include <QVector>
#include <QMutex>

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
    virtual void Export(const QString &filename){}
    Cell &Cells (const int &x, const int &y);
    int Player(const int &x, const int &y);
    int Influence(const int &x, const int &y);
    void setPlayer(const int &x, const int &y, const int &Player);
    void setInfluence(const int &x, const int &y, const int &Influence);
    int Width();
    int Height();
    void lock();
    void unlock();
    static QVector<int> Preprocess(const QString &filename);
signals:
    void Loaded();
    void Update();
public slots:
    virtual void onLoad(){}
protected:
    QMutex mutex;
    bool Valid(const int &x, const int &y);
    QVector<Cell> field; // **field <>
    QVector<QVector<short> > sequences;
    int width, height, MaxPlayersCount, SequenceOutLength, SequenceLength;
};


#endif // MAP_H
