#ifndef MAP_H
#define MAP_H

#include "drawer.h"

class Map : public Drawer::Drawable
{
public:
    Map(bool createdDynamically = false);
protected:
    void Draw(Drawer *drawer);
    int Width, Height;
    GLuint surface, border;
};

#endif // MAP_H
