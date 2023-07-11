#ifndef ENTITY_MGR_H_
#define ENTITY_MGR_H_

#include <cstddef>
#include <cstdint>
#include <vector>
#include <unordered_map>

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

private:
  std::unordered_map<std::uint64_t, std::vector<std::size_t> > m_map;
  std::vector<std::uint64_t> m_freelist;
  std::uint64_t m_counter = 0;
};

} /* end of namespace xen */

#endif /* ENTITY_MGR_H_ */
