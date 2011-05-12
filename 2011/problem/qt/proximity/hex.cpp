#include "hex.h"
#include "playercolor.h"
#include <QPair>
#include <QMap>
GLuint Hex::border = 0;
GLuint Hex::surface = 0;
GLuint Hex::surface_cover = 0;
bool Hex::border_generated = false;
bool Hex::surface_generated = false;
bool Hex::color_generated = false;
float Hex::colors_array[(Quality*6 + 2)*4];
float Hex::vertices_array[(Quality*6 + 2)*3];
float Hex::prev_color[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
QMap<QPair<int, int>, GLuint> surfaces;
QMap<QPair<int, int>, bool> surfaces_generated;
const float HexConst[] = {173.0f/400.0f, 0.25f, 0.5f};
const float step = 1.0f/Quality;

Hex::Hex()
{
}

GLuint Hex::Border()
{
    if (!border_generated)
    {
        border = Hex::GenerateBorder();
        border_generated = true;
    }
    return border;
}

QColor Hex::GetColor(int Player, int influence)
{
    QColor c = PlayerColors::Color(Player);
    if (influence && Player)
    {
        c = Blend(PlayerColors::Color(0), c, (float)influence/MAX_INFLUENCE);
    }
    return c;
}

void Hex::Surface(int player, int influence)
{
    QColor c = GetColor(player, (int)floor(influence/2)*2);
    QPair<int, int> p = qMakePair(player, (int)floor(influence/2)*2);
    qreal cl[4];
    cl[0] = c.redF();
    cl[1] = c.greenF();
    cl[2] = c.blueF();
    cl[3] = c.alphaF();
    Hex::color_generated = true;
    for (int i = 0; i < 4; i++)
        Hex::prev_color[i] = cl[i];
    if (!surfaces_generated[p])
    {

        surfaces[p] = Hex::GenerateSurface();
        surfaces_generated[p] = true;
    }
    glCallList(surfaces[p]);
    //return surface;
}

void Hex::CleanUp()
{
    if (border_generated)
        glDeleteLists(border, 1);
}

float Hex::Height()
{
    return 1.0f;
}

float Hex::Width()
{
    return HexConst[0]*2;
}

GLuint Hex::GenerateBorder()
{
    GLuint res = glGenLists(1);
    glNewList(res, GL_COMPILE);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0f, -HexConst[2], 0.0f);
    glVertex3f(HexConst[0], -HexConst[1], 0.0f);
    glVertex3f(HexConst[0], HexConst[1], 0.0f);
    glVertex3f(0.0f, HexConst[2], 0.0f);
    glVertex3f(-HexConst[0], HexConst[1], 0.0f);
    glVertex3f(-HexConst[0], -HexConst[1], 0.0f);
    glEnd();
    glEndList();
    return res;
}
float blend(float x, float y, float c)
{
    return x*(1.0f-c) + y*c;
}

void Hex::vtx(int ind, float x, float y, float z)
{
    float r = sqrt(x*x + y*y)/0.5f;
    r = r>1.0f?1.0f:r;
    glColor4f(r*Hex::prev_color[0], r*Hex::prev_color[1], r*Hex::prev_color[2], 1.0f);
    glVertex3f(x, y, z);
}

GLuint Hex::GenerateSurface()
{
    GLuint res = glGenLists(1);
    glNewList(res, GL_COMPILE);
    int j = 0;
    glBegin(GL_TRIANGLE_FAN);
    vtx(j++, 0.0f, 0.0f, 0.0f);
    for (float i = 0.0f; i <= 1.0f; i += step)
        vtx(j++, blend(0.0f, HexConst[0], i), blend(-HexConst[2], -HexConst[1], i), 0.0f);
    for (float i = step; i <= 1.0f; i += step)
        vtx(j++, HexConst[0], blend(-HexConst[1], HexConst[1], i), 0.0f);
    for (float i = step; i <= 1.0f; i += step)
        vtx(j++, blend(HexConst[0], 0.0f, i), blend(HexConst[1], HexConst[2], i), 0.0f);
    for (float i = step; i <= 1.0f; i += step)
        vtx(j++, blend(0.0f, -HexConst[0], i), blend(HexConst[2], HexConst[1], i), 0.0f);
    for (float i = step; i <= 1.0f; i += step)
        vtx(j++, -HexConst[0], blend(HexConst[1], -HexConst[1], i), 0.0f);
    for (float i = step; i <= 1.0f; i += step)
        vtx(j++, blend(-HexConst[0], 0.0f, i), blend(-HexConst[1], -HexConst[2], i), 0.0f);
    glEnd();
    glEndList();
    return res;
}

QLineF Hex::Line(int index)
{
    return QLineF(vertices_array[index*Quality*3 + 3], vertices_array[index*Quality*3 + 4],
                  vertices_array[(index+1)*Quality*3 + 3], vertices_array[(index+1)*Quality*3 + 4]);
}
