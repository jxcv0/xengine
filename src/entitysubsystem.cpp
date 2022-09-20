#include "entitysubsystem.h"

#include <iterator>

/*------------------------------------------------------------------------------
 */
EntitySubsystem::EntitySubsystem() {
  for (eid_t i = 0; i < MAX_ENTITIES; ++i) {
    m_available_eids.push(i);
  }
}

/*------------------------------------------------------------------------------
 */
eid_t EntitySubsystem::create_entity() {
  if (m_num_entities < MAX_ENTITIES) {
    return -1;
  }
  auto entity = m_available_eids.front();
  m_available_eids.pop();
  ++m_num_entities;

  return entity;
}

/*------------------------------------------------------------------------------
 */
eid_t EntitySubsystem::create_entity(signature_t sig) {
  auto entity = create_entity();
  set_signature(entity, sig);
  return entity;
}

/*------------------------------------------------------------------------------
 */
void EntitySubsystem::destroy_entity(eid_t entity) {
  assert(entity < MAX_ENTITIES);
  m_signatures[entity].reset();
  m_available_eids.push(entity);
  --m_num_entities;
}

/*------------------------------------------------------------------------------
 */
void EntitySubsystem::set_signature(eid_t entity, signature_t sig) {
  assert(entity < MAX_ENTITIES);
  m_signatures[entity] = sig;
}

/*------------------------------------------------------------------------------
 */

signature_t EntitySubsystem::get_signature(eid_t entity) {
  assert(entity < MAX_ENTITIES);
  return m_signatures[entity];
}
