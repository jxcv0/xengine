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
    for (archetype_base *b : m_archetypes)
      {
        for (size_t i = 0; i < sizeof...(T); i++)
          {
            // ...
          }
      }
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
  std::vector<archetype_base *> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_H_ */
