#ifndef VEC4_H_
#define VEC4_H_

#include <cmath>
#include <initializer_list>
#include <ostream>

/**
 * @brief A 4 dimensional vector.
 */
class Vec4 {
 public:
  Vec4() = default;

  /**
   * @brief Construct a Vec4 from 4 floats.
   *
   * @param x The initial x component.
   * @param y The initial y component.
   * @param z The initial z component.
   * @param w The initial w component.
   */
  constexpr inline Vec4(float x, float y, float z, float w)
      : m_data{x, y, z, w} {}

  /**
   * @brief Access Vec4 with subscript operator.
   *
   * @param i The index to access.
   * @return The value at index i.
   */
  constexpr inline auto operator[](int i) const { return m_data[i]; }

  /**
   * @brief Access Vec4 with subscript operator.
   *
   * @param i The index to access.
   * @return A reference to the value at index i.
   */
  constexpr inline auto &operator[](int i) { return m_data[i]; }

  /**
   * @brief Set a component to a value.
   *
   * @param i The index of the component.
   * @param f The new value.
   */
  constexpr inline void set(int i, float f) noexcept { m_data[i] = f; }

  /**
   * @brief Comparison operator.
   *
   * @param other The other Vec3 to compare against.
   */
  constexpr inline bool operator==(const Vec4 &other) const noexcept {
    return (m_data[0] == other[0]) && (m_data[1] == other[1]) &&
           (m_data[2] == other[2]) && (m_data[3] == other[3]);
  }

  /**
   * @brief Get the x component of the Vec4
   *
   * @return A floating point value.
   */
  constexpr inline auto x() const noexcept { return m_data[0]; }

  /**
   * @brief Get the y component of the Vec4
   *
   * @return A floating point value.
   */
  constexpr inline auto y() const noexcept { return m_data[1]; }

  /**
   * @brief Get the z component of the Vec4
   *
   * @return A floating point value.
   */
  constexpr inline auto z() const noexcept { return m_data[2]; }

  /**
   * @brief Get the w component of the Vec4
   *
   * @return A floating point value.
   */
  constexpr inline auto w() const noexcept { return m_data[3]; }

  /**
   * @brief Multiply the Vec4 by a scalar.
   *
   * @param scalar The scalar value.
   * @return A new Vec4 containing the result.
   */
  constexpr inline auto operator*(float scalar) const noexcept {
    return Vec4(m_data[0] * scalar, m_data[1] * scalar, m_data[2] * scalar,
                m_data[3] * scalar);
  }

  /**
   * @brief Add together 2 vec4s.
   *
   * @param v1 The first vec4.
   * @param v2 The other vec4.
   * @return A new vec that is the sum of the two vectors
   */
  constexpr inline friend Vec4 operator+(const Vec4 &v1, const Vec4 &v2) {
    return Vec4((v1.m_data[0] + v2.m_data[0]), (v1.m_data[1] + v2.m_data[1]),
                (v1.m_data[2] + v2.m_data[2]), v1.m_data[3] + v2.m_data[3]);
  }

  /**
   * @brief Sum the Vec4 with another.
   *
   * @param other The other vector.
   */
  constexpr inline void operator+=(const Vec4 &other) noexcept {
    m_data[0] += other.x();
    m_data[1] += other.y();
    m_data[2] += other.z();
    m_data[3] += other.w();
  }

  /**
   * @brief Subtract 2 vec3s.
   *
   * @param v1 The first vec3.
   * @param v2 The other vec3.
   * @return A new vec that is the sum of the two vectors
   */
  constexpr inline friend Vec4 operator-(const Vec4 &v1, const Vec4 &v2) {
    return Vec4((v1.m_data[0] - v2.m_data[0]), (v1.m_data[1] - v2.m_data[1]),
                (v1.m_data[2] - v2.m_data[2]), (v1.m_data[3] - v2.m_data[3]));
  }

  /**
   * @brief Subtract a Vec4 with from the vector.
   *
   * @param other The other vector.
   */
  constexpr inline void operator-=(const Vec4 &other) noexcept {
    m_data[0] -= other.x();
    m_data[1] -= other.y();
    m_data[2] -= other.z();
    m_data[3] -= other.w();
  }

  /**
   * @brief Get the dot product of two Vec4s
   *
   * @param v1 The first Vec4.
   * @param v2 The second Vec4.
   * @return The dot product.
   */
  constexpr inline friend auto dot(const Vec4 &v1, const Vec4 &v2) {
    return (v1.m_data[0] * v2.m_data[0]) + (v1.m_data[1] * v2.m_data[1]) +
           (v1.m_data[2] * v2.m_data[2]) + (v1.m_data[3] * v2.m_data[3]);
  }

  /**
   * @brief Multiply the Vec4 with a scalar.
   *
   * @param scalar The value to multiply the values by.
   */
  constexpr inline void operator*=(float scalar) noexcept {
    m_data[0] *= scalar;
    m_data[1] *= scalar;
    m_data[2] *= scalar;
    m_data[3] *= scalar;
  }

  /**
   * @brief return a normalized version of the vector with magnitude of 1.0f.
   *
   * @return A vec3 with normalized values.
   */
  auto normalize() const noexcept {
    const auto l = std::sqrt((m_data[0] * m_data[0]) + (m_data[1] * m_data[1]) +
                             (m_data[2] * m_data[2]) + (m_data[3] * m_data[3]));
    return Vec4(m_data[0] / l, m_data[1] / l, m_data[2] / l, m_data[3] / l);
  }

  /**
   * @brief Stream operator to aid in debugging.
   *
   * @param os Outstream to append stream to.
   * @param v The vector to stream.
   * @return The ostream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Vec4 &v) {
    os << "{ " << v.m_data[0] << ", " << v.m_data[1] << ", " << v.m_data[2]
       << ", " << v.m_data[3] << " }";
    return os;
  }

  /**
   * @brief Access the underlying array for OpenGL API.
   *
   * @return A const pointer to the underlying array.
   */
  const constexpr inline float *value_ptr() const noexcept {
    return &m_data[0];
  }

 private:
  float m_data[4] = {0};
};

#endif  // VEC4_H_
