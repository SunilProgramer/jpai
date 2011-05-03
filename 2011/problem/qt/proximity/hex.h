#ifndef HEX_H
#define HEX_H

#include <QtOpenGL>
#include <GL/gl.h>

class Hex
{
public:
    Hex();
    static GLuint Surface();
    static GLuint Border();
    static void CleanUp();
    static float Width();
    static float Height();
private:
    static GLuint surface;
    static bool surface_generated;
    static GLuint border;
    static bool border_generated;
    static GLuint GenerateSurface();
    static GLuint GenerateBorder();
};

#endif // HEX_H
