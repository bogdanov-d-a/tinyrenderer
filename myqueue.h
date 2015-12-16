#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <QQueue>
#include <QMutex>

template <typename T>
class MyQueue
{
public:
    void Enqueue(T const& elem)
    {
        mutex.lock();
        queue.enqueue(elem);
        mutex.unlock();
    }

    bool TryDequeue(T& elem)
    {
        mutex.lock();

        bool result = !queue.empty();
        if (result)
        {
            elem = queue.dequeue();
        }

        mutex.unlock();
        return result;
    }

private:
    QQueue<T> queue;
    QMutex mutex;
};

#endif // MYQUEUE_H
