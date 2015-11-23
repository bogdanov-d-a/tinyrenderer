#include "renderthread.h"

RenderThread::RenderThread(ModelPtrArray const& models, FrameTile &tile, DrawFunction draw)
    :idle(true)
    ,stopped(false)
    ,models(models)
    ,tile(tile)
    ,draw(draw)
{
    start();
}

void RenderThread::Wake()
{
    Q_ASSERT(!stopped);
    Q_ASSERT(idle);
    idle = false;
}

bool RenderThread::Idle() const
{
    Q_ASSERT(!stopped);
    return idle;
}

void RenderThread::Stop()
{
    Q_ASSERT(!stopped);
    Q_ASSERT(idle);
    stopped = true;
}

void RenderThread::run()
{
    while (true)
    {
        while (idle)
        {
            if (stopped)
                return;
            msleep(10);
        }

        for (auto const& pModel : models)
            draw(*pModel, tile);

        idle = true;
    }
}
