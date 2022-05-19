#ifndef FILESYS_H
#define FILESYS_H

#include <pthread.h>

namespace
{
    struct
    {
        int (*loadFunc)(const char*);
    } _requests[256];

    bool run = true;
    pthread_t _thread;
    int _threadRet;
    // synchronization?

    void spin()
    {
    }
} // namespace

namespace xen::filesys
{
    // init file system
    void init()
    {
        pthread_create(&_thread, NULL, spin
    }
} // namespace xen::filesys

#endif // FILESYS_H
