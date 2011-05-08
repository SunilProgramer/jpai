#include "playercolor.h"
#include "definition.h"
#include <algorithm>
#include <math.h>
#include <QRegExp>
QMap<int, QColor> PlayerColors::colors;

PlayerColors::PlayerColors()
{
}

QColor StringToColor(const QString &s)
{
    static QRegExp r("^\\s*(\\d{1,3})\\s*\\,\\s*(\\d{1,3})\\s*\\,\\s*(\\d{1,3})\\,\\s*(\\d{1,3})");
    if (!r.exactMatch(s))
        return QColor("white");
    QStringList l = r.capturedTexts();
    QColor res(l[1].toInt(), l[2].toInt(), l[3].toInt(), l[4].toInt());
    return res;
}

QString ColorToString(const QColor &c)
{
    return QString::number(c.red(), 10) + "," + QString::number(c.green(), 10) + "," + QString::number(c.blue(), 10) + "," + QString::number(c.alpha(), 10);
}

float normex(float f)
{
    while (f < 0.0f)
        f += 1.0f;
    while (f > 1.0f)
        f -= 1.0f;
    return f;
}

QColor GetColor(const int &ind)
{
    int i = (ind)%24 - 7, s = 3;
    while (i >= 0)
    {
        s *= 2;
        i -= s;
    }
    while (i < 0) i+= s;
    return QColor::fromHsvF(normex(1 - (float)(i%s + ((ind)%24 > 3)*0.5f)/(float)s), 1.0f*(ind!=0), 1.0f/pow(2, (ind!=24)*ind/24)/*/(pow(2, (ind>10)*ind/10.0f))*/, 1.0f);//
}

QColor PlayerColors::Color(const int &index)
{
    int i = std::max(index, 0);
    if (colors.find(i) == colors.end())
    {
        SettingsManager *sm = SettingsManager::Instance();
        colors[i] = StringToColor(sm->getValue(PLAYER_COLORS, QString::number(index, 10), ColorToString(GetColor(index))).toString());
    }
    return colors[i];
}

QColor Blend(const QColor &a, const QColor &b, const float &factor)
{
    return QColor::fromRgbF(a.redF()*(1-factor) + b.redF()*factor,
                            a.greenF()*(1-factor) + b.greenF()*factor,
                            a.blueF()*(1-factor) + b.blueF()*factor,
                            a.alphaF()*(1-factor) + b.alphaF()*factor);
}
