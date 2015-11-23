#include <vector>
#include <limits>
#include <iostream>
#include "tgaimage.h"
#include "geometry.h"
#include "our_gl.h"
#include "shader.h"
#include "sdlwindow.h"
#include <SDL2/SDL.h>
#include "renderthread.h"

const int WIDTH  = 800;
const int HEIGHT = 800;

Vec3f LIGHT_DIR(1,1,1);
Vec3f       EYE(1,1,3);
Vec3f    CENTER(0,0,0);
Vec3f        UP(0,1,0);

Vec3f get_rotated_eye()
{
    float secondsSinceStart = 0.001f * float(SDL_GetTicks());
    float angle = 0.5f * secondsSinceStart;
    Matrix rotation = Matrix::identity();
    rotation[0][0] = cos(angle);
    rotation[1][0] = -sin(angle);
    rotation[1][1] = cos(angle);
    rotation[0][1] = sin(angle);
    Vec4f rotated = rotation * embed<4>(EYE, 1.f);
    return proj<3>(rotated);
}

void draw_3d_model_tile(Model &model, FrameTile &frame)
{
    Shader shader;
    shader.setLightDirection(LIGHT_DIR);
    shader.pModel = &model;
    for (int i=0; i<model.nfaces(); i++) {
        for (int j=0; j<3; j++) {
            shader.vertex(i, j);
        }
        triangle(shader.varying_tri, shader, frame);
    }
}

void draw_3d_model_simple(TGAImage &frame, float *zbuffer, std::vector<FrameTile> &tiles, std::vector<std::unique_ptr<RenderThread>> &threads)
{
    for (auto &tile : tiles)
        tile.init(frame, zbuffer);

    for (auto &thread : threads)
        thread->Wake();

    while (true)
    {
        bool end = true;
        for (auto &thread : threads)
        {
            if (!thread->Idle())
                end = false;
        }

        if (end)
            break;

        QThread::msleep(10);
    }
}

int qMain(int argc, char** argv) {
    if (2>argc) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }
    ModelPtrArray models;
    for (int m=1; m<argc; m++) {
        const char *filePath = argv[m];
        models.push_back(std::make_shared<Model>(filePath));
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    std::unique_ptr<float[]> zbuffer(new float[WIDTH*HEIGHT]);

    SDLWindow window(WIDTH, HEIGHT);
    std::shared_ptr<TGAImage> pFrame;
    window.swapBuffers(pFrame);

    const int width1 = pFrame->get_width() / 2;
    const int width2 = pFrame->get_width() - width1;
    const int height1 = pFrame->get_height() / 2;
    const int height2 = pFrame->get_height() - height1;

    std::vector<FrameTile> tiles;
    tiles.push_back(FrameTile(Vec2i(0, 0), Vec2i(width1, height1)));
    tiles.push_back(FrameTile(Vec2i(width1, 0), Vec2i(width2, height1)));
    tiles.push_back(FrameTile(Vec2i(0, height1), Vec2i(width2, height2)));
    tiles.push_back(FrameTile(Vec2i(width1, height1), Vec2i(width2, height2)));

    std::vector<std::unique_ptr<RenderThread>> threads;
    for (auto &tile : tiles)
        threads.push_back(std::unique_ptr<RenderThread>(new RenderThread(models, tile, draw_3d_model_tile)));

    window.do_on_idle([&]() {
        pFrame->clear();
        for (int i=WIDTH*HEIGHT; i--; zbuffer[i] = -std::numeric_limits<float>::max());
        Vec3f eye = get_rotated_eye();
        lookat(eye, CENTER, UP);
        viewport(WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
        projection(-1.f/(eye-CENTER).norm());
        draw_3d_model_simple(*pFrame, zbuffer.get(), tiles, threads);
        pFrame->flip_vertically(); // to place the origin in the bottom left corner of the image
        window.swapBuffers(pFrame);
    });
    window.show();
    window.wait_for_closed();

    for (auto &thread : threads)
        thread->Stop();

    while (true)
    {
        bool end = true;
        for (auto &thread : threads)
        {
            if (thread->isRunning())
                end = false;
        }

        if (end)
            break;

        QThread::msleep(10);
    }

    return 0;
}

