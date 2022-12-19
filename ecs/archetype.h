#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

// K.I.S.S

/**
 * @brief Storage for heterogeneous comonent types in contiguous memory.
 */
class Archetype {
 public:
  Archetype(int id) : m_entity_id(id) {}

 private:
  int m_entity_id;
};

#endif  // ARCHETYPE_H_
