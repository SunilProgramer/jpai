#ifndef AIHANDLER_H
#define AIHANDLER_H

#include <QThread>

class AIHandler : public QThread
{
public:
    AIHandler();
protected:
    void run();
};

#endif // AIHANDLER_H
