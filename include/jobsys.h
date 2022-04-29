#ifndef JOBSYS_H
#define JOBSYS_H

#include <list>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

namespace xen::jobsys
{

	// job exit status
	enum ExitStatus { SUCCESS, PAUSE };
	
	// entry and data for a job
	struct Job
	{
		ExitStatus (*entry)(void*);	// pointer to job function
		void* params;		// job function parameters
	};
	
	// kick a job
	bool kick(Job &job)
	{
		return job.entry(job.params);
	}

	// initialize a threadpool with a thread count and a wait function
	void initThreadPool(std::vector<std::thread> &threadPool, unsigned int nThreads, void (*spin)(void))
	{
		for (size_t i = 0; i < nThreads; i++)
		{
			threadPool.emplace_back(spin);
		}
	}

	// join all threads in a threadpool
	void joinThreadPool(std::vector<std::thread> &threadPool)
	{
		for (auto &thread : threadPool) { thread.join(); }
	}

	// push a job to the back of a queue
	void pushJob(std::list<Job> &jobQueue, const Job &job, std::atomic_flag *lk)
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

		jobQueue.push_back(job);
		std::atomic_flag_clear_explicit(lk, std::memory_order_release);
	}

	// global scope
	extern std::atomic_flag GLOBAL_LK;
	extern std::list<Job> GLOBAL_JOB_QUEUE;
	extern std::atomic<bool> GLOBAL_RUN;

	// worker thread loop
	void spin()
	{
		Job job;
		while(GLOBAL_RUN)
		{
			// test
			while (GLOBAL_LK.test())
			{
				asm ("pause");	// does this work before a yield()?
				std::this_thread::yield();
			}

			// test and set
			while (std::atomic_flag_test_and_set_explicit(&GLOBAL_LK, std::memory_order_acquire))
			{
				asm ("pause");
				std::this_thread::yield();
			}

			if (!GLOBAL_JOB_QUEUE.empty())
			{
				job = GLOBAL_JOB_QUEUE.front();
				GLOBAL_JOB_QUEUE.pop_front();

				std::atomic_flag_clear_explicit(&GLOBAL_LK, std::memory_order_release);
				if (kick(job) == PAUSE) { pushJob(GLOBAL_JOB_QUEUE, job, &GLOBAL_LK); }
			}
			else 
			{
				std::atomic_flag_clear_explicit(&GLOBAL_LK, std::memory_order_release);
			}
		}
	}

	// construct a job from a function and data
	constexpr Job makeJob(ExitStatus (*func)(void*), void* data)
	{
		Job j = { func, data };
		return j;
	}
} // namespace xen::jobsys

#endif // JOBSYS_H
