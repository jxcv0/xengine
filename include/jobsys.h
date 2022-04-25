#ifndef JOBSYS_H
#define JOBSYS_H

#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

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
		for (auto &thread : threadPool) { thread.join(); };
	}

	// global variables
	extern std::mutex m;
	extern std::condition_variable cv;
	extern std::list<Job> jobQueue;

	// push a job to the back of a queue
	void pushJob(Job &job)
	{
		{
			std::lock_guard lk(m);
			jobQueue.push_back(job);
		}
		cv.notify_one();
	}

	// wait on work from producer thread
	extern bool run;
	void wait()
	{
		Job job;
		while (run)
		{
			std::unique_lock lk(m);
			cv.wait(lk, [&]{ return !jobQueue.empty(); });
			job = jobQueue.front();
			jobQueue.pop_front();

			lk.unlock();
			cv.notify_one();

			if (!kick(job)) { pushJob(job); };
		}
	}
} // namespace xen

#endif // JOBSYS_H
