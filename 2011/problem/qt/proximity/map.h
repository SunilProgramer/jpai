#ifndef MAP_H
#define MAP_H

#include "drawer.h"
//#include "utils.h"

class Map
{
public:
    struct Cell
    {
        short player;
        short influence;
    };
    Map();
    ~Map();
    void Load(const QString &filename);
    void Export(const QString &filename);
    Cell operator() (const int &x, const int &y);
    int Player(const int &x, const int &y);
    int Influence(const int &x, const int &y);
    int Width();
    int Height();
protected:
    QVector<Cell> field; // **field <>
    QVector<QVector<short> > sequences;
    int width, height, MaxPlayersCount, Window, SequenceLength;
};

class MapDrawer : public Drawer::Drawable
{
public:
    MapDrawer(bool createdDynamically = true);
    void SetMap(const Map &m);
    Map &GetMap();
protected:
    QPointF GetCoord(int x, int y);
    QPoint GetCell(float x, float y);
    Map map;
    void CalculateBBox();
    void Draw(Drawer *drawer);
    void Click(Drawer *drawer, float x, float y);
    GLuint surface, border;
};

#endif // MAP_H
