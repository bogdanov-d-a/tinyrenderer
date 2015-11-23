#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include "model.h"
#include <memory>
#include "frametile.h"

typedef std::shared_ptr<Model> ModelPtr;
typedef std::vector<ModelPtr> ModelPtrArray;

class RenderThread : public QThread
{
public:
    typedef void (*DrawFunction)(Model &model, FrameTile &frame);

    RenderThread(ModelPtrArray const& models, FrameTile &tile, DrawFunction draw);
    void Wake();
    bool Idle() const;
    void Stop();

protected:
    void run();

private:
    bool idle, stopped;
    ModelPtrArray const& models;
    FrameTile &tile;
    DrawFunction draw;
};

#endif // RENDERTHREAD_H
