#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cstddef>
#include <cstdint>
#include <functional>
#include <ios>
#include <iostream>
#include <vector>

// this might not be any better than a 2 dimentional array

template <typename... Components>
struct Archetype {
  /**
   * @brief Construct an Archetype object.
   *
   * @param mem The memory block to store the components in. mem is assumed to
   *            be large enough to store one of each component.
   */
  constexpr explicit inline Archetype(void *mem) noexcept : mp_data(mem) {
    m_id = (... | Components::id);
    static_assert((Components::id > ...));  // templates must be in order
    auto addr = reinterpret_cast<uintptr_t>(mp_data);
    (
        [&]() {
          auto p = reinterpret_cast<Components *>(addr);
          *p = Components();
          addr += sizeof(Components);
        }(),
        ...);
  }

  constexpr auto inline size() const noexcept {
    return (0 + ... + sizeof(Components));
  }

  constexpr inline auto id() const noexcept { return m_id; }

  constexpr inline auto count() const noexcept { return sizeof...(Components); }

  template <typename T>
  auto get() {}

  int m_id;  // a bitmask of all id's of the components of this type;
  void *mp_data;
};

template <typename... Components>
class ArchetypeArray {
 private:
  std::vector<Archetype<Components...>> m_components;
};

#endif  // COMPONENTARRAY_H_
