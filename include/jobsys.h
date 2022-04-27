#ifndef JOBSYS_H
#define JOBSYS_H

#include <list>
#include <thread>
#include <atomic>
#include <vector>

namespace xen
{
	// entry and data for a job
	struct Job
	{
		bool (*entry)(void*);	// pointer to job function - returns true on successful completion, false if the job must be suspended
		void* params;		// job function parameters
	};
	
	// kick a job
	bool kick(Job &job)
	{
		return job.entry(job.params);
	}

	// initialize a threadpool with a thread count and a wait function
	void initThreadPool(std::vector<std::thread> &threadPool, unsigned int nThreads, void (*wait)(void))
	{
		for (size_t i = 0; i < nThreads; i ++)
		{
			threadPool.emplace_back(wait);
		}
	}

	// join all threads in a threadpool
	void joinThreadPool(std::vector<std::thread> &threadPool)
	{
		for (auto &thread : threadPool) { thread.join(); }
	}

	// push a job to the back of a queue
	void pushJob(std::list<Job> &jobQueue, Job &job, std::atomic_flag *lk)
	{
		while (std::atomic_flag_test_and_set(lk));
		jobQueue.push_back(job);
		std::atomic_flag_clear(lk);
	}

	// global scope
	extern std::atomic_flag lk;
	extern std::list<Job> jobQueue;
	extern std::atomic<bool> run;

	// worker thread loop
	void wait()
	{
		Job job;
		while(run)
		{
			while (std::atomic_flag_test_and_set_explicit(&lk, std::memory_order_acquire))
			{
				asm ("pause");
				std::this_thread::yield();
			}

			if (!jobQueue.empty())
			{
				job = jobQueue.front();
				jobQueue.pop_front();

				std::atomic_flag_clear_explicit(&lk, std::memory_order_release);
				if (!kick(job)) { pushJob(jobQueue, job, &lk); };
			}
			else 
			{
				std::atomic_flag_clear_explicit(&lk, std::memory_order_release);
			}
		}
	}
} // namespace xen

#endif // JOBSYS_H
