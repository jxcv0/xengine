#ifndef FILESYS_H
#define FILESYS_H

#include <string>
#include <pthread.h>
#include "buffer.h"

namespace
{
    struct Request
    {
        int (*loadFunc)(const char*);
        std::string filepath;
    };

    xen::CircularBuffer<Request> _reqQueue(256);

    bool _filesysRun = true;
    pthread_t _filesysThread;
    int _filesysThreadRet;
    pthread_mutex_t _filesysMutex;
    pthread_cond_t _filesysCond;

    void* filesys_main(void*)
    {
        while(_filesysRun)
        {
            Request* req = nullptr;

            pthread_mutex_lock(&_filesysMutex);
            pthread_cond_wait(&_filesysCond, &_filesysMutex);

            if (_reqQueue.size() > 0) { req = _reqQueue.readptr(); }

            pthread_mutex_unlock(&_filesysMutex);
            pthread_cond_signal(&_filesysCond);

            if (req) { req->loadFunc(req->filepath.c_str()); }
        }
        return nullptr;
    }
} // namespace

namespace xen::filesys
{
    // init file system
    void init()
    {
        pthread_create(&_filesysThread, NULL, filesys_main, (void*)0);
    }

    // async
} // namespace xen::filesys

#endif // FILESYS_H
