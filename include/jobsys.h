#ifndef JOBSYS_H
#define JOBSYS_H

#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>

namespace xen
{
    struct Job
    {
        void operator()(){ func(data); }
        void (*func)(void*);
        void* data;
    };

	struct ThreadPool
	{

        // constructor
        ThreadPool(size_t nThreads = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < nThreads; i++)
			{
                std::thread t([this]{
                    while(_run)
                    {
                        Job job;
                        std::unique_lock lk(_m);
                        _cv.wait(lk);

                        if (!_jobs.empty())
                        {
                            job = _jobs.front();
                            _jobs.erase(_jobs.begin());
                        }

                        lk.unlock();
                        _cv.notify_one();
                        job();
                    }
                });
                _threads.push_back(std::move(t));
			}
		}

        // destructor
		~ThreadPool()
		{
            _run = false;
			for (auto &thread : _threads)
			{
                _cv.notify_one();
				thread.join();
			}
            _threads.clear();
		}

        // kick a job
		void push(Job job)
		{
            {
                std::lock_guard lk(_m);
                _jobs.push_back(job);
            }
            _cv.notify_one();
		}

        // kick a job
		void push(void(*func)(void*), void* data)
		{
            push(Job{func, data});
		}

	private:
        std::atomic<bool> _run = true;
		std::vector<std::thread> _threads;
		std::vector<Job> _jobs;
        std::mutex _m;
        std::condition_variable _cv;
	};
    
} // namespace xen

#endif // JOBSYS_H
