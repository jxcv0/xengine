#ifndef MAT4_H_
#define MAT4_H_

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <ostream>

#include "vec4.h"

/**
 * @brief A 4 x 4 dimensional matrix.
 */
class Mat4 {
 public:
  Mat4() = default;

  /**
   * @brief Construct a matrix with a value in the identity positihons.
   *
   * @param f The value to construct with.
   */
  constexpr inline Mat4(float f) noexcept {
    m_data[0][0] = f;
    m_data[1][1] = f;
    m_data[2][2] = f;
    m_data[3][3] = f;
  }

  /**
   * @brief Subscript operator to access underlying array.
   *
   * @param i The index to access;
   * @return The 4 dimentional vector at index i;
   */
  constexpr inline auto operator[](int i) const noexcept { return m_data[i]; }

  /**
   * @brief Subscript operator to access underlying array.
   *
   * @param i The index to acces;
   * @return The 4 dimentional vector at index i;
   */
  constexpr inline auto& operator[](int i) noexcept { return m_data[i]; }

  /**
   * @brief Get the matrix product of 2 Mat4's and return the result in a new
   *        Mat4.
   *
   * @param m1 The first matrix.
   * @param m2 The second matrix.
   * @return A matrix containing the matrix product.
   */
  constexpr inline friend auto operator*(const Mat4& m1,
                                         const Mat4& m2) noexcept {
    Mat4 result(0.0f);
    result.m_data[0][0] = dot(m1[0], m2.col(0));
    result.m_data[0][1] = dot(m1[0], m2.col(1));
    result.m_data[0][2] = dot(m1[0], m2.col(2));
    result.m_data[0][3] = dot(m1[0], m2.col(3));

    result.m_data[1][0] = dot(m1[1], m2.col(0));
    result.m_data[1][1] = dot(m1[1], m2.col(1));
    result.m_data[1][2] = dot(m1[1], m2.col(2));
    result.m_data[1][3] = dot(m1[1], m2.col(3));

    result.m_data[2][0] = dot(m1[2], m2.col(0));
    result.m_data[2][1] = dot(m1[2], m2.col(1));
    result.m_data[2][2] = dot(m1[2], m2.col(2));
    result.m_data[2][3] = dot(m1[2], m2.col(3));

    result.m_data[3][0] = dot(m1[3], m2.col(0));
    result.m_data[3][1] = dot(m1[3], m2.col(1));
    result.m_data[3][2] = dot(m1[3], m2.col(2));
    result.m_data[3][3] = dot(m1[3], m2.col(3));
    return result;
  }

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

  /**
   * @brief Access the underlying array for OpenGL API.
   *
   * @return A const pointer to the underlying array.
   */
  const constexpr inline auto value_ptr() const noexcept {
    return m_data[0].value_ptr();
  }

 private:
  Vec4 m_data[4];
};

#endif  // MAT4_H_
