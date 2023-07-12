#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include <unordered_map>

namespace xen
{

template <typename... T> class archetype
{
public:
  template <typename U>
  bool
  has_component ()
  {
    return (std::is_same<U, T>::value || ...);
  }

  template <typename... U>
  bool
  has_components ()
  {
    const std::size_t ntypes = sizeof...(U);
    std::size_t hascount = 0;
    (
        [&] {
          if (has_component<U> ())
            {
              ++hascount;
            }
        }(),
        ...);
    return hascount == ntypes;
  }

  template <typename... U, typename... V>
  friend void
  move_entity (std::uint64_t entity, const archetype<U...> &from,
              archetype<V...> &to)
  {
    // HOW??
  }

  bool
  has_entity (std::uint64_t entity) const
  {
    auto it = std::find_if (m_entries.cbegin (), m_entries.cend (),
                          [=] (const auto &e) { return e.entity == entity; });
    return it != m_entries.cend ();
  }

private:
  struct entity_entry
  {
    std::uint64_t entity;
    std::tuple<T...> tuple;
  };
  std::vector<entity_entry> m_entries;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
