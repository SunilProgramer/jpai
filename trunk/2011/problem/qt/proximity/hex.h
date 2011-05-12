#ifndef HEX_H
#define HEX_H

#include <QtOpenGL>
#include <GL/gl.h>
#include <QLineF>
#include "game.h"

const int Quality = 2;

class Hex
{
public:
    Hex();
    static void Surface(int player, int influence);
    static QColor GetColor(int Player, int influence);
    static GLuint Border();
    static void CleanUp();
    static float Width();
    static float Height();
    static QLineF Line(int index);
private:
    static void vtx(int ind, float x, float y, float z);
    static float vertices_array[(Quality*6 + 2)*3];
    static float colors_array[(Quality*6 + 2)*4];
    static float prev_color[4];
    static bool surface_generated;
    static bool color_generated;
    static GLuint border;
    static bool border_generated;
    static void GenerateSurface();
    static GLuint GenerateBorder();
};

#endif // HEX_H
