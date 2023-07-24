#ifndef INTERSECT_HPP_
#define INTERSECT_HPP_

#include <tuple>

#include "table.hpp"

namespace xen {

template <typename EndIt1, typename EndIt2>
struct sentinel {
  EndIt1 it1;
  EndIt2 it2;
};

template <typename C1, typename C2>
class intersection {
 public:
  constexpr intersection(table<C1>& t1, table<C2>& t2) noexcept
      : m_table1{t1}, m_table2{t2} {}

  class iterator;

  constexpr auto begin() noexcept {
    return iterator{
        m_table1.begin(), m_table2.begin(), {m_table1.end(), m_table2.end()}};
  }
  constexpr auto end() noexcept {
    return iterator{
        m_table1.end(), m_table2.end(), {m_table1.end(), m_table2.end()}};
  }

 private:
  table<C1>& m_table1;
  table<C2>& m_table2;
};

template <typename C1, typename C2>
class intersection<C1, C2>::iterator {
 public:
  using InputIt1 = decltype(std::declval<table<C1>>().begin());
  using InputIt2 = decltype(std::declval<table<C2>>().begin());
  using End = sentinel<InputIt1, InputIt2>;

  constexpr iterator(InputIt1 it1, InputIt2 it2, End end) noexcept
      : m_it1{it1}, m_it2{it2}, m_end{end} {};

  constexpr friend bool operator==(const iterator& a, const iterator& b) {
    return a.m_it1 == b.m_it1 && b.m_it2 == b.m_it2;
  }

  constexpr iterator operator++() noexcept {
    while (m_it1 != m_it2 || m_end.it1 != m_it2 || m_end.it2 != m_it2) {
      if (m_it1 < m_it2) {
        ++m_it1;
      }
      if (m_it1 > m_it2) {
        ++m_it2;
      }
    }
    return *this;
  }

  constexpr iterator operator++(int) noexcept {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  constexpr auto operator*() { return std::forward_as_tuple(*m_it1, *m_it2); }

 private:
  InputIt1 m_it1;
  InputIt2 m_it2;
  End m_end;
};

}  // namespace xen

#endif  // INTERSECT_HPP_