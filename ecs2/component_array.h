#ifndef COMPONENT_ARRAY_H_
#define COMPONENT_ARRAY_H_

#include <algorithm>
#include <cstdint>
#include <execution>
#include <stdexcept>
#include <vector>

namespace xen
{

template <typename T> struct component
{
  std::uint64_t entity;
  T value;

  static std::size_t
  id ()
  {
    return typeid (T).hash_code ();
  }
};

struct component_array_base
{
  virtual std::size_t component_id () const = 0;
  virtual void assign (std::uint64_t entity) = 0;
  virtual void remove (std::uint64_t entity) = 0;
};

template <typename T> class component_array : public component_array_base
{
public:
  std::size_t
  component_id () const override
  {
    return component<T>::id ();
  }

  void
  assign (std::uint64_t entity) override
  {
    /* TODO: entities cannot have multiple entries in the array */
    component<T> c{};
    c.entity = entity;
    m_arr.push_back (c);
  }

  void
  remove (std::uint64_t entity) override
  {
    m_arr.erase (std::remove_if (
                     std::execution::par_unseq, m_arr.begin (), m_arr.end (),
                     [=] (const auto &c) { return c.entity == entity; }),
                 m_arr.end ());
  }

  T
  get (std::uint64_t entity)
  {
    auto it = std::find_if (
        std::execution::par_unseq, m_arr.cbegin (), m_arr.cend (),
        [=] (const auto &c) { return c.entity == entity; });
    if (it != m_arr.cend ())
      {
        return (*it).value;
      }
    else
      {
        throw std::runtime_error ("Entity not found");
      }
  }

  void
  set (std::uint64_t entity, T val)
  {
    auto it = std::find_if (
        std::execution::par_unseq, m_arr.begin (), m_arr.end (),
        [=] (const auto &c) { return c.entity == entity; });
    if (it != m_arr.end ())
      {
        (*it).value = val;
      }
  }

private:
  std::vector<component<T> > m_arr{};
};

} /* end of namespace xen */

#endif /* COMPONENT_ARRAY_H_ */
