#ifndef COMPONENT_TABLE_HPP_
#define COMPONENT_TABLE_HPP_

#include <algorithm>
#include <cstddef>
#include <vector>

namespace xen {

template <typename T, typename Key = std::size_t,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<Key, T>>>
class component_table {
 public:
  using pair_type = std::pair<Key, T>;
  using container_type = std::vector<pair_type, Allocator>;
  using size_type = typename container_type::size_type;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

  component_table() = default;
  component_table(size_type count) : m_storage{count} {}

  void insert(const Key& key, const T& value) {
    auto it = std::find_if(
        m_storage.cbegin(), m_storage.cend(),
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

  template <typename InternalIterT>
  class iterator : public std::iterator<std::forward_iterator_tag, pair_type> {
   public:
    using iter = iterator<InternalIterT>;

    iterator(InternalIterT it) : m_it{it} {}

    iter operator++() {
      m_it++;
      return *this;
    }

    iter operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    iter operator+(std::size_t i) { return iterator(m_it + i); }
    iter operator-(std::size_t i) { return iterator(m_it - i); }
    reference operator*() { return m_it->second; }

    friend bool operator==(const iter& a, const iter& b) {
      return a.m_it->first == b.m_it->first;
    }

    friend bool operator!=(const iter& a, const iter& b) {
      return a.m_it->first != b.m_it->first;
    }

    friend bool operator<(const iter& a, const iter& b) {
      return a.m_it->first < b.m_it->first;
    }

    friend bool operator>(const iter& a, const iter& b) {
      return a.m_it->first > b.m_it->first;
    }

   private:
    InternalIterT m_it;
  };

  auto begin() { return iterator(m_storage.begin()); }
  auto end() { return iterator(m_storage.end()); }

  auto cbegin() const { return iterator(m_storage.begin()); }
  auto cend() const { return iterator(m_storage.end()); }

 private:
  container_type m_storage;
};

}  // namespace xen

#endif /* COMPONENT_TABLE_HPP_ */