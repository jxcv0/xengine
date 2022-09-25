#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "mesh.h"

/**
 * @brief A Model is a collection of meshes.
 */
struct Model {
  std::vector<Mesh> m_meshes;
};

#endif // MODEL_H_
