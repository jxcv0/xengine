/* MIT License
 *
 * Copyright (c) 2023 Jacob Micoud
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <mutex>
#include <vector>

namespace xen {

using oid_t = std::uint64_t;

template <typename T> class table {
public:
  table() = default;
  table(const table &) = delete;
  table(table &&) = delete;

  void insert(oid_t entity, const T &val) {
    auto it = m_arr.cbegin();
    for (; it != m_arr.cend(); ++it) {
      if (entity == it->first) {
        return; // no duplucates
      }

      if (entity < it->first) {
        break;
      }
    }
    m_arr.insert(it, {entity, val});
  }

  void insert(oid_t entity) {
    auto it = m_arr.cbegin();
    for (; it != m_arr.cend(); ++it) {
      if (entity == it->first) {
        return; // no duplucates
      }

      if (entity < it->first) {
        break;
      }
    }
    m_arr.insert(it, {entity, T{}});
  }

  void remove(oid_t entity) {
    auto it = m_arr.cbegin();
    for (; it != m_arr.cend(); ++it) {
      if (entity == it->first) {
        m_arr.erase(it);
        return;
      }
    }
  }

  T &operator[](std::size_t i) { return m_arr[i].second; }
  const T &operator[](std::size_t i) const { return m_arr[i].second; }

  std::size_t size() { return m_arr.size(); }

  auto begin() { return m_arr.begin(); }
  auto end() { return m_arr.end(); }

  std::mutex &mutex() { return m_mutex; }

private:
  std::mutex m_mutex;
  std::vector<std::pair<oid_t, T>> m_arr;
};

template <typename T1, typename T2, typename Func>
void iterate_tables(table<T1> &t1, table<T2> &t2, Func f) {
  std::lock_guard lk1(t1.mutex());
  std::lock_guard lk2(t2.mutex());
  auto it1 = std::begin(t1);
  auto it2 = std::begin(t2);
  // TODO parallelize
  while (it1 != std::end(t1) && it2 != std::end(t2)) {
    if (it1->first == it2->first) {
      f(it1->second, it2->second);
      ++it1;
      ++it2;
    } else {
      if (it1->first < it2->first)
        ++it1;
      if (it1->first > it2->first)
        ++it2;
    }
  }
}

template <typename T1, typename T2, typename T3, typename Func>
void iterate_tables(table<T1> &t1, table<T2> &t2, table<T3> &t3, Func f) {
  std::lock_guard lk1(t1.mutex());
  std::lock_guard lk2(t2.mutex());
  std::lock_guard lk3(t3.mutex());
  auto it1 = std::begin(t1);
  auto it2 = std::begin(t2);
  auto it3 = std::begin(t3);
  // TODO parallelize
  while (it1 != std::end(t1) && it2 != std::end(t2) && it3 != std::end(t3)) {
    if (it1->first == it2->first && it2->first == it3->first) {
      f(it1->second, it2->second, it3->second);
      ++it1;
      ++it2;
      ++it3;
    } else {
      if (it1->first < it2->first || it1->first < it3->first)
        ++it1;
      if (it2->first < it1->first || it2->first < it3->first)
        ++it2;
      if (it3->first < it1->first || it3->first < it2->first)
        ++it3;
    }
  }
}

} // namespace xen

#endif // TABLE_HPP_
