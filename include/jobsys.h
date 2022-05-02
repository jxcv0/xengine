#ifndef JOBSYS_H
#define JOBSYS_H

#include <list>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace xen::jobs
{
	// entry and data for a job
	struct Job
	{
		int (*entry)(void*);	// pointer to job function
		void* params;		// job function parameters
	};

	// kick a job
	bool kick(Job &job)
	{
		return job.entry(job.params);
	}

	// initialize a threadpool with a thread count and a wait function
	void init(std::vector<std::thread> &threadPool, size_t nThreads, void (*spin)(void))
	{
		for (size_t i = 0; i < nThreads; i++)
		{
			threadPool.emplace_back(spin);
		}
	}

	// join all threads in a threadpool
	void join(std::vector<std::thread> &threadPool)
	{
		for (auto &thread : threadPool) { thread.join(); }
	}

	// spin on a lock
	void acquire(std::atomic_flag *lk)
	{
		// test
		while (lk->test())
		{
			asm ("pause");
			std::this_thread::yield();
		}

		// test and set
		while (std::atomic_flag_test_and_set_explicit(lk, std::memory_order_acquire))
		{
			asm ("pause");
			std::this_thread::yield();
		}
	}

	// release lock
	void release(std::atomic_flag *lk)
	{
		std::atomic_flag_clear_explicit(lk, std::memory_order_release);
	}

	// push a job to the back of a queue
	void push_job(std::list<Job> &jobQueue, const Job &job, std::atomic_flag *lk)
	{
		acquire(lk);
		jobQueue.push_back(job); release(lk);
	}

	// singleton job system manager
    // TODO - allocator for std::function<> and std::packaged_task<>
	struct JobSystemMgr
	{
        JobSystemMgr() { /* do nothing */ }
        ~JobSystemMgr() { /* do nothing */ }

		// start up
		void start_up(size_t nThreads = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < nThreads; i++)
			{
				std::thread t([&]{
					while (_run)
					{
						std::unique_lock lk(_m);
						_cv.wait(lk);

						if (!_jobs.empty())
						{
							auto job = _jobs.front();
							_jobs.pop_front();
							lk.unlock();
							_cv.notify_one();
							job();
						}
						else
						{
							lk.unlock();
							_cv.notify_one();
						}
					}
				});
				_threads.push_back(std::move(t));
			}
		}

		// shut down
		void shut_down()
		{
			_run = false;
			for (auto &thread : _threads)
			{
				thread.join();
			}
		}

		void push_job(std::function<void(void)> job)
		{
			{
				std::lock_guard lk(_m);
				_jobs.push_back(job);
			}
			_cv.notify_one();
		}

        bool empty() { return _jobs.empty(); }

	private:
		std::vector<std::thread> _threads;
		std::atomic<bool> _run = true;
		std::list<std::function<void(void)>> _jobs;
		std::mutex _m;
		std::condition_variable _cv;
	};
} // namespace xen::jobs

#endif // JOBSYS_H
