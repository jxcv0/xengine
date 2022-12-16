#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>

// this might not be any better than a 2 dimentional array

template <typename... Components>
class Archetype {
 public:
  /**
   * @brief Construct an Archetype object.
   *
   * @param mem The memory block to store the components in. mem is assumed to
   *            be large enough to store one of each component.
   */
  constexpr explicit inline Archetype() noexcept {
    m_id = (... | Components::id);
    static_assert((Components::id > ...));  // templates must be in order
    auto addr = reinterpret_cast<uintptr_t>(m_data);
    (
        [&] {
          auto p = reinterpret_cast<Components *>(addr);
          *p = Components();
          addr += sizeof(Components);
        }(),
        ...);
  }

  /**
   * @brief Get the number of bytes required to store components
   *
   * @return The size of the memory occupied by components
   */
  constexpr auto inline size() const noexcept {
    return (... + sizeof(Components));
  }

  /**
   * @brief Get the id of this archetype. This is a bitmask of all the
   *        Component ids.
   *
   * @return And integer component id.
   */
  constexpr inline auto id() const noexcept { return m_id; }

  /**
   * @brief Get the number of components in the archetype.
   *
   * @return The number of archetypes.
   */
  constexpr inline auto count() const noexcept { return sizeof...(Components); }

  /**
   * @brief get a pointer to a component from an archetype.
   *
   * @tparam T The type of the component to search for.
   * @param arch The archetype to search
   * @return A pointer to the archetype if one with the correct id is found.
   *         If no component is found then nullptr is returned.
   */
  template <typename T>
  constexpr friend auto get_component(const Archetype<Components...> &arch) {
    auto addr = reinterpret_cast<uintptr_t>(arch.m_data);
    T *t = nullptr;
    (
        [&] {
          auto p = reinterpret_cast<Components *>(addr);
          if (T::id == Components::id) {
            t = reinterpret_cast<T *>(p);
          }
          addr += sizeof(Components);
        }(),
        ...);
    return t;
  }

 private:
  int m_id;  // a bitmask of all id's of the components of this type;
  char m_data[(... + sizeof(Components))];
};

template <std::size_t N, typename... Components>
class ArchetypeArray {
 private:
  Archetype<Components...> m_components[N];
};

#endif  // COMPONENTARRAY_H_
