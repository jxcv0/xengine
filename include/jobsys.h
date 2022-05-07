#ifndef JOBSYS_H
#define JOBSYS_H

#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <type_traits>

namespace xen
{
	struct ThreadPool
	{
        ThreadPool(size_t nThreads = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < nThreads; i++)
			{
                std::thread t([this]{
                    std::function<void(void)> job = nullptr;
                    while(_run)
                    {
                        std::unique_lock lk(_m);
                        _cv.wait(lk);

                        if (!_jobs.empty())
                        {
                            job = _jobs.front();
                            _jobs.erase(_jobs.begin());
                        }

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
                _cv.notify_one();
				thread.join();
			}
            _threads.clear();
		}

		void push(std::function<void(void)>job)
		{
            {
                std::lock_guard lk(_m);
                _jobs.push_back(job);
            }
            _cv.notify_one();
		}

	private:
        std::atomic<bool> _run = true;
		std::vector<std::thread> _threads;
		std::vector<std::function<void(void)>> _jobs;
        std::mutex _m;
        std::condition_variable _cv;
	};
} // namespace xen

#endif // JOBSYS_H
