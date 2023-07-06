#include "assets.h"

#include <string.h>

int
asset_type (const char *path)
{
  char *ext = strrchr (path, '.');

  if (ext == NULL)
    {
      return -1;
    }

  const char *supported_asset_exts[] = { ".mesh", ".tex" };
  for (int i = 0; i < 2; i++)
    {
      if (strcmp (supported_asset_exts[i], ext) == 0)
        {
          return i;
        }
    }

  return -1;
}
