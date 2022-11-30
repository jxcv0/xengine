#ifndef VEC3_H_
#define VEC3_H_

#include <cmath>
#include <initializer_list>
#include <ostream>

/**
 * @brief A 3 dimensional vector.
 */
class Vec3 {
 public:
  Vec3() = default;

  /**
   * @brief Construct a Vec3 from 3 float.
   *
   * @param x The initial x component.
   * @param y The initial y component.
   * @param z The initial z component.
   */
  constexpr inline Vec3(float x, float y, float z) : m_data{x, y, z} {}

  /**
   * @brief Access Vec3 with subscript operator.
   *
   * @param i The index to access.
   * @return The value at index i.
   */
  constexpr inline auto operator[](int i) const { return m_data[i]; }

  /**
   * @brief Comparison operator.
   *
   * @param other The other Vec2 to compare against.
   * @return true if the vectors are the same, otherwise false.
   */
  constexpr inline bool operator==(const Vec3 &other) const noexcept {
    return (m_data[0] == other[0]) && (m_data[1] == other[1]) &&
           (m_data[2] == other[2]);
  }

  /**
   * @brief Get the x component of the Vec3
   *
   * @return A floating point value.
   */
  constexpr inline auto x() const noexcept { return m_data[0]; }

  /**
   * @brief Get the y component of the Vec3
   *
   * @return A floating point value.
   */
  constexpr inline auto y() const noexcept { return m_data[1]; }

  /**
   * @brief Get the z component of the Vec3
   *
   * @return A floating point value.
   */
  constexpr inline auto z() const noexcept { return m_data[2]; }

  /**
   * @brief Multiply the Vec3 by a scalar.
   *
   * @param scalar The scalar value.
   * @return A new Vec3 containing the result.
   */
  constexpr inline auto operator*(float scalar) const noexcept {
    return Vec3(m_data[0] * scalar, m_data[1] * scalar, m_data[2] * scalar);
  }

  /**
   * @brief Add together 2 vec3s.
   *
   * @param v1 The first vec3.
   * @param v2 The other vec3.
   * @return A new vec that is the sum of the two vectors
   */
  constexpr inline friend Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return Vec3((v1.m_data[0] + v2.m_data[0]), (v1.m_data[1] + v2.m_data[1]),
                (v1.m_data[2] + v2.m_data[2]));
  }

  /**
   * @brief Sum the Vec3 with another.
   *
   * @param other The other vector.
   */
  constexpr inline void operator+=(const Vec3 &other) noexcept {
    m_data[0] += other.x();
    m_data[1] += other.y();
    m_data[2] += other.z();
  }

  /**
   * @brief Subtract 2 vec3s.
   *
   * @param v1 The first vec3.
   * @param v2 The other vec3.
   * @return A new vec that is the sum of the two vectors
   */
  constexpr inline friend Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3((v1.m_data[0] - v2.m_data[0]), (v1.m_data[1] - v2.m_data[1]),
                (v1.m_data[2] - v2.m_data[2]));
  }

  /**
   * @brief Subtract a Vec3 with from the vector.
   *
   * @param other The other vector.
   */
  constexpr inline void operator-=(const Vec3 &other) noexcept {
    m_data[0] -= other.x();
    m_data[1] -= other.y();
    m_data[2] -= other.z();
  }

  /**
   * @brief Get the dot product of two Vec3s
   *
   * @param v1 The first Vec3.
   * @param v2 The second Vec3.
   * @return The dot product.
   */
  constexpr inline friend auto dot(const Vec3 &v1, const Vec3 &v2) {
    return (v1.m_data[0] * v2.m_data[0]) + (v1.m_data[1] * v2.m_data[1]) +
           (v1.m_data[2] * v2.m_data[2]);
  }

  /**
   * @brief Multiply the Vec3 with a scalar.
   *
   * @param scalar The value to multiply the values by.
   */
  constexpr inline void operator*=(float scalar) noexcept {
    m_data[0] *= scalar;
    m_data[1] *= scalar;
    m_data[2] *= scalar;
  }

  /**
   * @brief Get the cross product of two Vec3s.
   *
   * @param v1 The first vec3.
   * @param v2 The other vec3.
   * @return A new Vec3 that contains the cross product.
   */
  constexpr inline friend Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return Vec3((v1.m_data[1] * v2.m_data[2]) - (v1.m_data[2] * v2.m_data[1]),
                (v1.m_data[2] * v2.m_data[0]) - (v1.m_data[0] * v2.m_data[2]),
                (v1.m_data[0] * v2.m_data[1]) - (v1.m_data[1] * v2.m_data[0]));
  }

  /**
   * @brief Cross the Vec3 with another.
   *
   * @param other The other Vec3
   */
  constexpr inline void operator*=(const Vec3 &other) noexcept {
    auto x = (m_data[1] * other[2]) - (m_data[2] * other[1]);
    auto y = (m_data[2] * other[0]) - (m_data[0] * other[2]);
    auto z = (m_data[0] * other[1]) - (m_data[1] * other[0]);
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  /**
   * @brief return a normalized version of the vector with magnitude of 1.0f.
   *
   * @return A vec3 with normalized values.
   */
  auto normalize() const noexcept {
    auto l = std::sqrt((m_data[0] * m_data[0]) + (m_data[1] * m_data[1]) +
                       (m_data[2] * m_data[2]));
    return Vec3(m_data[0] / l, m_data[1] / l, m_data[2] / l);
  }

  /**
   * @brief Stream operator to aid in debugging.
   *
   * @param os Outstream to append stream to.
   * @param v The vector to stream.
   * @return The ostream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
    os << "{ " << v.m_data[0] << ", " << v.m_data[1] << ", " << v.m_data[2]
       << " }";
    return os;
  }

  /**
   * @brief Access the underlying array for OpenGL API.
   *
   * @return A const pointer to the underlying array.
   */
  const constexpr inline auto value_ptr() const noexcept { return &m_data[0]; }

 private:
  float m_data[3] = {0};
};

#endif  // VEC3_H_
