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
    archetype_base *arch = get_archetype<T...> ();
    if (arch != nullptr)
      {
        arch->add_entity (new_entity);
      }
    return new_entity;
  }

  template <typename... T>
  void
  register_archetype (archetype<T...> *arch)
  {
    archetype_base *a = get_archetype<T...> ();
    if (a != nullptr)
      {
        return;
      }
    m_archetypes.push_back (arch);
  }

  void
  delete_entity (std::uint64_t entity)
  {
    m_mgr.delete_entity (entity);
    for (archetype_base *a : m_archetypes)
    {
        a->remove_entity(entity);
    }
  }

  template <typename... T>
  archetype_base *
  get_archetype ()
  {
    archetype_base *a = nullptr;
    for (archetype_base *b : m_archetypes)
      {
        std::size_t hascount = 0;
        if (b->type_count () != sizeof...(T))
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
        if (hascount == sizeof...(T))
          {
            a = b;
          }
      }
    return a;
  }

private:
  entity_mgr m_mgr;
  std::vector<archetype_base *> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_H_ */
