#ifndef ARCHETYPEARRAY_H_
#define ARCHETYPEARRAY_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <queue>
#include <type_traits>

/**
 * @brief Stores contiguously heterogenious components.
 *
 * @tparam Components The component types to be stored in this Archetype.
 */
template <typename... Components>
class Archetype {
 public:
  Archetype() = default;
  /**
   * @brief Construct an Archetype object.
   *
   * @param mem The memory block to store the components in. mem is assumed to
   *            be large enough to store one of each component.
   */
  constexpr explicit inline Archetype(int id) noexcept : m_entity_id(id) {
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

  Archetype(const Archetype<Components...> &) = default;
  Archetype(Archetype<Components...> &&) = default;
  Archetype<Components...> &operator=(const Archetype<Components...> &) =
      default;
  ~Archetype() = default;

  /**
   * @brief Compare two archetypes.
   *
   * @param a1 The first archetype.
   * @param a2 The second archetype.
   * @return true if the components in both are the same.
   */
  constexpr friend auto operator==(const Archetype<Components...> &a1,
                                   const Archetype<Components...> &a2) {
    return (... &&
            (a1.get_component<Components>() == a2.get_component<Components>()));
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
  template <typename Component>
  constexpr Component &get_component() const {
    static_assert((std::is_same_v<Component, Components> || ...));
    auto addr = reinterpret_cast<uintptr_t>(&m_data[0]);
    Component *c = nullptr;
    (
        [&] {
          auto p = reinterpret_cast<Components *>(addr);
          if (Component::id == Components::id) {
            c = reinterpret_cast<Component *>(p);
          }
          addr += sizeof(Components);
        }(),
        ...);
    return *c;
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
    get_component<T>() = component;
  }

  int entity() const noexcept { return m_entity_id; }

 private:
  int m_entity_id = 0;
  char m_data[(... + sizeof(Components))];
};

/**
 * @brief Base class for ArchetypeArray.
 */
class ArchetypeArrayBase {
 public:
  virtual ~ArchetypeArrayBase(){};
  constexpr virtual int id() const noexcept = 0;
  constexpr virtual int add_entity(const int e) noexcept = 0;
  constexpr virtual void remove_entity(const int e) noexcept = 0;
};

/**
 * @brief Contiguous archetype storage.
 *
 * @tparam Components The component types to be stored in this Archetype.
 */
template <typename... Components>
class ArchetypeArray : public ArchetypeArrayBase {
 public:
  using archetype = Archetype<Components...>;

  ArchetypeArray(unsigned int n) : m_max_components(n) {
    for (unsigned int i = 0; i < m_max_components; i++) {
      m_free_list.push(i);
    }
    mp_components = new archetype[m_max_components];
  }

  virtual ~ArchetypeArray() { delete[] mp_components; }

  int id() const noexcept override { return Archetype<Components...>::id(); }

  /**
   * @brief Register an entity with this array. The Components are default
   *        constructed.
   *
   * @param e The entity id.
   */
  constexpr int add_entity(const int e) noexcept override {
    if (!m_free_list.empty()) {
      return -1;
    }
    mp_components[m_free_list.front()] = archetype(e);
    m_free_list.pop();
    return 0;
  }

  /**
   * @brief Remove an entity and its components from the array.
   *
   * @param e The entity to remove.
   * @throws Exception if there is no entity e in this table.
   */
  constexpr void remove_entity(const int e) noexcept override {
    if (find(e) != nullptr) {
      m_free_list.push(e);
    }
  }

  /**
   * @brief Get a pointer to the component with type T that is assigned to the
   *        entity id e.
   *
   * @tparam T The type of the component to fetch.
   * @param e The entity to which the components are assigned to.
   * @return A pointer to the component or nullptr if no component is found.
   */
  template <typename Component>
  Component &get_component(const int e) {
    static_assert((std::is_same_v<Component, Components> || ...));
    assert(find(e) != nullptr);  // entity id is not in array
    return find(e)->template get_component<Component>();
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
    get_component<T>(e) = component;
  }

  constexpr auto size() const noexcept {
    return m_max_components - m_free_list.size();
  }

  template <typename Component>
  class Iterator {
   public:
    using iter = Iterator<Component>;
    using value_type = Component;
    using pointer = Component *;
    using reference = Component &;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr inline explicit Iterator(archetype *p) {
      mp_ptr = &p->template get_component<Component>();
    }

    constexpr inline pointer operator->() { return mp_ptr; }

    constexpr inline reference operator*() { return *mp_ptr; }

    constexpr friend bool operator==(const iter &it1, const iter &it2) {
      return it1.mp_ptr == it2.mp_ptr;
    }

    constexpr friend bool operator!=(const iter &it1, const iter &it2) {
      return it1.mp_ptr != it2.mp_ptr;
    }

    constexpr inline iter &operator++() {
      mp_ptr += sizeof(archetype);
      return *this;
    }

    constexpr inline iter operator++(int) {
      auto tmp = *this;
      operator++();
      return tmp;
    }

   private:
    pointer mp_ptr;
  };

  template <typename Component>
  auto begin() {
    return Iterator<Component>(&mp_components[0]);
  }

  template <typename Component>
  auto end() {
    return Iterator<Component>(&mp_components[size()]);
  }

 private:
  archetype *find(const int e) const noexcept {
    for (unsigned int i = 0; i < size(); i++) {
      if (mp_components[i].entity() == e) {
        return &mp_components[i];
      }
    }
    return nullptr;
  }

  unsigned int m_max_components;
  std::queue<int> m_free_list;
  archetype *mp_components;
};

#endif  // ARCHETYPEARRAY_H_
