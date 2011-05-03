#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QProcess>

//Executor - thx cap

class Executor : public QProcess
{
public:
    Executor(const int &RunningTime = -1);
    int run(const QString &RunDir, const QString &Executable);
private:
    int RunTime;
};

#endif // EXECUTOR_H
