#ifndef ECS_H_
#define ECS_H_

#include <iterator>
#include <memory>

#include "archetype.h"
#include "entity_mgr.h"

namespace xen
{

class ecs
{
public:
  using ArchetypeStorage = std::vector<std::shared_ptr<archetype_base> >;

  ecs() = default;
  ~ecs() = default;

  template <typename... Components>
  std::uint64_t
  create_entity()
  {
    std::uint64_t new_entity = m_mgr.create_entity();
    auto arch = get_archetype<Components...>();
    if (arch != nullptr)
      {
        arch->add_entity(new_entity);
      }
    return new_entity;
  }

  template <typename... Components>
  void
  create_archetype()
  {
    auto a = get_archetype<Components...>();
    if (a != nullptr)
      {
        return;
      }
    m_archetypes.push_back(
        std::shared_ptr<archetype_base>(new archetype<Components...>));
  }

  template <typename... Components>
  void
  register_archetype(archetype<Components...>* arch)
  {
    auto a = get_archetype<Components...>();
    if (a != nullptr)
      {
        return;
      }
    m_archetypes.push_back(std::shared_ptr<archetype_base>(arch));
  }

  void
  delete_entity(std::uint64_t entity)
  {
    m_mgr.delete_entity(entity);
    for (const auto& a : m_archetypes)
      {
        a->remove_entity(entity);
      }
  }

  template <typename... Components>
  std::shared_ptr<archetype_base>
  get_archetype()
  {
    std::shared_ptr<archetype_base> a = nullptr;
    for (auto& b : m_archetypes)
      {
        std::size_t hascount = 0;
        if (b->type_count() != sizeof...(Components))
          {
            continue;
          }

        (
            [&] {
              if (b->has_type(typeid(Components).hash_code()))
                {
                  ++hascount;
                }
            }(),
            ...);
        if (hascount == sizeof...(Components))
          {
            a = b;
          }
      }
    return a;
  }

  friend class query;
  template <typename... Components>
  class query
  {
  public:
    class iterator
        : public std::iterator<std::forward_iterator_tag,
                               std::shared_ptr<archetype_base>, std::ptrdiff_t,
                               std::shared_ptr<archetype_base>,
                               std::shared_ptr<archetype_base> >
    {
    public:
      using reference = std::shared_ptr<archetype_base>&;
      using pointer = std::shared_ptr<archetype_base>;

      /*
            reference
            operator*() const
            {
              return *m_ptr;
            }

            pointer
            operator->()
            {
              return m_ptr;
            }

            iterator&
            operator++()
            {
            }

            iterator
            operator++(int)
            {
            }

            friend bool
            operator==(const iterator& a, const iterator& b)
            {
            }

            friend bool
            operator!=(const iterator& a, const iterator& b)
            {
            }
            */

      iterator
      begin()
      {
      }

    private:
      /* Iterate to the next archetype with Components */
      std::shared_ptr<archetype_base>
      get_next()
      {
        while (!(*m_ecs_iter)->has_components<Components...>())
          {
            m_ecs_iter++;
          }
        return *m_ecs_iter;
      }

      ArchetypeStorage::iterator m_ecs_iter;
    };

  private:
    ecs& m_ecs;
  };

  template <typename... Components>
  query<Components...>
  create_query()
  {
    return query<Components...>(this);
  }

private:
  entity_mgr m_mgr;
  ArchetypeStorage m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_H_ */
