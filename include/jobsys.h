#ifndef JOBSYS_H
#define JOBSYS_H

#include "checkerr.h"
#include "buffer.h"
#include <pthread.h>


namespace
{
    const size_t BUFFER_SIZE = 256;
    const size_t NTHREADS = 4;

    bool _run = true; // does not need to be atomic for single producer
    pthread_t _threads[NTHREADS];
    int _threadRet[NTHREADS];
    pthread_mutex_t _m;
    pthread_cond_t _cv;

    // Job type
    struct Job
    {
        void* (*func)(void*);
        void* data;
    };

    xen::CircularBuffer<Job> _workQueue(BUFFER_SIZE);

    // wait loop for each worker thread
    void* spin(void*)
    {
        while(_run)
        {
            Job *job = nullptr;

            pthread_mutex_lock(&_m);
            pthread_cond_wait(&_cv, &_m);

            if (_workQueue.size() > 0) { job = _workQueue.readptr(); }

            pthread_mutex_unlock(&_m);
            pthread_cond_signal(&_cv);

            if (job) { job->func(job->data); }
        }
        return nullptr;
    }
} // namespace

// all jobs sent to the jobsystem are to be completed in that frame
namespace xen::jobsys
{
    // init job system
    void init()
    {
        for (size_t i = 0; i < NTHREADS; i++)
        {
            pthread_create(&_threads[i], NULL, spin, (void*)0);
#ifdef XEN_DEBUG
            std::string msg("Created thread: " + std::to_string(_threads[i]));
            logmsg(msg.c_str());
#endif
        }
    }

    // notify and join all threads
    void terminate()
    {
        _run = false;
        for (size_t i = 0; i < NTHREADS; i++)
        {
            pthread_cond_signal(&_cv);
            pthread_join(_threads[i], NULL);
        }
    }

    // push a job to the back of the work queue
    void push(Job job)
    {
        pthread_mutex_lock(&_m);
        _workQueue.write(job);
        pthread_mutex_unlock(&_m);
        pthread_cond_signal(&_cv);
    }

    // push a job to the back of the work queue
    void push(void* (*func)(void*), void* data)
    {
        push(Job{func, data});
    }
} // namespace xen::jobsys

#endif // JOBSYS_H
