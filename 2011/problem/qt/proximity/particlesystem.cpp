#include "particlesystem.h"
#include "settingsmanager.h"
#include "definition.h"

ParticleSystem::ParticleSystem()
{
}

Particle::Particle(int LifeTime, float _x, float _y, float _s, QColor color, float _ds, float _dx, float _dy):
        Drawer::Drawable(), TotalLifeTime(LifeTime), x(_x), y(_y), s(_s), Color(color), ds(_ds), dx(_dx), dy(_dy), Life(0)
{
    Updated = false;
}

void Particle::Update(Drawer *drawer)
{
    Life += 33;
    if (Life >= TotalLifeTime)
        Deleted = true;
    x += dx; y += dy; s += ds;
    Updated = false;
}

void Particle::Draw(Drawer *drawer)
{
    float r = s*drawer->Zoom;
    bool flag = SettingsManager::Instance()->getValue(GRAPHICS_SETTINGS, COOL_PARTICLES, true).toBool();
    if (flag)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(Color.redF(), Color.greenF(), Color.blueF(), Color.alphaF()*sqrt(1.0f-(float)Life/TotalLifeTime));
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, -0.5f);
    for (int i = 0; i <= 16; i++)
    {
        if (flag)
            glColor4f(0.0f, 0.0f, 0.0f, 0.0f);// second
        else
            glColor4f(Color.redF(), Color.greenF(), Color.blueF(), 0.0f);// first
        glVertex3f(x + s*cos(3.1415f*i/8.0f), y + s*sin(3.1415f*i/8.0f), -0.5f);
    }
    glEnd();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
