#ifndef ASSETMGR_H_
#define ASSETMGR_H_

#include <semaphore.h>
#include "asset_types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum req_type
{
  req_type_REGISTER,
  req_type_LOAD,
  req_type_UNLOAD
};

/**
 * @brief Get the type of an asset based on a file extension.
 *
 * @param filepath The filepath to extract the asset type from
 * @return The type of the asset with the filepath.
 */
int asset_type (const char *filepath);

#ifdef __cplusplus
}
#endif

#endif // ASSETMGR_H_
