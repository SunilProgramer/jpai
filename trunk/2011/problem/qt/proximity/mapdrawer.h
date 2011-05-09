#ifndef MAPDRAWER_H
#define MAPDRAWER_H

#include "aihandler.h"
#include "drawer.h"

class MapDrawer : public QObject, public Drawer::Drawable
{
    Q_OBJECT
public:
    MapDrawer(bool createdDynamically = true);
    void SetMap(MapHandler *m);
    MapHandler *GetMap();
protected:
    QPointF GetCoord(int x, int y);
    QPoint GetCell(float x, float y);
    bool inHex(float x, float y, int xx, int yy);
    MapHandler *map;
    void CalculateBBox();
    void Draw(Drawer *drawer);
    void Click(Drawer *drawer, float x, float y);
    void Explode(Drawer *drawer, int x, int y);
    GLuint surface, border;
public slots:
    void onUpdate();
};

#endif // MAPDRAWER_H
