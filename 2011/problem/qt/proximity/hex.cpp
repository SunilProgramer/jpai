#include "hex.h"
GLuint Hex::border = 0;
GLuint Hex::surface = 0;
bool Hex::border_generated = false;
bool Hex::surface_generated = false;
const float HexConst[] = {173.0f/400.0f, 0.25f, 0.5f};

Hex::Hex()
{
}

GLuint Hex::Border()
{
    if (!Hex::border_generated)
    {
        Hex::border = Hex::GenerateBorder();
        Hex::border_generated = true;
    }
    return Hex::border;
}

GLuint Hex::Surface()
{
    if (!Hex::surface_generated)
    {
        Hex::surface = Hex::GenerateSurface();
        Hex::surface_generated = true;
    }
    return Hex::surface;
}

void Hex::CleanUp()
{
    if (Hex::border_generated)
        glDeleteLists(Hex::border, 1);
    if (Hex::surface_generated)
        glDeleteLists(Hex::surface, 1);
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

void vtx(float x, float y, float z)
{
    float r = sqrt(x*x + y*y)/1.0f;
    r = r>1.0f?1.0f:r;
    glColor3f(r, r, r);//, r);
    glVertex3f(x, y, z);
}

GLuint Hex::GenerateSurface()
{
    GLuint res = glGenLists(1);
    glNewList(res, GL_COMPILE);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 0.0f);//, 0.01f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    //glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
    for (float i = 0; i <= 1.0f; i += 0.5f)
    {
        vtx(blend(0.0f, HexConst[0], i), blend(-HexConst[2], -HexConst[1], i), 0.0f);
    }
    for (float i = 0; i <= 1.0f; i += 0.5f)
        vtx(HexConst[0], blend(-HexConst[1], HexConst[1], i), 0.0f);
    for (float i = 0; i <= 1.0f; i += 0.5f)
        vtx(blend(HexConst[0], 0.0f, i), blend(HexConst[1], HexConst[2], i), 0.0f);
    for (float i = 0; i <= 1.0f; i += 0.5f)
        vtx(blend(0.0f, -HexConst[0], i), blend(HexConst[2], HexConst[1], i), 0.0f);
    for (float i = 0; i <= 1.0f; i += 0.5f)
        vtx(-HexConst[0], blend(HexConst[1], -HexConst[1], i), 0.0f);
    for (float i = 0; i <= 1.0f; i += 0.5f)
        vtx(blend(-HexConst[0], 0.0f, i), blend(-HexConst[1], -HexConst[2], i), 0.0f);
    vtx(0.0f, -HexConst[2], 0.0f);
    //glVertex3f(0.0f, -HexConst[2], 0.0f);
   // glVertex3f(HexConst[0], -HexConst[1], 0.0f);
   // glVertex3f(HexConst[0], HexConst[1], 0.0f);
    //glVertex3f(0.0f, HexConst[2], 0.0f);
    //glVertex3f(-HexConst[0], HexConst[1], 0.0f);
    //glVertex3f(-HexConst[0], -HexConst[1], 0.0f);
    //glVertex3f(0.0f, -HexConst[2], 0.0f);
    glEnd();
    glEndList();
    return res;
}
