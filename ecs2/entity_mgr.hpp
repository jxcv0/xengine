#ifndef ENTITY_MGR_HPP_
#define ENTITY_MGR_HPP_

#include "types.hpp"
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
  eid_t create_entity();
  void delete_entity(std::uint64_t entity);
  std::size_t num_entities() const;
  bool is_valid_id(eid_t entity) const;

private:
  std::vector<eid_t> m_freelist;
  eid_t m_counter = 0;
};

} /* end of namespace xen */

#endif /* ENTITY_MGR_HPP_ */
