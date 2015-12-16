#include "mymanager.h"

MyManager::MyManager(int workerCnt)
    :hasUnmanagedTasks(false)
    ,needStop(false)
{
    for (int i = 0; i < workerCnt; ++i)
    {
        workers.push_back(std::move(std::unique_ptr<MyWorker>(new MyWorker())));
    }

    start();
}

MyManager::~MyManager()
{
    needStop = true;
    wait();
}

void MyManager::AddTask(const std::function<void()> &task)
{
    hasUnmanagedTasks = true;
    tasks.Enqueue(task);
}

bool MyManager::Idle() const
{
    if (hasUnmanagedTasks)
    {
        return false;
    }

    for (auto &worker: workers)
    {
        if (worker->Busy())
        {
            return false;
        }
    }

    return true;
}

void MyManager::WaitForIdle() const
{
    while (!Idle())
    {
        msleep(10);
    }
}

MyWorker *MyManager::TryGetWorker() const
{
    for (auto &worker : workers)
    {
        if (!worker->Busy())
        {
            return worker.get();
        }
    }

    return nullptr;
}

MyWorker *MyManager::GetWorker() const
{
    MyWorker *worker = nullptr;

    while (!worker)
    {
        worker = TryGetWorker();
        msleep(10);
    }

    return worker;
}

void MyManager::run()
{
    std::function<void()> task;

    while (true)
    {
        if (tasks.TryDequeue(task))
        {
            GetWorker()->Assign(task);
        }
        else
        {
            hasUnmanagedTasks = false;

            if (needStop)
            {
                return;
            }

            msleep(10);
        }
    }
}
