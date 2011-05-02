#include "drawer.h"
#include <algorithm>
using namespace std;

Drawer::Drawer(QWidget *parent) :
    QGLWidget(parent)
{
    MapWidth = 100;
    MapHeight = 100;
    ox = 0;
    oy = 0; // remove this stuff
    DragStarted = false;

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
    Zoom = min(width()/MapWidth, height()/MapHeight);//only on init!

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
    if (DragStarted)
    {
    QPoint p = mapFromGlobal(QCursor::pos());
    ox += (p.x() - sx)/Zoom;
    oy += (p.y() - sy)/Zoom;
    sx = p.x();
    sy = p.y();
    }
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glScalef(Zoom, Zoom, 1.0f);
        glTranslatef(ox, oy, 0.0f);
        glLineWidth(3);
        for (int i = 0; i < MapWidth; i++)
            for (int j = 0; j < MapHeight; j++)
            {
                glBegin(GL_LINE_LOOP);
                for (int k = 0; k < 6; k++)
                {
                    glVertex3f( i + 0.5f*(1+j%2) + 0.5f*sin((float)3.1416*k/3.0f), j + 0.5f + 0.5f*cos((float)3.1416*k/3.0f), 0.0f);//define pi
                }
                glEnd();
            }
//                glColor3f(1.0f, 0,0);
//                glColor3f(0, 1.0f,0);
//                glVertex3f(MapWidth, 0, 0.0f);
//                glColor3f(0, 0,1.0f);
//                glVertex3f( MapWidth,MapHeight, 0.0f);
}

void Drawer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;
    DragStarted = true;
    sx = event->x();
    sy = event->y();
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
    Zoom = clamp(Zoom, 0.2f, 200.0f);//ZOOM_MIN, ZOOM_MAX);
    if (Zoom == OldZoom)
        return;
    ox = -(p.x() - ox*OldZoom)/OldZoom + p.x()/Zoom;
    oy = -(p.y() - oy*OldZoom)/OldZoom + p.y()/Zoom;
    updateGL();
    event->accept();
    //oy = - Tempy * (MapHeight * Zoom) + p.y();
}

void Drawer::mouseMoveEvent(QMouseEvent *event)
{
    if (!DragStarted)
        return;
    QPoint p = mapFromGlobal(QCursor::pos());
    ox += (p.x() - sx)/Zoom;
    oy += (p.y() - sy)/Zoom;
    sx = p.x();
    sy = p.y();
    updateGL();
    event->accept();
}

void Drawer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) // Reset zoom
    {
        ox = oy = 0.0f;
        Zoom = min(width()/MapWidth, height()/MapHeight);
        updateGL();
    }
    if (event->button() != Qt::LeftButton || !DragStarted)
        return;
    DragStarted = false;
    ox += (event->x() - sx)/Zoom;
    oy += (event->y() - sy)/Zoom;
    updateGL();
    event->accept();
}


