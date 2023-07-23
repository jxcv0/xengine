#ifndef PROCESS_JOIN_HPP_
#define PROCESS_JOIN_HPP_

#include <utility>

#include "component_table.hpp"

namespace xen {

template <typename Table1, typename Table2>
class splice {
 public:
  using value_type =
      std::pair<typename Table1::value_type, typename Table2::value_type>;

  splice(Table1& t1, Table2& t2) : m_table1{t1}, m_table2{t2} {}

  template <typename TableIter1, typename TableIter2, typename EndIter>
  class iterator {
   public:
    constexpr iterator(TableIter1 iter1, TableIter2 iter2, EndIter end)
        : m_iter1{iter1}, m_iter2{iter2}, m_end{end} {}

    constexpr iterator operator++() noexcept {
      while (m_iter1 != m_end || m_iter2 != m_end || m_iter1 != m_iter2) {
        if (m_iter1 < m_iter2) {
          ++m_iter1;
        } else if (m_iter1 > m_iter2) {
          ++m_iter2;
        }
      }
      return *this;
    }

    constexpr iterator operator++(int) noexcept {
      auto tmp = *this;
      (*this)++;
      return tmp;
    }

    constexpr value_type operator*() {
      return std::make_pair(*m_iter1, *m_iter2);
    }

   private:
    const EndIter m_end;
    TableIter1 m_iter1;
    TableIter2 m_iter2;
  };

  auto begin() {
    if (m_table1.size() <= m_table2.size()) {
      return iterator(m_table1.begin(), m_table2.begin(), m_table1.end());
    }
    return iterator(m_table1.begin(), m_table2.begin(), m_table2.end());
  }

 private:
  Table1& m_table1;
  Table2& m_table2;
};

}  // namespace xen

#endif  // PROCESS_JOIN_HPP_