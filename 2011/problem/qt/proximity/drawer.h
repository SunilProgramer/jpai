#ifndef DRAWER_H
#define DRAWER_H

#include <QtOpenGL>
#include <QGLWidget>

class Drawer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Drawer(QWidget *parent = 0);
    QPainter painter;
    float Zoom;
    float ox, oy, MapWidth, MapHeight;
protected:
    void resizeGL(int w, int h);
    void initializeGL();

    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:

public slots:

};

#endif // DRAWER_H
