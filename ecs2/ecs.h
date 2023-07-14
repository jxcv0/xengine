#ifndef ECS_H_
#define ECS_H_

#include <iterator>
#include <memory>

#include "archetype.h"
#include "entity_mgr.h"

namespace xen
{

class ecs : public std::enable_shared_from_this<ecs>
{
public:

  [[nodiscard]] static std::shared_ptr<ecs>
  create()
  {
    return std::shared_ptr<ecs>(new ecs);
  }

  using ArchetypeStorage = std::vector<std::shared_ptr<archetype_base> >;
  ecs() = default;

  template <typename... T>
  std::uint64_t
  create_entity()
  {
    std::uint64_t new_entity = m_mgr.create_entity();
    auto arch = get_archetype<T...>();
    if (arch != nullptr)
      {
        arch->add_entity(new_entity);
      }
    return new_entity;
  }

  template <typename... T>
  void
  create_archetype()
  {
    auto a = get_archetype<T...>();
    if (a != nullptr)
      {
        return;
      }
    m_archetypes.push_back(
        std::shared_ptr<archetype_base>(new archetype<T...>));
  }

  template <typename... T>
  void
  register_archetype(archetype<T...>* arch)
  {
    auto a = get_archetype<T...>();
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

  template <typename... T>
  std::shared_ptr<archetype_base>
  get_archetype()
  {
    std::shared_ptr<archetype_base> a = nullptr;
    for (auto& b : m_archetypes)
      {
        std::size_t hascount = 0;
        if (b->type_count() != sizeof...(T))
          {
            continue;
          }

        (
            [&] {
              if (b->has_type(typeid(T).hash_code()))
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

  friend class query;
  template <typename... Components>
  class query
  {
  public:
    friend class iterator;
    class iterator
    {
    public:
      using iterator_type = std::forward_iterator_tag;
      using reference = std::shared_ptr<archetype_base>&;
      using pointer = std::shared_ptr<archetype_base>;

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

    private:
      std::shared_ptr<archetype_base>
      get_next()
      {
        while ((*m_ecs_iter)->has_types // TODO
      }

      ArchetypeStorage::iterator m_ecs_iter;
    };

  private:
    std::shared_ptr<ecs> m_ecs;
  };

  template <typename... Components>
  query<Components...>
  create_query()
  {
    return query<Components...>(this);
  };

private:
  entity_mgr m_mgr;
  ArchetypeStorage m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_H_ */
