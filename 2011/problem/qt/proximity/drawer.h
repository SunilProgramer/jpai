#ifndef DRAWER_H
#define DRAWER_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QList>

class Drawer : public QGLWidget
{
    Q_OBJECT
public:
    class Drawable
    {
    public:
        Drawable(bool createdDynamically = true);
        virtual void Draw(Drawer *drawer) = 0;
    protected:
        friend class Drawer;
        QRectF BB;
        bool CreatedDynamically;
    };
    explicit Drawer(QWidget *parent = 0);
    ~Drawer();
    QPainter painter;
    float Zoom;
    float ox, oy, MapWidth, MapHeight;
    int sx, sy;
    bool DragStarted;
    void Add(Drawable *obj);
    Drawable *Get(const int &ind);
protected:
    QList<Drawable*> objects;
    void resizeGL(int w, int h);
    void initializeGL();

    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:

public slots:

};

#endif // DRAWER_H
