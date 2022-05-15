#ifndef RENDER_H
#define RENDER_H

#include <glad.h>

#include <pthread.h>

namespace
{
    int _thread;
    pthread_mutex_t _m;
    pthread_cond_t _cv;
} // namespace

namespace xen::render
{
} // namespace xen::render

#endif // RENDER_H
