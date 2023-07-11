#ifndef ENTITY_MGR_H_
#define ENTITY_MGR_H_

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <algorithm>

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

  template <typename T> void add_component(std::uint64_t entity)
  {
      m_map.at(entity).push_back(typeid(T).hash_code());
  }

  template <typename T> void remove_component(std::uint64_t entity)
  {
      auto &v = m_map.at(entity);
      auto it = std::find(v.cbegin(), v.cend(), typeid(T).hash_code());
      if (it != v.cend())
        {
          v.erase(it);
        }
  }

  template <typename T> bool has_component(std::uint64_t entity)
  {
    const auto &v = m_map.at(entity);
    if (std::find(v.cbegin(), v.cend(), typeid(T).hash_code()) == v.cend())
      {
        return false;
      }
    return true;
  }

private:
  std::unordered_map<std::uint64_t, std::vector<std::size_t> > m_map;
  std::vector<std::uint64_t> m_freelist;
  std::uint64_t m_counter = 0;
};

} /* end of namespace xen */

#endif /* ENTITY_MGR_H_ */
