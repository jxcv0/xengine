#ifndef ASSETMGR_H_
#define ASSETMGR_H_

#include <semaphore.h>

#define	ASSETMGR_SHMPATH "/resrcmgr"
#define MAX_FILENAME_LEN 32

#ifdef __cplusplus
extern "C" {
#endif

enum asset_type { asset_type_MESH = 0, asset_type_MATERIAL = 1 };

struct assetmgr_shm {
	sem_t sem;
	char filepath[MAX_FILENAME_LEN];
	void *dest;
};

int asset_type(const char *filepath);

#ifdef __cplusplus
}
#endif

#endif  // ASSETMGR_H_
