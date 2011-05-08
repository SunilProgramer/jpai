#include "map.h"
#include "hex.h"
#include "settingsmanager.h"
#include "definition.h"
#include "particlesystem.h"
#include <algorithm>

Map::Map() : field(), width(0), height(0), MaxPlayersCount(0), Window(0), SequenceLength(0)
{
}

Map::~Map()
{
}

Map::Cell Map::operator ()(const int &x, const int &y)
{
    if (x < 0 || x >= width || y < 0 || x >= height)
        throw " ";// bad
    return field[x + y*width];
}

int Map::Influence(const int &x, const int &y)
{
    return operator ()(x, y).influence;
}

int Map::Player(const int &x, const int &y)
{
    return operator ()(x, y).player;
}

void Map::Load(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);

    stream >> width >> height >> MaxPlayersCount >> Window >> SequenceLength;
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
}

int Map::Width()
{
    return width;
}
int Map::Height()
{
    return height;
}

MapDrawer::MapDrawer(bool createdDynamically): Drawer::Drawable::Drawable(createdDynamically)
{
}

void MapDrawer::CalculateBBox()
{
    Drawer::Drawable::CalculateBBox();
    BB.setWidth((map.Width() + 0.5f)*Hex::Width());
    BB.setHeight((map.Height() + 1.0f/3.0f)*3.0f*Hex::Height()/4.0f);
}

void MapDrawer::SetMap(const Map &m)
{
    map = m;
}

void MapDrawer::Draw(Drawer *drawer)
{
    bool DrawBorder = SettingsManager::Instance()->getValue(GRAPHICS_SETTINGS, DRAW_BORDER, true).toBool();
    glLineWidth(5);
    int sy = std::max((-drawer->oy - 1)*4.0f/(Hex::Height()*3.0f), 0.0f),
        my = std::min((int)((-drawer->oy + 1 + (float)drawer->height()/drawer->Zoom)*4.0f/(Hex::Height()*3.0f)) + 1, map.Height());
    for (int j = sy; j < my; j++) // very bad
    {
        glPushMatrix();
        int sx = std::max((-drawer->ox)/Hex::Width() - j%2, 0.0f),
            mx = std::min((int)((-drawer->ox + (float)drawer->width()/drawer->Zoom)/Hex::Width()) + 1, map.Width());//get valuse by coordinate -> hex.h
        glTranslatef((0.5f*(1 + j%2) + sx)*Hex::Width(), j*0.75f*Hex::Height() + Hex::Height()*0.5f, 0.0f);// mb replace with appropriate procedures like GetOffset & others
        for (int i = sx; i < mx; i++)
        {
            glColor3f(0.0f, 0.0f, 0.0f);
            short pl = map.Player(i, j);
            Hex::Surface(pl, pl==0?MAX_INFLUENCE:map.Influence(i, j));
            if (DrawBorder)
            {
                glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
                glCallList(Hex::Border());
            }
            if (map.Influence(i,j))
            {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                drawer->DrawText((i + (1 + j%2)*0.5f)*Hex::Width(), j*Hex::Height()*0.75f, 0, QString::number(map.Influence(i, j)));
            }
            glTranslatef(Hex::Width(), 0.0f, 0.0f);
        }
        glPopMatrix();
    }

}

void MapDrawer::Click(Drawer *drawer, float x, float y)
{
    qDebug("1");
    Particle *p = new Particle(300, x, y, 0.2f, QColor(rand()%256, rand()%256, rand()%256,255), 0.2f);
    drawer->Add(p);
}
