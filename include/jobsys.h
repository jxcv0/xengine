#ifndef JOBSYS_H
#define JOBSYS_H

#include "checkerr.h"
#include <pthread.h>

#define JOBSYS_CIRCULAR_BUFFER_SIZE 256
#define JOBSYS_NTHREADS 4

namespace
{
    bool _run = true; // does not need to be atomic for single producer
    pthread_t _threads[JOBSYS_NTHREADS];
    int _threadRet[JOBSYS_NTHREADS];
    pthread_mutex_t _m;
    pthread_cond_t _cv;
} // namespace

namespace xen::jobsys
{
    // struct for storing job function and data
    struct Job
    {
        void* (*func)(void*);
        void* data;
    };

    // circular fifo queue
    struct CircularWorkBuffer
    {
        inline void write(Job job)
        {
            _buffer[_write] = job;
            _write = (_write + 1) % JOBSYS_CIRCULAR_BUFFER_SIZE;
            size++;
        }

        inline Job* read()
        {
            auto temp = _read;
            _read = (_read + 1) % JOBSYS_CIRCULAR_BUFFER_SIZE;

            size--;
            return &_buffer[temp];
        }

        // seemed odd to allow access to size
        auto job_count() { return size; }

    private:
        unsigned int _read, _write = 0;
        Job _buffer[JOBSYS_CIRCULAR_BUFFER_SIZE];
        unsigned int size = 0;
    };

    CircularWorkBuffer _workQueue;

    // wait loop for each worker thread
    void* spin(void*)
    {
        while(_run)
        {
            Job *job = nullptr;

            pthread_mutex_lock(&_m);
            pthread_cond_wait(&_cv, &_m);

            if (_workQueue.job_count() > 0) { job = _workQueue.read(); }

            pthread_mutex_unlock(&_m);
            pthread_cond_signal(&_cv);

            if (job) { job->func(job->data); }
        }
        return nullptr;
    }

    // init job system
    void init()
    {
        for (size_t i = 0; i < JOBSYS_NTHREADS; i++)
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
        for (size_t i = 0; i < JOBSYS_NTHREADS; i++)
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
