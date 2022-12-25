#ifndef VEC2_H_
#define VEC2_H_

#include <ostream>

/**
 * @brief A 2 dimensional vector.
 */
class Vec2 {
 public:
  Vec2() = default;

  /**
   * @brief Construct a Vec2 from 3 float.
   *
   * @param x The initial x component.
   * @param y The initial y component.
   * @param z The initial z component.
   */
  constexpr inline Vec2(float x, float y) : m_data{x, y} {}

  /**
   * @brief Access Vec2 with subscript operator.
   *
   * @param i The index to access.
   * @return The value at index i.
   */
  constexpr inline auto operator[](int i) const { return m_data[i]; }

  /**
   * @brief Comparison operator.
   *
   * @param other The other Vec3 to compare against.
   * @return true if the vectors are the same, otherwise false.
   */
  constexpr inline bool operator==(const Vec2 &other) const noexcept {
    return (m_data[0] == other[0]) && (m_data[1] == other[1]);
  }

  /**
   * @brief Multiply the vector by a scalar.
   *
   * @param scalar The scalar.
   * @return A new Vec2 scaled by the scalar.
   */
  constexpr inline Vec2 operator*(const float scalar) const noexcept {
    return Vec2(m_data[0] * scalar, m_data[1] * scalar);
  }

  /**
   * @brief Multiply the vector by a scalar.
   *
   * @param scalar The scalar.
   */
  constexpr inline void operator*=(const float scalar) noexcept {
    m_data[0] *= scalar;
    m_data[1] *= scalar;
  }

  /**
   * @breif Subtract a vec2 from this one.
   *
   * @param other The other Vec2.
   * @return The difference between this and the other Vec2.
   */
  constexpr inline auto operator-(const Vec2 &other) const noexcept {
    return Vec2(m_data[0] - other.m_data[0], m_data[1] - other.m_data[1]);
  }

  /**
   * @brief Get the x component of the Vec2
   *
   * @return A floating point value.
   */
  constexpr inline auto x() const noexcept { return m_data[0]; }

  /**
   * @brief Get the y component of the Vec2
   *
   * @return A floating point value.
   */
  constexpr inline auto y() const noexcept { return m_data[1]; }

  /**
   */
  friend std::ostream &operator<<(std::ostream &os, const Vec2 &v) {
    os << "{ " << v.m_data[0] << ", " << v.m_data[1] << " }";
    return os;
  }

  /**
   * @brief Access the underlying array for OpenGL API.
   *
   * @return A const pointer to the underlying array.
   */
  constexpr inline auto value_ptr() const noexcept { return &m_data[0]; }

 private:
  float m_data[2] = {0};
};

#endif  // VEC2_H_
