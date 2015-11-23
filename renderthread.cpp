#include "renderthread.h"

RenderThread::RenderThread(ModelPtrArray const& models, FrameTile &tile, DrawFunction draw)
    :idle(true)
    ,models(models)
    ,tile(tile)
    ,draw(draw)
{
    start();
}

void RenderThread::Wake()
{
    Q_ASSERT(idle);
    idle = false;
}

bool RenderThread::Idle() const
{
    return idle;
}

void RenderThread::run()
{
    while (true)
    {
        while (idle)
            msleep(10);

        for (auto const& pModel : models)
            draw(*pModel, tile);

        idle = true;
    }
}
