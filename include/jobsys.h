#ifndef JOBSYS_H
#define JOBSYS_H

namespace xen
{
	// job priority
	// TODO cant these be more specific? FRAME, DELAYABLE
	enum class Priority
	{
		HIGH, NORMAL, LOW
	};

	// entry and data for a job
	struct Job
	{
		void (*entry)(void*);	// pointer to job function
		void* params;		// job function parameters
		Priority priority;
	};

	// kick a job
	void kickJob(Job &job)
	{
		job.entry(params);
	}
} // namespace xen

#endif // JOBSYS_H
