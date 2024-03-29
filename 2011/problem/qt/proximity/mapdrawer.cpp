#include "mapdrawer.h"
#include "hex.h"
#include "settingsmanager.h"
#include "definition.h"
#include "particlesystem.h"
#include "playercolor.h"
#include <algorithm>

MapDrawer::MapDrawer(bool createdDynamically): QObject(), Drawer::Drawable::Drawable(createdDynamically)
{
}

void MapDrawer::CalculateBBox()
{
    Drawer::Drawable::CalculateBBox();
    BB.setSize(QSizeF((map->Width() + 0.5f)*Hex::Width(), (map->Height() + 1.0f/3.0f)*0.75f*Hex::Height()));
}

void MapDrawer::SetMap(MapHandler *m)
{
    map = m;
    disconnect();
    connect(map, SIGNAL(Update()), this, SLOT(onUpdate()));
}

void MapDrawer::Draw(Drawer *drawer)
{
    map->lock();
    bool DrawBorder = SettingsManager::Instance()->getValue(GRAPHICS_SETTINGS, DRAW_BORDER, true).toBool();
    glLineWidth(5);
    int sy = std::max((-drawer->oy - 1)*4.0f/(Hex::Height()*3.0f), 0.0f),
        my = std::min((int)((-drawer->oy + 1 + (float)drawer->height()/drawer->Zoom)*4.0f/(Hex::Height()*3.0f)) + 1, map->Height()),
        mx = std::min((int)((-drawer->ox + (float)drawer->width()/drawer->Zoom)/Hex::Width()) + 1, map->Width());
    for (int j = sy; j < my; j++) // very bad
    {
        glPushMatrix();
        int sx = std::max((-drawer->ox)/Hex::Width() - j%2, 0.0f);//get valuse by coordinate -> hex.h
        glTranslatef((0.5f*(1 + j%2) + sx)*Hex::Width(), j*0.75f*Hex::Height() + Hex::Height()*0.5f, 0.0f);// mb replace with appropriate procedures like GetOffset & others
        for (int i = sx; i < mx; i++)
        {
            glColor3f(0.0f, 0.0f, 0.0f);
            short pl = map->Player(i, j);
            Hex::Surface(pl, pl==0?MAX_INFLUENCE:map->Influence(i, j));
            if (DrawBorder)
            {
                glLineWidth(drawer->Zoom*0.04f);
                if (pl)
                {
                    QColor pc = PlayerColors::Color(pl);
                    glColor4f(pc.redF(), pc.greenF(), pc.blueF(), 0.6f);
                }
                else
                    glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
                glCallList(Hex::Border());
            }
            //if (map->Influence(i,j))
            //{
            //    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            //    drawer->DrawText((i + (1 + j%2)*0.5f)*Hex::Width(), j*Hex::Height()*0.75f, 0, QString::number(map->Influence(i, j)));
            //}
            glTranslatef(Hex::Width(), 0.0f, 0.0f);
        }
        glPopMatrix();
    }
    for (int j = sy; j < my; j++) // very bad
    {
        glPushMatrix();
        int sx = std::max((-drawer->ox)/Hex::Width() - j%2, 0.0f);//get valuse by coordinate -> hex.h
        glTranslatef((0.5f*(1 + j%2) + sx)*Hex::Width(), j*0.75f*Hex::Height() + Hex::Height()*0.5f, 0.0f);// mb replace with appropriate procedures like GetOffset & others
        for (int i = sx; i < mx; i++)
        {
            if (map->Influence(i,j))
            {
                Hex::DrawCaption(map->Influence(i, j));
            }
            glTranslatef(Hex::Width(), 0.0f, 0.0f);
         }
        glPopMatrix();
    }
    while (!map->changed.isEmpty())
    {
        Explode(drawer, map->changed.front().x(), map->changed.front().y());
        map->changed.pop_front();
    }
    if (DisplayGraph)
    {
        glPushMatrix();
        glLoadIdentity();
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        glBegin(GL_QUADS);
        glVertex3f(10.0f, 10.0f, -0.8f);
        glVertex3f(drawer->width() - 10, 10.0f, -0.8f);
        glVertex3f(drawer->width() - 10, 110, -0.8f);
        glVertex3f(10.0f, 110, -0.8f);
        glEnd();
        for (int i = 0; i < map->PlayersCount(); i++)
        {
            QColor c = PlayerColors::Color(i + 1);
            glColor4f(c.redF(), c.greenF(), c.blueF(), 0.5f);
            glLineWidth(3.0f);
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < (drawer->width()-20)*0.1f; j++)
            {
                glVertex3f(10.0f + j*20.0f, 110.0f - map->Sequence(i, j)*5.0f + 5.0f, -0.9f);
            }
            glEnd();
        }
        glPopMatrix();
    }
    map->unlock();
}

void MapDrawer::Explode(Drawer *drawer, int x, int y)
{
    QPointF r = GetCoord(x, y);
    Particle *p = new Particle(150, r.x(), r.y(), 0.6f, PlayerColors::Color(map->Player(x, y)), 0.4f);
    drawer->Add(p);
}

void MapDrawer::Click(Drawer *drawer, float x, float y)
{
    DisplayGraph = !DisplayGraph;
    Updated = false;
/*    QPoint t = GetCell(x, y);
    if (t.x() == -1 || t.y() == -1)
        return;
    map->Step(t.x(), t.y());

    if (!map->Player(t.x(), t.y()))
        return;
    while (!map->changed.isEmpty())
    {
        Explode(drawer, map->changed.front().x(), map->changed.front().y());
        map->changed.pop_front();
    }*/
}

QPointF MapDrawer::GetCoord(int x, int y)
{
    float rx = (x + (1 + y%2)*0.5f)*Hex::Width(), ry = y*0.75*Hex::Height() + Hex::Height()*0.5f;
    return QPointF::QPointF(rx, ry);
}

bool MapDrawer::inHex(float x, float y, int xx, int yy)
{
    if (xx < 0 || yy < 0 || xx >= map->Width() || yy >= map->Height())
        return false;
    x -= (xx + (yy%2 + 1)*0.5f)*Hex::Width();
    y -= yy*(0.75f*Hex::Height()) + Hex::Height()*0.5f;
    QLineF d(0.0f, 0.0f, x, y);
    for (int i = 0; i < 6; i+=1 + (i%3==0))
         if (!d.intersect(Hex::Line(i), NULL))
            return false;
    return true;
}

QPoint MapDrawer::GetCell(float x, float y)
{
    int yy = y/(0.75f*Hex::Height());
    int xx = (x-0.5f*(yy%2))/Hex::Width();
    if (inHex(x, y, xx, yy))
        return QPoint::QPoint(xx, yy);
    if (inHex(x, y, xx + yy%2, yy - 1))
        return QPoint::QPoint(xx + yy%2, yy - 1);
    if (inHex(x, y, xx + yy%2 - 1, yy - 1))
        return QPoint::QPoint(xx + yy%2 - 1, yy - 1);
    if (inHex(x, y, xx + yy%2, yy + 1))
        return QPoint::QPoint(xx + yy%2, yy + 1);
    if (inHex(x, y, xx + yy%2 - 1, yy + 1))
        return QPoint::QPoint(xx + yy%2 - 1, yy + 1);
    return QPoint::QPoint(-1, -1);

}

void MapDrawer::onUpdate()
{
    Updated = false;
}
