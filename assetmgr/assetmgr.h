#ifndef ASSETMGR_H_
#define ASSETMGR_H_

#include <semaphore.h>

#define ASSETMGR_SHMPATH "/resrcmgr"
#define MAX_FILENAME_LEN 32
#define MAX_NUM_REQUESTS 8

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Asset types
 *
 */
enum asset_type { asset_type_MESH, asset_type_MATERIAL };

enum assetreq_status {
  assetreq_status_WAITING,
  asetreq_status_READY,
  asssetreq_status_ERROR
};

struct assetreq {
  char filepath[MAX_FILENAME_LEN];
  int status;
};

/**
 * @brief The shared memory structure used to request assets.
 */
struct assetmgr_shm {
  sem_t sem;
  struct assetreq requests[MAX_NUM_REQUESTS];
};

/**
 * @brief Map the shared memory of the asset manager into the callers address
 * space.
 *
 * @return A pointer to the shared assetmgr_shm structure.
 */
struct assetmgr_shm *mmap_assetmgr_shm(void);

/**
 * @brief Initialize the assetmgr shared memory. This should only be called by the assetmgr process.
 * 
 * @return A pointer to the initialized shared memory. Or NULL on error.
 */
struct assetmgr_shm *init_assetmgr_shm(void);

int register_asset(struct assetmgr_shm *shm, const char *filepath);

int load_assets(struct assetmgr_shm *shm, int nassets, int *ids);
int unload_assets(struct assetmgr_shm *shm, int nassets, int *ids);

int assetmgrd_process_requests(struct assetmgr_shm *shm);

/**
 * @brief Get the type of an asset based on a file extension.
 *
 * @param filepath The filepath to extract the asset type from
 * @return The type of the asset with the filepath.
 */
int asset_type(const char *filepath);

#ifdef __cplusplus
}
#endif

#endif  // ASSETMGR_H_
