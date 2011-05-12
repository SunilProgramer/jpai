#include "drawer.h"
#include <algorithm>
#include <QFont>
using namespace std;

Drawer::Drawable::Drawable(bool createdDynamically): CreatedDynamically(createdDynamically), Updated(true), Deleted(false)
{
}

void Drawer::Drawable::CalculateBBox()
{
}

Drawer::Drawer(QWidget *parent) :
    QGLWidget(parent)
{
    MapWidth = 100;
    MapHeight = 100;
    ox = 0;
    oy = 0; // remove this stuff
    DragStarted = false;
    QTimer::singleShot(33, this, SLOT(Update()));

}

Drawer::~Drawer()
{
    mutex.lock();
    while (!objects.isEmpty())
    {
        Drawable *d = objects.front();
        if (d->CreatedDynamically)
        //    delete d; <-- fix this bug
        objects.pop_front();
    }
    mutex.unlock();
}

void Drawer::DrawText(float x, float y, int font_size, QString s)
{
    glPushMatrix();
    glLoadIdentity();
    QFont f;
    f.setPixelSize(Zoom*0.5f); // too edgy
    QFontMetricsF m(f);
    QRectF r = m.boundingRect(s);
    glTranslatef((x+ox)*Zoom - r.width()*0.5f - r.left(), (y+oy)*Zoom + r.height() + r.bottom(), -0.2f);
    renderText(0.0f, 0.0f, 0.0f, s, f);
    glPopMatrix();
}

void Drawer::Add(Drawable *obj)
{
    if (!mutex.tryLock())
    {
        to_add.append(obj);
        return;
    }
    objects.append(obj);
    mutex.unlock();
}

Drawer::Drawable *Drawer::Get(const int &ind)
{
    return objects[ind];
}

void Drawer::Drawable::Update(Drawer *drawer)
{

}

void Drawer::Drawable::Click(Drawer *drawer, float x, float y)
{

}

void Drawer::resizeGL(int w, int h)
{
    h = h?h:!h;

    glViewport(0, 0, w, h);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, w - 1.0f, h - 1.0f, 0.0f, 10.0f, -10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    QRectF r;
    mutex.lock();
    for (QList<Drawable*>::iterator i = objects.begin(); i != objects.end(); ++i)
    {
        (*i)->CalculateBBox();
        r = (i == objects.begin())?(*i)->BB:r.unite((*i)->BB);
    }
    mutex.unlock();
    AddUnadded();
    Zoom = min(width()/r.width(), height()/r.height());
    ox = r.left();
    oy = r.top();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Drawer::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, .5f);
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
    QRect r(ox*Zoom, oy*Zoom, width(), height());
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glScalef(Zoom, Zoom, 1.0f);
    glTranslatef(ox, oy, 0.0f);
    mutex.lock();
    for (QList<Drawable*>::iterator i = objects.begin(); i != objects.end(); ++i)
        (*i)->Draw(this);
    mutex.unlock();
    AddUnadded();
}

void Drawer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::RightButton)
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
    Zoom *= pow(1.2f, event->delta()/120.0f);
    Zoom = clamp(Zoom, 0.2f, 300.0f);//ZOOM_MIN, ZOOM_MAX);
    if (Zoom == OldZoom)
        return;
    ox = -(p.x() - ox*OldZoom)/OldZoom + p.x()/Zoom;
    oy = -(p.y() - oy*OldZoom)/OldZoom + p.y()/Zoom;
    updateGL();
    event->accept();
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
    if (event->button() == Qt::LeftButton)
    {
        QRectF r;
        mutex.lock();
        QList<Drawable*> clicked;
        for (QList<Drawable*>::iterator i = objects.begin(); i != objects.end(); ++i)
        {
            (*i)->CalculateBBox();
            r = (*i)->BB;
            r.setRect((ox + r.left())*Zoom, (oy + r.top())*Zoom, r.width()*Zoom, r.height()*Zoom);
            if (r.contains(event->pos()))
            {
                clicked.push_back(*i);
            }
        }
        mutex.unlock();
        AddUnadded();
        for (QList<Drawable*>::iterator i = clicked.begin(); i != clicked.end(); ++i)
        {
            (*i)->Click(this, event->x()/Zoom - ox, event->y()/Zoom - oy);
        }

    }
    if (event->button() == Qt::MidButton) // Reset zoom
    {
        QRectF r;
        mutex.lock();
        for (QList<Drawable*>::iterator i = objects.begin(); i != objects.end(); ++i)
        {
            (*i)->CalculateBBox();
            r = (i == objects.begin())?(*i)->BB:r.unite((*i)->BB);
        }
        mutex.unlock();
        Zoom = min(width()/r.width(), height()/r.height());
        ox = r.left();
        oy = r.top();
        updateGL();
    }
    if (event->button() != Qt::RightButton || !DragStarted)
        return;
    DragStarted = false;
    ox += (event->x() - sx)/Zoom;
    oy += (event->y() - sy)/Zoom;
    updateGL();
    event->accept();
}

void Drawer::Update()
{
    bool upd = false;
    mutex.lock();
    QList<Drawable*>::iterator j = objects.begin();
    while (j != objects.end())
    {
        if ((*j)->Deleted)
        {
            if ((*j)->CreatedDynamically)
                delete *j;
            j = objects.erase(j);
        }
        else j++;
    }
    for (QList<Drawable*>::iterator i = objects.begin(); i != objects.end(); ++i)
    {
        if (!(*i)->Updated)
        {
            upd = true;
            (*i)->Updated = true;
            (*i)->Update(this);
        }
    }
    mutex.unlock();
    AddUnadded();
    if (upd)
        updateGL();
    QTimer::singleShot(33, this, SLOT(Update()));
}

void Drawer::AddUnadded()
{
    mutex.lock();
    while (!to_add.isEmpty())
    {
        objects.push_back(to_add.front());
        to_add.pop_front();
    }
    mutex.unlock();
}

