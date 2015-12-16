#include "myworker.h"

MyWorker::MyWorker()
    :taskSet(false)
    ,needStop(false)
{
    start();
}

MyWorker::~MyWorker()
{
    needStop = true;
    wait();
}

void MyWorker::Assign(const std::function<void()> &task)
{
    Q_ASSERT(!taskSet);
    this->task = task;
    taskSet = true;
}

bool MyWorker::Busy() const
{
    return taskSet;
}

void MyWorker::run()
{
    while (true)
    {
        if (taskSet)
        {
            task();
            taskSet = false;
        }
        else
        {
            if (needStop)
            {
                return;
            }
        }

        msleep(10);
    }
}
