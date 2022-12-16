#ifndef ARCHETYPEARRAY_H_
#define ARCHETYPEARRAY_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <vector>

/**
 * @brief Stores contiguously heterogenious components.
 *
 * @tparam Components The component types to be stored in this Archetype.
 */
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
  constexpr static inline auto id() noexcept { return (... | Components::id); }

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
   * @return A pointer to the archetype if one with the correct id is found.
   *         If no component is found then nullptr is returned.
   */
  template <typename T>
  constexpr auto get_component() {
    auto addr = reinterpret_cast<uintptr_t>(&m_data[0]);
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

  /**
   * @brief Assign a value to a component. This uses the components assignment
   *        operator=().
   *
   * @tparam T The component type.
   * @param component The component value to assign.
   */
  template <typename T>
  constexpr void set_component(const T &component) {
    auto t = get_component<T>();
    *t = component;
  }

 private:
  char m_data[(... + sizeof(Components))];
};

/**
 * @brief Base class for ArchetypeArray.
 */
class ArchetypeArrayBase {
 public:
  virtual ~ArchetypeArrayBase(){};
  virtual int id() const noexcept = 0;
  virtual void add_entity(const int e) = 0;
  virtual void remove_entity(const int e) = 0;
};

/**
 * @brief Contiguous archetype storage.
 *
 * @tparam Components The component types to be stored in this Archetype.
 */
template <typename... Components>
class ArchetypeArray : public ArchetypeArrayBase {
 public:
  virtual ~ArchetypeArray(){};
  int id() const noexcept override { return Archetype<Components...>::id(); }

  /**
   * @brief Register an entity with this array.
   *
   * @param e The entity id.
   */
  void add_entity(const int e) override {
    m_entity_to_index[e] = m_components.size();
    m_components.emplace_back();
  }

  /**
   * @brief Remove an entity and its components from the array.
   *
   * @param e The entity to remove.
   */
  void remove_entity(int e) override { /* TODO */
    (void)e;
  }

  /**
   * @brief Get a pointer to the component with type T that is assigned to the
   *        entity id e
   *
   * @tparam T The type of the component to fetch.
   * @param e The entity to which the components are assigned to.
   */
  template <typename T>
  T *get_component(int e) {
    auto index = m_entity_to_index[e];
    auto archetype = &m_components[index];
    return archetype->template get_component<T>();
  }

  /**
   * @brief Set the value of a component assigned to an entity.
   *
   * @tparam T The type of the component.
   * @param e The id of the entity to which belongs the component.
   * @param component The value to assign.
   */
  template <typename T>
  void set_component(const int e, const T component) {
    auto c = get_component<T>(e);
    *c = component;
  }

 private:
  std::vector<Archetype<Components...>> m_components;  // TODO
  std::unordered_map<int, int> m_entity_to_index;
};

#endif  // ARCHETYPEARRAY_H_
