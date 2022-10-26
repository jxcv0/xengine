#ifndef ECMAP_H_
#define ECMAP_H_

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

/**
 * @brief Very simple unsorted associative container.
 */
template <std::size_t N, typename KeyType, typename ValueType>
class ECMap {
 public:
  ECMap() = default;
  ~ECMap() = default;

  /**
   * @brief map a key to a value.
   *        TODO these should probably be forwarded rval refs or something.
   *
   * @param key The key of the new pair.
   * @param type The type of the new pair.
   */
  void assign(KeyType key, ValueType value) {
    if (m_size == N) {
      throw std::out_of_range("maximum size of map reached");
    }
    m_pairs[m_size++] = { key, value };
  }

  /**
   * @brief Remove a pair by key.
   *
   * @param k The key to search for.
   */
  void remove_by_key(KeyType k) {
    for (auto i = 0; i < m_size; i++) {
      if (m_pairs[i].m_key == k) {
        m_pairs[i] = m_pairs[m_size]; // swap position with last pair.
        m_size--;
      }
    }
  }

  /**
   * @brief Remove a pair by value.
   *
   * @param b The value to search for.
   */
  void remove_by_value(ValueType v) {
    for (auto i = 0; i < m_size; i++) {
      if (m_pairs[i].m_value == v) {
        m_pairs[i] = m_pairs[m_size]; // swap position with last pair.
        m_size--;
      }
    }
  }

  /**
   * @brief Find a value by a key with a linear search.
   *
   * @param k The key to search for.
   * @return The value associated with k.
   */
  ValueType find_by_key(KeyType k) const {
    for (auto i = 0; i < m_size; i++) {
      if (m_pairs[i].m_key == k) {
        return m_pairs[i].m_value;
      } else {
        throw std::runtime_error("key not found");
      }
    }
  }

  /**
   * @brief Find a key by a value with a linear search.
   *
   * @param v The value to search for.
   * @return The key associated with a value.
   */
  KeyType find_by_value(ValueType v) const {
    for (auto i = 0; i < m_size; i++) {
      if (m_pairs[i].m_value == v) {
        return m_pairs[i].m_key;
      } else {
        throw std::runtime_error("value not found");
      }
    }
  }

 private:
  /**
   * @brief A key and a value.
   */
  struct Pair {
    KeyType m_key;
    ValueType m_value;
  };

  Pair m_pairs[N]{0};
  std::size_t m_size{0};
};

#endif  // ECMAP_H_
