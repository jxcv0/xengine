#ifndef ECS_HPP_
#define ECS_HPP_

#include <atomic>
#include <cstdint>
#include <memory>
#include <tuple>
#include <typeindex>

namespace xen {

using eid_t = std::uint64_t;

struct archetype_interface {
  virtual ~archetype_interface() {}

  virtual bool has_component(const std::type_index&) = 0;
  virtual std::size_t num_component_types() = 0;

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
};

template <typename... T>
using entry = std::pair<eid_t, std::tuple<T...>>;

template <typename... T>
using table = std::vector<entry<T...>>;

template <typename... T>
class archetype : public archetype_interface {
 public:
  static constexpr std::size_t num_types = sizeof...(T);

  bool has_component(const std::type_index& index) {
    return ((index == std::type_index(typeid(T))) || ...);
  }

  std::size_t num_component_types() override { return num_types; }

  void add_entity(eid_t entity, T... vals) {
    m_table.emplace_back({entity, std::tuple<T...>{vals...}});
  }

 private:
  table<T...> m_table;
};

class ecs {
 public:
  ecs() : m_archetypes{}, m_counter{0} {}

  template <typename... T>
  void create_entity(T... vals) {
    auto it = find_archetype<T...>();
    if (it != m_archetypes.cend()) {
      it->add_entity(m_counter++, vals...);
    } else {
      m_archetypes.emplace_back(new archetype<T...>);
      m_archetypes.back()->add_entity(m_counter++, vals...);
    }
  }

 private:
  std::vector<std::shared_ptr<archetype_interface>> m_archetypes;
  std::atomic<eid_t> m_counter;

  template <typename... T>
  constexpr auto find_archetype() const noexcept {
    return std::find_if(
        m_archetypes.cbegin(), m_archetypes.cend(),
        [](const auto& arch) { return arch.template has_components<T...>(); });
  }
};

}  // namespace xen

#endif  // ECS_HPP_
