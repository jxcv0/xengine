#ifndef JOBSYS_H
#define JOBSYS_H

#include "checkerr.h"
#include "buffer.h"
#include <pthread.h>


namespace
{
    const size_t NTHREADS = 4;

    bool _jobsysRun = true; // does not need to be atomic for single producer
    pthread_t _jobsysThreads[NTHREADS];
    int _jobsysThreadRet[NTHREADS];
    pthread_mutex_t _jobsysMutex;
    pthread_cond_t _jobsysCond;

    // Job type
    struct Job
    {
        void* (*func)(void*);
        void* data;
    };

    xen::CircularBuffer<Job> _workQueue(256);

    // wait loop for each worker thread
    void* jobsys_main(void*)
    {
        while(_jobsysRun)
        {
            Job *job = nullptr;

            pthread_mutex_lock(&_jobsysMutex);
            pthread_cond_wait(&_jobsysCond, &_jobsysMutex);

            if (_workQueue.size() > 0) { job = _workQueue.readptr(); }

            pthread_mutex_unlock(&_jobsysMutex);
            pthread_cond_signal(&_jobsysCond);

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
            pthread_create(&_jobsysThreads[i], NULL, jobsys_main, (void*)0);
#ifdef XEN_DEBUG
            std::string msg("Created thread: " + std::to_string(_jobsysThreads[i]));
            logmsg(msg.c_str());
#endif
        }
    }

    // notify and join all threads
    void terminate()
    {
        _jobsysRun = false;
        for (size_t i = 0; i < NTHREADS; i++)
        {
            pthread_cond_signal(&_jobsysCond);
            pthread_join(_jobsysThreads[i], NULL);
        }
    }

    // push a job to the back of the work queue
    void push(Job job)
    {
        pthread_mutex_lock(&_jobsysMutex);
        _workQueue.write(job);
        pthread_mutex_unlock(&_jobsysMutex);
        pthread_cond_signal(&_jobsysCond);
    }

    // push a job to the back of the work queue
    void push(void* (*func)(void*), void* data)
    {
        push(Job{func, data});
    }
} // namespace xen::jobsys

#endif // JOBSYS_H
