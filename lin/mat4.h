#ifndef MAT4_H_
#define MAT4_H_

#include <cmath>
#include <initializer_list>
#include <ostream>

/**
 * @brief A 4 x 4 dimensional matrix.
 */
class Mat4 {
 public:
  /**
   * @brief Construct a matrix with a value in the identity positihons.
   * 
   * @param f The value to construct with.
   */
  explicit constexpr inline Mat4(float f) noexcept {
    m_data[0][0] = f;
    m_data[1][1] = f;
    m_data[2][2] = f;
    m_data[3][3] = f;
  }

  /**
   * @brief Subscript operator to access underlying array.
   *
   * @param i The index to acces;
   * @return The value at index i;
   */
  constexpr inline auto operator[](int i) const noexcept {
    return m_data[i];
  }

  friend std::ostream& operator<<(std::ostream& os, const Mat4& m) {
    os << "{ ";
    for (auto i = 0; i < 4; i++) {
      os << "{ " << m.m_data[0] << ", " << m.m_data[1] << ", " << m.m_data[2]
         << ", " << m.m_data[3]
         << " }";
    }
    os << " }";
    return os;
  }

 private:
  float m_data[4][4] = {0};
};

#endif  // MAT4_H_
