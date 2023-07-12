#ifndef ECS_H_
#define ECS_H_

#include "entity_mgr.h"
#include "archetype.h"
#include <memory>

namespace xen
{

class ecs
{
public:
  ecs () = default;

  template <typename... T>
  std::uint64_t
  create_entity ()
  {
    return m_mgr.create_entity ();
  }

  void
  delete_entity (std::uint64_t entity)
  {
    m_mgr.delete_entity (entity);
    /* TODO: delete from archetypes */
  }

private:
  entity_mgr m_mgr;
};

} /* end of namespace xen */

#endif /* ECS_H_ */
