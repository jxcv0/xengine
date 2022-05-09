#ifndef RENDER_H
#define RENDER_H

#include <mutex>
#include <condition_variable>

namespace xen
{
    struct RenderThread
    {
        template<template Renderable>
        void operator() (Renderable r)
        {
            r.render();
        }

    private:
        std::mutex _m;
        std::condition_variable _cv;
    };
} // namespace xen
#endif // RENDER_H
