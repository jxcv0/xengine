#ifndef ECS_HPP_
#define ECS_HPP_

#include <atomic>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <typeindex>

namespace xen {

using eid_t = std::uint64_t;

struct archetype_interface {
  virtual ~archetype_interface() {}

  virtual bool has_component(const std::type_index&) = 0;
  virtual std::size_t num_component_types() = 0;
  virtual void* get_component(eid_t, const std::type_index&) = 0;
  virtual void add_entity(eid_t) = 0;

  template <typename T>
  T& get_component(eid_t entity) {
    return *reinterpret_cast<T*>(
        get_component(entity, std::type_index(typeid(T))));
  }

  template <typename... T>
  bool has_components() {
    std::size_t count = 0;
    (
        [&] {
          if (has_component(std::type_index(typeid(T)))) {
            ++count;
          }
        }(),
        ...);
    return count == sizeof...(T);
  }

  template <typename... T>
  void add_entity(eid_t entity, T... vals) {
    add_entity(entity);
    ((get_component<T>(entity) = vals), ...);
  }
};

template <typename... T>
using entry = std::pair<eid_t, std::tuple<T...>>;

template <typename... T>
using table = std::vector<entry<T...>>;

template <typename... T>
class archetype : public archetype_interface {
 public:
  static constexpr std::size_t num_types = sizeof...(T);

  bool has_component(const std::type_index& index) override {
    return ((index == std::type_index(typeid(T))) || ...);
  }

  std::size_t num_component_types() override { return num_types; }

  void* get_component(eid_t entity, const std::type_index& index) override {
    auto it = find_by_entity(entity);
    void* ptr;
    if (it != m_table.cend()) {
      (
          [&] {
            if (std::type_index(typeid(T)) == index) {
              ptr = &std::get<T>(it->second);
            }
          }(),
          ...);
    } else {
      throw std::out_of_range("Entity not found");
    }
    return ptr;
  }

  void add_entity(eid_t entity) override {
    m_table.emplace_back(entity, std::tuple<T...>{});
  }

  template <typename U>
  U& get_component(eid_t entity) {
    auto it = find_by_entity(entity);
    if (it == m_table.end()) {
      throw std::out_of_range("Entity not found");
    }
    return std::get<U>(it->second);
  }

 private:
  table<T...> m_table;

  constexpr auto find_by_entity(eid_t entity) const {
    return std::find_if(m_table.cbegin(), m_table.cend(),
                        [=](const auto& e) { return e.first == entity; });
  }

  constexpr auto find_by_entity(eid_t entity) {
    return std::find_if(m_table.begin(), m_table.end(),
                        [=](const auto& e) { return e.first == entity; });
  }
};

class ecs {
 public:
  ecs() : m_archetypes{}, m_counter{0} {}

  template <typename... T>
  eid_t create_entity(T... vals) {
    auto it = find_archetype<T...>();
    eid_t entity = m_counter;
    if (it != m_archetypes.cend()) {
      (*it)->add_entity(entity, vals...);
    } else {
      m_archetypes.emplace_back(new archetype<T...>);
      m_archetypes.back()->add_entity(entity, vals...);
    }
    ++m_counter;
    return entity;
  }

  template <typename... T>
  auto get_archetype() {
    auto it = std::find_if(m_archetypes.begin(), m_archetypes.end(),
                           [](const auto& arch) {
                             return arch->template has_components<T...>() &&
                                    arch->num_component_types() == sizeof...(T);
                           });
    if (it == m_archetypes.end()) {
      throw std::runtime_error("Archetype not found");
    }
    return std::static_pointer_cast<archetype<T...>>(*it);
  }

 private:
  std::vector<std::shared_ptr<archetype_interface>> m_archetypes;
  std::atomic<eid_t> m_counter;

  template <typename... T>
  constexpr auto find_archetype() const noexcept {
    return std::find_if(
        m_archetypes.cbegin(), m_archetypes.cend(),
        [](const auto& arch) { return arch->template has_components<T...>(); });
  }
};

}  // namespace xen

#endif  // ECS_HPP_
