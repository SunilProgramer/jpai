#include "map.h"
#include "hex.h"
#include <algorithm>

Map::Map(bool createdDynamically): Drawer::Drawable::Drawable(createdDynamically)
{
    Width = 200;
    Height = 200;
    BB.setWidth((Width + 0.5f)*Hex::Width());
    BB.setHeight((Height + 1.0f/3.0f)*3.0f*Hex::Height()/4.0f);
}

void Map::Draw(Drawer *drawer)
{
    glLineWidth(1);
    int sy = std::max((-drawer->oy - 1)*4.0f/(Hex::Height()*3.0f), 0.0f), my = std::min((int)((-drawer->oy + 1 + (float)drawer->height()/drawer->Zoom)*4.0f/(Hex::Height()*3.0f)) + 1, Height);
    for (int j = sy; j < my; j++) // very bad
    {
        glPushMatrix();
        int sx = std::max((-drawer->ox)/Hex::Width() - j%2, 0.0f), mx = std::min((int)((-drawer->ox + (float)drawer->width()/drawer->Zoom)/Hex::Width()) + 1, Width);
        glTranslatef((0.5f*(1 + j%2) + sx)*Hex::Width(), (j + 0.5f)*3.0f*Hex::Height()/4.0f, 0.0f);// mb replace with appropriate procedures like GetOffset & others
        for (int i = sx; i < mx; i++)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glCallList(Hex::Surface());
            glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
            glCallList(Hex::Border());
            glTranslatef(Hex::Width(), 0.0f, 0.0f);
        }
        glPopMatrix();
    }
}
