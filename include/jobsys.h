#ifndef JOBSYS_H
#define JOBSYS_H

#include <list>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "alloc.h"

namespace xen
{
    // some experimenting
    template<typename T>
    concept SyncType = requires(T t)
    {
        { t.lock() };
        { t.release() };
    }; 

    // spinlock based synchronization policy
    struct Spinlock
    {
        void lock()
        {
            while (_lk.test()) { __builtin_ia32_pause(); }
            while (std::atomic_flag_test_and_set_explicit(&_lk, std::memory_order_acquire)) { __builtin_ia32_pause(); }
        }

        void release()
        {
            std::atomic_flag_clear_explicit(&_lk, std::memory_order_release);
        }

    private:
        std::atomic_flag _lk = ATOMIC_FLAG_INIT;
    };

    struct MutexLock
    {
        void lock()
        {
            _lk.lock();
            _cv.wait(_lk);
        }

        void release()
        {
            _lk.unlock();
            _cv.notify_one();
        }

    protected:
        std::mutex _m;
        std::unique_lock<std::mutex> _lk = std::unique_lock(_m);
        std::condition_variable _cv;
    };

	// singleton job system manager
    template<SyncType S = Spinlock>
	struct JobSystemMgr : public S
	{
        JobSystemMgr() = default;

		// start up
		void start_up(size_t nThreads = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < nThreads; i++)
			{
				std::thread t([&]{
					while (_run)
					{
                        this->lock();

						if (!_jobs.empty())
						{
							auto job = _jobs.front();
							_jobs.pop_front();

                            this->release();
							job();
						}
						else
						{
                            this->release();
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

        // push a job to the back of the jobsystem
		void push_job(std::function<void(void)> job)
		{
            this->lock();
            _jobs.push_back(job);
            this->release();
		}

        bool empty() { return _jobs.empty(); }

	private:
		std::vector<std::thread> _threads;
		std::atomic<bool> _run = true;
		std::list<std::function<void(void)>> _jobs;
	};

    // template specialization to avoid deadlock in the case of JobSystemMgr<MutexLock>
    template<>
    void JobSystemMgr<MutexLock>::shut_down()
    {
        _run = false;
        for (auto &thread : _threads)
        {
            this->_cv.notify_one();
            thread.join();
        }
    }

    // template specialization to avoid deadlock in the case of JobSystemMgr<MutexLock>
    template<>
    void JobSystemMgr<MutexLock>::push_job(std::function<void(void)> job)
    {
        {
            std::lock_guard(this->_lk);
            _jobs.push_back(job);
        }
        this->_cv.notify_one();
    }
} // namespace xen

#endif // JOBSYS_H
