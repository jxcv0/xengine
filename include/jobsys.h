#ifndef JOBSYS_H
#define JOBSYS_H

#include <list>
#include <thread>
#include <atomic>

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
			std::cout << std::this_thread::get_id() << "\n";
			std::cout << "locking...\n";
			while (std::atomic_flag_test_and_set_explicit(&lk, std::memory_order_acquire)) { std::this_thread::yield(); }

			if (!jobQueue.empty())
			{
				std::cout << "getting job...\n";
				job = jobQueue.front();
				jobQueue.pop_front();

				std::cout << "unlocking..\n";
				std::atomic_flag_clear_explicit(&lk, std::memory_order_release);
				std::cout << "kicking...\n";
				if (!kick(job)) { std::cout << "returning job...\n"; pushJob(jobQueue, job, &lk); };
			}
			else 
			{
				std::cout << "unlocking..\n";
				std::atomic_flag_clear_explicit(&lk, std::memory_order_release);
			}
		}
		std::cout << "exiting...\n";
	}
} // namespace xen

#endif // JOBSYS_H
