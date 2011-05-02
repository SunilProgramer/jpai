#include "drawer.h"
#include <algorithm>
using namespace std;

Drawer::Drawer(QWidget *parent) :
    QGLWidget(parent)
{
    Zoom = 1.0f;
    MapWidth = 120;
    MapHeight = 120;
    ox = 0;
    oy = 0; // remove this stuff

}

void Drawer::resizeGL(int w, int h)
{
    h = h?h:!h;

    glViewport(0, 0, w, h);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w-1.0, h-1.0, 0.0, 10.0, -10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Drawer::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void Drawer::paintGL()
{
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
        glLoadIdentity();									// Reset The Current Modelview Matrix
        glLineWidth(sqrt(width()*height())/100.0f);
        glBegin(GL_LINE_LOOP);

                glColor3f(1.0f, 0,0);// Drawing Using Triangles
                glVertex3f( ox*Zoom, oy*Zoom, 0.0f);					// Top
                glColor3f(0, 1.0f,0);// Drawing Using Triangles
                glVertex3f((MapWidth+ox)*Zoom, oy*Zoom, 0.0f);					// Bottom Left
                glColor3f(0, 0,1.0f);// Drawing Using Triangles
                glVertex3f( (MapWidth+ox)*Zoom,(MapHeight+oy)*Zoom, 0.0f);					// Bottom Right
        glEnd();											// Finished Drawing The Triangle
        glTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units
        glBegin(GL_QUADS);									// Draw A Quad
                glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
                glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
                glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
                glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
                glEnd();											// Done Drawing The Quad
}

void Drawer::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}
float clamp(float x, float min_val, float max_val)
{
    return min(max(min_val, x), max_val);
}

void Drawer::wheelEvent(QWheelEvent *event)
{
    QPoint p = mapFromGlobal(QCursor::pos());
    float OldZoom = Zoom;
    Zoom *= pow(1.1f, event->delta()/120.0f);
    Zoom = clamp(Zoom, 0.2f, 8.0f);//ZOOM_MIN, ZOOM_MAX);
    if (Zoom == OldZoom)
        return;
    float Tempx, Tempy;
    Tempx = p.x() - ox;
    Tempy = p.y() - oy;
    ox = (p.x()/OldZoom + ox) - p.x()/Zoom;//- Tempx * (MapWidth * Zoom) + p.x();
    oy = (p.y()/OldZoom + oy) - p.y()/Zoom;//- Tempx * (MapWidth * Zoom) + p.x();
    updateGL();
    //oy = - Tempy * (MapHeight * Zoom) + p.y();
}

void Drawer::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}
