#ifndef ECS_H_
#define ECS_H_

#include "archetype.h"
#include "entity_mgr.h"
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
    std::uint64_t new_entity = m_mgr.create_entity ();
    const std::size_t ntypes = sizeof...(T);
    for (archetype_base *b : m_archetypes)
      {
        std::size_t hascount = 0;
        if (b->type_count () != ntypes)
          {
            continue;
          }

        (
            [&] {
              if (b->has_type (typeid (T).hash_code ()))
                {
                  ++hascount;
                }
            }(),
            ...);
        if (hascount == ntypes)
          {
            b->add_entity (new_entity);
          }
      }
    return new_entity;
  }

  template <typename... T>
  void
  register_archetype (archetype<T...> *arch)
  {
    /* TODO should check we dont already have one like arch */
    m_archetypes.push_back (arch);
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
