#ifndef JOBSYS_H
#define JOBSYS_H

#include <queue>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace xen
{
	// singleton job system manager
	struct ThreadPool
	{
        ThreadPool(size_t nThreads = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < nThreads; i++)
			{
                std::thread t([this](){
                    std::function<void(void)> job = nullptr;
                    for(;;)
                    {
                        std::unique_lock lk(_m);
                        _cv.wait(lk, [this, &job]{ return !_jobs.empty() || _run; });
                        if (!_run) { return; }
                        if (_jobs.empty()) { return; }
                        job = _jobs.front();
                        _jobs.pop();
                        lk.unlock();
                        _cv.notify_one();
                        if (job)
                        {
                            job();
                            job = nullptr;
                        }
                    }
                });
                _threads.push_back(std::move(t));
			}
		}

		~ThreadPool()
		{
			_run = false;
			for (auto &thread : _threads)
			{
				thread.join();
			}
		}

        // push a job to the back of the jobsystem
		void operator() (const std::function<void(void)> job)
		{
            {
                std::lock_guard lk(_m);
                _jobs.push(job);
            }
            _cv.notify_one();
		}

	private:
		std::vector<std::thread> _threads;
		std::atomic<bool> _run = true;
		std::queue<std::function<void(void)>> _jobs;
        std::mutex _m;
        std::condition_variable _cv;
	};
} // namespace xen

#endif // JOBSYS_H
