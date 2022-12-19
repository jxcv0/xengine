#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cstdint>

namespace xen {

/**
 * @brief Entity identifyer.
 */
using entity_id = int;

/**
 * @brief Entity archetype bitmask;
 */
using entity_archetype = int;

/**
 * @brief Max total number of entities.
 */
constexpr int MAX_ENTITIES = 4096;

/**
 * @brief Max total number of components per entity.
 */
constexpr int MAX_COMPONENTS = 32;

}  // namespace xen

#endif  // CONSTANTS_H_
