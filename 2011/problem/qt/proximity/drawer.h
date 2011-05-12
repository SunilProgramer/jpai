#ifndef DRAWER_H
#define DRAWER_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QList>
#include <QMutex> // maybe get rid of this

class Drawer : public QGLWidget
{
    Q_OBJECT
public:
    class Drawable
    {
    public:
        Drawable(bool createdDynamically = true);
        virtual void Draw(Drawer *drawer) = 0;
        virtual void Update(Drawer *drawer);
        virtual void Click(Drawer *drawer, float x, float y);
    protected:
        friend class Drawer;
        virtual void CalculateBBox();
        QRectF BB;
        bool CreatedDynamically;
        bool Updated;
        bool Deleted;
    };
    explicit Drawer(QWidget *parent = 0);
    ~Drawer();
    void DrawText(float x, float y, int font_size, QString s);
    template<typename T>
    T *Add();
    void Add(Drawable *obj);
    Drawable *Get(const int &ind);

    float Zoom;
    float ox, oy, MapWidth, MapHeight;
    int sx, sy;
    bool DragStarted;
public slots:
    void Update();
    void ResetZoom();
protected:
    QMutex mutex;
    QList<Drawable*> objects;
    QList<Drawable*> to_add;
    void resizeGL(int w, int h);
    void initializeGL();
    void paintGL();
    void AddUnadded();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

template<typename T>
T *Drawer::Add()
{
    T *res = new T;
    Add(res);
    return res;
}

#endif // DRAWER_H
