#include "executor.h"

Executor::Executor(const int &RunningTime) : RunTime(RunningTime)
{

}

int Executor::run(const QString &RunDir, const QString &Executable)
{
    setWorkingDirectory(RunDir);
    start(Executable);
    if (!waitForFinished(RunTime))
        close();
    qDebug("%d", exitStatus());
    return exitCode();
}
