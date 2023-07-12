#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include <execution>
#include <unordered_map>
#include <utility>

namespace xen
{

struct archetype_base
{
  virtual bool has_type(std::size_t) const = 0;
  virtual bool has_entity(std::uint64_t) const = 0;
};

template <typename... T> class archetype : public archetype_base
{
public:
  template <typename U>
  constexpr bool
  has_component ()
  {
    return (std::is_same<U, T>::value || ...);
  }

  template <typename... U>
  constexpr bool
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

  template <typename... U>
  void
  register_entity (std::uint64_t entity, U... vals)
  {
    if (has_entity (entity) || !has_components<U...> ())
      {
        return;
      }
    std::tuple<U...> new_tuple;
    ((std::get<U> (new_tuple) = vals), ...);
    m_entries.push_back (std::make_pair (entity, new_tuple));
  }

  bool has_type(std::size_t hash) const override
  {
    return ((hash == typeid(T).hash_code()) || ...);
  }
  
  bool
  has_entity (std::uint64_t entity) const override
  {
    auto it = std::find_if (std::execution::par_unseq, m_entries.cbegin (),
                            m_entries.cend (),
                            [=] (const auto &e) { return e.first == entity; });
    return it != m_entries.cend ();
  }

  template <typename U>
  U &
  get_component (std::uint64_t entity)
  {
    auto it = std::find_if (std::execution::par_unseq, m_entries.begin (),
                            m_entries.end (),
                            [=] (const auto &e) { return e.first == entity; });
    if (it == m_entries.cend ())
      {
        throw std::runtime_error ("Entity not found in archetype");
      }
    return std::get<U> ((*it).second);
  }

private:
  std::vector<std::pair<std::uint64_t, std::tuple<T...> > > m_entries;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
