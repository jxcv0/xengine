#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include <cstdint>
#include <execution>
#include <unordered_map>
#include <utility>

namespace xen
{

/**
 * @brief Interface class for all class template archetypes.
 *
 */
class archetype_base
{
public:
  virtual void
  add_entity(std::uint64_t)
  {
  }

  virtual void
  remove_entity(std::uint64_t)
  {
  }

  virtual bool
  has_type(std::size_t) const
  {
    return false;
  }

  virtual bool
  has_entity(std::uint64_t) const
  {
    return false;
  }

  virtual std::size_t
  type_count() const
  {
    return 0;
  }

  virtual void*
  get_type(std::uint64_t, std::size_t)
  {
    return nullptr;
  }

  virtual void*
  get_at_index(std::size_t, std::size_t)
  {
    return nullptr;
  }

  virtual std::size_t
  size() const
  {
    return 0;
  }

  template <typename T>
  T*
  get_component(std::uint64_t entity)
  {
    return static_cast<T*>(get_type(entity, typeid(T).hash_code()));
  }

  template <typename... Components>
  bool
  has_components()
  {
    const std::size_t ntypes = sizeof...(Components);
    std::size_t hascount = 0;
    (
        [&] {
          if (has_type(typeid(Components).hash_code()))
            {
              ++hascount;
            }
        }(),
        ...);
    return hascount == ntypes;
  }

  template <typename T>
  T*
  get_component_by_index(std::size_t index)
  {
    return static_cast<T*>(get_at_index(index, typeid(T).hash_code()));
  }
};

/**
 * @brief Archetypes are a group of entities with the same components.
 *
 * @tparam T The types of component of the archetype.
 */
template <typename... T>
class archetype : public archetype_base
{
public:
  using container_type
      = std::vector<std::pair<std::uint64_t, std::tuple<T...> > >;

  /**
   * @brief Check if the archetype has a component type.
   *
   * @tparam U The type of the component to check for.
   * @return true if the archetype has component type U.
   * @return false if the archetype does not have component U.
   */
  template <typename U>
  constexpr bool
  has_component()
  {
    return (std::is_same<U, T>::value || ...);
  }

  /**
   * @brief Check if the archetype has multiple component types.
   *
   * @tparam U The types to check for.
   * @return true if all component types U are in this archetype.
   * @return false if any of component types U are not in this archetype.
   */
  template <typename... U>
  constexpr bool
  has_components()
  {
    const std::size_t ntypes = sizeof...(U);
    std::size_t hascount = 0;
    (
        [&] {
          if (has_component<U>())
            {
              ++hascount;
            }
        }(),
        ...);
    return hascount == ntypes;
  }

  /**
   * @brief Add an entity to the archetype.
   *
   * @param entity The entity to add.
   */
  void
  add_entity(std::uint64_t entity) override
  {
    m_entries.push_back(std::make_pair(entity, std::tuple<T...>{}));
  }

  /**
   * @brief Remove an entity from the archetype.
   *
   * @param entity The entity to remove.
   */
  void
  remove_entity(std::uint64_t entity) override
  {
    m_entries.erase(std::remove_if(
        std::execution::par_unseq, m_entries.begin(), m_entries.end(),
        [=](const auto& e) { return e.first == entity; }));
  }

  /**
   * @brief Check if the archetype has a component by the hash code of its
   * type.
   *
   * @param hash The has code of the type.
   * @return true if hash is equal to the hash of one of the types of
   * components in this archetype.
   * @return false if hash is not equal to any of the hashes of the types of
   * the components in this archetype.
   */
  bool
  has_type(std::size_t hash) const override
  {
    return ((hash == typeid(T).hash_code()) || ...);
  }

  /**
   * @brief Check if an entity is of the archetype.
   *
   * @param entity The entity to check for.
   * @return true if the entity is of this archetype.
   * @return false if the entity is not of this archetype.
   */
  bool
  has_entity(std::uint64_t entity) const override
  {
    auto it = std::find_if(std::execution::par_unseq, m_entries.cbegin(),
                           m_entries.cend(),
                           [=](const auto& e) { return e.first == entity; });
    return it != m_entries.cend();
  }

  /**
   * @brief Get the number of component types this archetype is comprised of.
   *
   * @return The number of component types this archetype is comprised of.
   */
  std::size_t
  type_count() const override
  {
    return sizeof...(T);
  }

  /**
   * @brief Get a component by the hash code of its type.
   *
   * @param entity The entity that owns the component.
   * @param typehash The hashcode of the type.
   * @return A pointer to the component or nullptr if the component cannot be
   * retrieved.
   */
  void*
  get_type(std::uint64_t entity, std::size_t typehash) override
  {
    void* res = nullptr;
    auto& pair = get_by_entity(entity);
    /* Find the type by hash_code */
    (
        [&] {
          if (typeid(T).hash_code() == typehash)
            {
              res = &std::get<T>(pair.second);
            }
        }(),
        ...);

    return res;
  }

  void*
  get_at_index(std::size_t index, std::size_t typehash) override
  {
    void* res = nullptr;
    (
        [&] {
          if (typeid(T).hash_code() == typehash)
            {
              res = &std::get<T>(m_entries[index].second);
            }
        }(),
        ...);
    return res;
  }

  std::size_t
  size() const override
  {
    return m_entries.size();
  }

  /**
   * @brief Get a component belonging to an entity.
   *
   * @tparam U The type of the component.
   * @param entity The entity the component belongs too.
   * @return A reference to the component.
   */
  template <typename U>
  U&
  get_component(std::uint64_t entity)
  {
    auto& pair = get_by_entity(entity);
    return std::get<U>(pair.second);
  }

private:
  std::pair<std::uint64_t, std::tuple<T...> >&
  get_by_entity(std::uint64_t entity)
  {
    auto it = std::find_if(std::execution::par_unseq, m_entries.begin(),
                           m_entries.end(),
                           [=](const auto& e) { return e.first == entity; });
    if (it == m_entries.cend())
      {
        throw std::runtime_error("Entity not found in archetype");
      }
    return *it;
  }

private:
  container_type m_entries;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
