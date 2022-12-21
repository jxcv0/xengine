#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cstdint>

/**
 * @brief Entity identifyer.
 */
using entity_id = int;

/**
 * @brief Max total number of entities.
 */
constexpr const int MAX_ENTITIES = 4096;

/**
 * @brief Max total number of components per entity.
 */
constexpr const int MAX_COMPONENTS = 32;

/**
 * @brief Max total number of components per entity.
 */
constexpr const int MAX_SYSTEMS =
    MAX_ENTITIES * MAX_COMPONENTS;  // is this enough?

#endif  // CONSTANTS_H_
