#ifndef JOBSYS_H
#define JOBSYS_H

#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>

#define JOBSYS_DEBUG
#define CIRCULAR_BUFFER_SIZE 256

// singleton job system
namespace xen::jobsys
{
    std::atomic<bool> _run = true;
    std::vector<std::thread> _threads;
    std::mutex _m;
    std::condition_variable _cv;

    // struct for storing job function and data
    struct Job
    {
        void (*func)(void*);
        void* data;
    };

    // circular fifo queue
    struct CircularWorkBuffer
    {
        inline void write(Job job)
        {
            _buffer[_write] = job;
            _write = (_write + 1) % CIRCULAR_BUFFER_SIZE;
            size++;

#ifdef JOBSYS_DEBUG
            std::cout << "xen::jobsys::CircularWorkBuffer::write: " << _write << " on " << std::this_thread::get_id() << "\n";
#endif

        }

        inline Job* read()
        {
            auto temp = _read;
            _read = (_read + 1) % CIRCULAR_BUFFER_SIZE;

#ifdef JOBSYS_DEBUG
            std::cout << "xen::jobsys::CircularWorkBuffer::read: " << _read << " on " << std::this_thread::get_id() << "\n"; 
#endif

            size--;
            return &_buffer[temp];
        }

        unsigned int size = 0;
    private:
        unsigned int _read, _write = 0;
        Job _buffer[CIRCULAR_BUFFER_SIZE];
    };

    CircularWorkBuffer _workQueue;

    // wait loop for each worker thread
    void spin()
    {
        while(_run)
        {
            Job *job = nullptr;

            std::unique_lock lk(_m);
            _cv.wait(lk);

            if (_workQueue.size > 0) { job = _workQueue.read(); }

            lk.unlock();
            _cv.notify_one();

            if (job) { job->func(job->data); }
        }
    }

    // init job system
    void init(size_t nThreads = std::thread::hardware_concurrency())
    {
        for (size_t i = 0; i < nThreads; i++)
        {
            std::thread t(spin);
            _threads.push_back(std::move(t));
        }
    }

    // notify and join all threads
    void terminate()
    {
        _run = false;
        for (auto &thread : _threads)
        {
            _cv.notify_one();
            thread.join();
        }
        _threads.clear();
    }

    // push a job to the back of the work queue
    void push(Job job)
    {
        {
            std::lock_guard lk(_m);
            _workQueue.write(job);
        }
        _cv.notify_one();
    }

    // push a job to the back of the work queue
    void push(void(*func)(void*), void* data)
    {
        push(Job{func, data});
    }
} // namespace xen::jobsys

#endif // JOBSYS_H
