#ifndef MYWORKER_H
#define MYWORKER_H

#include <QThread>
#include <functional>

class MyWorker : public QThread
{
public:
    MyWorker();
    ~MyWorker();
    void Assign(std::function<void()> const& task);
    bool Busy() const;

private:
    std::function<void()> task;
    bool taskSet, needStop;

protected:
    void run();
};

#endif // MYWORKER_H
