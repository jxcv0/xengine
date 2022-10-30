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
   * @brief Get the cross product of 2 Mat4's.
   *
   * @param m1 The first matrix.
   * @param m2 The second matrix.
   * @return A matrix containing the cross product.
   */
  constexpr inline friend auto operator*(const Mat4& m1, const Mat4& m2) {
    Mat4 result(0.0f);
    // result.values[0][1] = dot_vec4(m1[0]), col_mat4(m2, 1));
    // result.values[0][2] = dot_vec4(row_mat4(m1, 0), col_mat4(m2, 2));
    // result.values[0][3] = dot_vec4(row_mat4(m1, 0), col_mat4(m2, 3));

    // result.values[1][0] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 0));
    // result.values[1][1] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 1));
    // result.values[1][2] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 2));
    // result.values[1][3] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 3));

    // result.values[2][0] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 0));
    // result.values[2][1] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 1));
    // result.values[2][2] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 2));
    // result.values[2][3] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 3));

    // result.values[3][0] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 0));
    // result.values[3][1] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 1));
    // result.values[3][2] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 2));
    // result.values[3][3] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 3));
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
  /**
   * @brief Get a column of the matrix.
   *
   * @param i The index of the column to return.
   * @return The column at index i.
   */
  constexpr inline Vec4 col(std::size_t i) const noexcept {
    assert(i < 4);
    return Vec4(m_data[0][i], m_data[1][i], m_data[2][i], m_data[3][i]);
  }

  float m_data[4][4] = {0};
};

#endif  // MAT4_H_
