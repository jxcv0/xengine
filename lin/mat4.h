#ifndef MAT4_H_
#define MAT4_H_

#include <cmath>
#include <initializer_list>
#include <ostream>

#include "vec4.h"

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
   * @return The 4 dimentional vector at index i;
   */
  constexpr inline auto operator[](int i) const noexcept {
    return Vec4(m_data[i][0], m_data[i][1], m_data[i][2], m_data[i][3]);
  }

  /**
   * @brief Stream operator for printing out the matrix.
   *
   * @param os The ostream.
   * @param m The matrix to print out.
   * @retun The ostream.
   */
  friend std::ostream& operator<<(std::ostream& os, const Mat4& m) {
    os << "{ " << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << " }";
    return os;
  }

 private:
  float m_data[4][4] = {0};
};

#endif  // MAT4_H_
