#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "drawer.h"

class ParticleSystem
{
public:
    ParticleSystem();
};

class Particle: public Drawer::Drawable
{
public:
    Particle(int LifeTime, float _x, float _y, float _s, QColor color, float _ds = 0.0f, float _dx = 0.0f, float _dy = 0.0f);
    float x, y, s;
    float dx, dy, ds;
    int TotalLifeTime, Life;
    QColor Color;
    void Draw(Drawer *drawer);
    void Update(Drawer *drawer);
};

#endif // PARTICLESYSTEM_H
