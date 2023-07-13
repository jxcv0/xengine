#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include <execution>
#include <unordered_map>
#include <utility>

namespace xen
{

struct archetype_base
{
  virtual void add_entity(std::uint64_t entity) = 0;
  virtual bool has_type(std::size_t) const = 0;
  virtual bool has_entity(std::uint64_t) const = 0;
  virtual std::size_t type_count() const = 0;
  virtual void *get_type(std::uint64_t entity, std::size_t typehash) = 0;
};

/*
 * template <typename T>
 * T &
 * get_component(std::uint64_t entity)
 * {
 *   xen::archetype_base * arch = get_arch_with_entity(entity);
 *   if (arch->has_type(typeid(T).hash_code())
 *     {
 *       return reinterpret_cast<T *>(arch.get_type(typeid(T).hash_code()));
 *     {
 *   return NULL;
 * }
 */

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

  void add_entity(std::uint64_t entity) override
  {
      m_entries.push_back(std::make_pair(entity, std::tuple<T...>{}));
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

  std::size_t
  type_count() const override
  {
      return sizeof...(T);
  }

  void *
  get_type(std::uint64_t entity, std::size_t typehash) override
  {
      void *res = NULL;
      auto &pair = get_by_entity(entity);
      /* Find the type by hash_code */
      (
       [&]{
            if (typeid(T).hash_code() == typehash)
            {
                res = &std::get<T>(pair.second);
            }
       }(), ...);
      
      return res;
  }

  template <typename U>
  U &
  get_component (std::uint64_t entity)
  {
    auto &pair = get_by_entity(entity);
    return std::get<U> (pair.second);
  }

private:
  std::pair<std::uint64_t, std::tuple<T...> > &
  get_by_entity(std::uint64_t entity)
  {
    auto it = std::find_if (std::execution::par_unseq, m_entries.begin (),
                            m_entries.end (),
                            [=] (const auto &e) { return e.first == entity; });
    if (it == m_entries.cend ())
      {
        throw std::runtime_error ("Entity not found in archetype");
      }
    return *it;
  }

private:
  std::vector<std::pair<std::uint64_t, std::tuple<T...> > > m_entries;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
