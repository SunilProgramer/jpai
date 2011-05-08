#ifndef PLAYERCOLOR_H
#define PLAYERCOLOR_H

#include <settingsmanager.h>
#include <QColor>
#include <QMap>


class PlayerColors
{
public:
    PlayerColors();
    static QColor Color(const int &index);
private:
    static QMap<int, QColor> colors;
};

QColor Blend(const QColor &a, const QColor &b, const float &factor);

#endif // PLAYERCOLOR_H
