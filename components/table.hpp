#ifndef COMPONENT_TABLE_HPP_
#define COMPONENT_TABLE_HPP_

#include <algorithm>
#include <cstddef>
#include <vector>

namespace xen {

template <typename T, typename Key = std::size_t,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<Key, T>>>
class table {
 public:
  using pair_type = std::pair<Key, T>;
  using container_type = std::vector<pair_type, Allocator>;
  using size_type = typename container_type::size_type;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

  table() = default;
  constexpr table(size_type count) noexcept : m_storage{count} {}

  constexpr void insert(const Key& key, const T& value) noexcept {
    auto it = std::find_if(
        m_storage.cbegin(), m_storage.cend(),
        [&](const auto& pair) { return Compare{}(key, pair.first); });
    m_storage.insert(it, {key, value});
  }

  constexpr void remove(const Key& key) noexcept {
    m_storage.erase(
        std::find_if(m_storage.cbegin(), m_storage.cend(),
                     [=](const auto& pair) { return key == pair.first; }));
  }

  constexpr reference operator[](size_type pos) noexcept {
    return m_storage[pos].second;
  }
  constexpr const_reference operator[](size_type pos) const noexcept {
    return m_storage[pos].second;
  }

  template <typename InternalIterT>
  class iterator : public std::iterator<std::forward_iterator_tag, pair_type> {
   public:
    using iter = iterator<InternalIterT>;

    constexpr explicit iterator(InternalIterT it) noexcept : m_it{it} {}

    constexpr iter operator++() noexcept {
      ++m_it;
      return *this;
    }

    constexpr iter operator++(int) noexcept {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    constexpr iter operator+(std::size_t i) noexcept {
      return iterator(m_it + i);
    }
    constexpr iter operator-(std::size_t i) noexcept {
      return iterator(m_it - i);
    }
    constexpr reference operator*() noexcept { return m_it->second; }

    constexpr friend bool operator==(const iter& a, const iter& b) {
      return a.m_it->first == b.m_it->first;
    }

    constexpr friend bool operator!=(const iter& a, const iter& b) {
      return a.m_it->first != b.m_it->first;
    }

    constexpr friend bool operator<(const iter& a, const iter& b) {
      return a.m_it->first < b.m_it->first;
    }

    constexpr friend bool operator>(const iter& a, const iter& b) {
      return a.m_it->first > b.m_it->first;
    }

    constexpr friend bool operator<=(const iter& a, const iter& b) {
      return a.m_it->first <= b.m_it->first;
    }

    constexpr friend bool operator>=(const iter& a, const iter& b) {
      return a.m_it->first >= b.m_it->first;
    }

   private:
    InternalIterT m_it;
  };

  auto begin() noexcept { return iterator(m_storage.begin()); }
  auto end() noexcept { return iterator(m_storage.end()); }
  auto cbegin() const noexcept { return iterator(m_storage.cbegin()); }
  auto cend() const noexcept { return iterator(m_storage.cend()); }

  constexpr auto size() const noexcept { return m_storage.size(); }

 private:
  container_type m_storage;
};

}  // namespace xen

#endif  // COMPONENT_TABLE_HPP_