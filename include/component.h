#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <cstdint>
#include "entitysubsystem.h"

/**
 * @brief Components are identified by a unique id
 */
using cid_t = std::uint32_t;

template<typename Component>
class ComponentArray {
public:
private:
    cid_t m_id;
    std::array<Component, MAX_COMPONENTS> m_components;
};

#endif // COMPONENT_H_
