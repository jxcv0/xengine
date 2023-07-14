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
  using container_type = std::vector<std::shared_ptr<archetype_base> >;
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

  template <typename... T>
  std::vector<std::shared_ptr<archetype_base> >
  get_archetypes()
  {
    std::vector<std::shared_ptr<archetype_base> > vec;
    std::shared_ptr<archetype_base> a = nullptr;
    for (auto& b : m_archetypes)
      {
        std::size_t hascount = 0;
        if (b->type_count() < sizeof...(T))
          {
            /* arch does not have all of T */
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
            vec.push_back(a);
          }
      }
    return vec;
  }

  template <typename... T> class view
  {
  public:
    class iterator
    {
    public:
      using iterator_type = std::forward_iterator_tag;
    };

    view(ecs& ecs) {}

    iterator
    begin()
    {
    }

  private:
    container_type::iterator m_container_iter;
  };

private:
  entity_mgr m_mgr;
  container_type m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_H_ */
