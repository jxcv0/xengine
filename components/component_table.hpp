#ifndef COMPONENT_TABLE_HPP_
#define COMPONENT_TABLE_HPP_

#include <algorithm>
#include <cstddef>
#include <vector>

namespace xen {

template <typename T, typename Key = std::size_t, typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<Key, T>>>
class component_table {
 public:
  using container_type = std::vector<std::pair<Key, T>, Allocator>;
  using size_type = typename container_type::size_type;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

  component_table() = default;
  component_table(size_type count) : m_storage{count} {}

  void insert(const Key& key, const T& value) {
    auto it = std::find_if(m_storage.cbegin(), m_storage.cend(),
                           [&](const auto& pair) { return Compare{}(key, pair.first); });
    m_storage.insert(it, {key, value});
  }

  void remove(const Key& key) {
    auto it = std::find_if(m_storage.cbegin(), m_storage.cend(),
                           [=](const auto& pair) { return key == pair.first; });
    m_storage.erase(it);
  }

  reference operator[](size_type pos) { return m_storage[pos].second; }
  const_reference operator[](size_type pos) const {
    return m_storage[pos].second;
  }

 private:
  container_type m_storage;
};

}  // namespace xen

#endif /* COMPONENT_TABLE_HPP_ */