#ifndef MYMANAGER_H
#define MYMANAGER_H

#include <QThread>
#include <vector>
#include "myworker.h"
#include <memory>
#include "myqueue.h"

class MyManager : public QThread
{
public:
    MyManager(int workerCnt);
    ~MyManager();
    void AddTask(std::function<void()> const& task);
    bool Idle() const;
    void WaitForIdle() const;

private:
    bool hasUnmanagedTasks, needStop;
    std::vector<std::unique_ptr<MyWorker>> workers;
    MyQueue<std::function<void()>> tasks;

    MyWorker* TryGetWorker() const;
    MyWorker* GetWorker() const;

protected:
    void run();
};

#endif // MYMANAGER_H
