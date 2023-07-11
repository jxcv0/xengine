#ifndef ENTITY_MGR_H_
#define ENTITY_MGR_H_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <unordered_map>
#include <vector>

namespace xen
{

/*
 * Creates and destroys entity IDs
 */
class entity_mgr
{
public:
  entity_mgr () = default;
  std::uint64_t create_entity ();
  void delete_entity (std::uint64_t entity);
  std::uint64_t num_entities ();

  template <typename T>
  void
  add_component (std::uint64_t entity)
  {
    m_map.at (entity).push_back (typeid (T).hash_code ());
  }

  template <typename T>
  void
  remove_component (std::uint64_t entity)
  {
    try
      {
        auto &v = m_map.at (entity);
        auto it = std::find (std::execution::par_unseq, v.cbegin (), v.cend (),
                             typeid (T).hash_code ());
        if (it != v.cend ())
          {
            v.erase (it);
          }
      }
    catch (const std::exception &e)
      {
        /* TODO: some logging would be nice */
      }
  }

  template <typename T>
  bool
  has_component (std::uint64_t entity)
  {
    try
      {
        const auto &v = m_map.at (entity);
        if (std::find (std::execution::par_unseq, v.cbegin (), v.cend (),
                       typeid (T).hash_code ())
            == v.cend ())
          {
            return false;
          }
        return true;
      }
    catch (const std::exception &e)
      {
        /* TODO: some logging would be nice */
        return false;
      }
  }

private:
  std::unordered_map<std::uint64_t, std::vector<std::size_t> > m_map;
  std::vector<std::uint64_t> m_freelist;
  std::uint64_t m_counter = 0;
};

} /* end of namespace xen */

#endif /* ENTITY_MGR_H_ */
