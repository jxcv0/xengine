#ifndef XEN_RESRCMGR_H_
#define XEN_RESRCMGR_H_

#include <semaphore.h>

#define	RESRCMGR_SHMPATH "/resrcmgr"
#define MAX_FILENAME_LEN 32

struct reqshmbuf {
	sem_t sem;
	char filepath[MAX_FILENAME_LEN];
	void *dest;
};

#endif  // XEN_RESRCMGR_H_
