#ifndef LIN_H_
#define LIN_H_

#include <cmath>
#include <type_traits>
// #include <numbers>

#include "mat4.h"
#include "vec3.h"

/**
 * @brief Linear algebra functions.
 */
namespace lin {

/**
 * @brief Convert an angle from degrees to radians. Not exact.
 *
 * @param degrees The angle in degrees.
 * @return A floating point representing the angle in radians.
 */
constexpr inline float radians(float degrees) noexcept {
  // return degrees * (std::numbers::pi / 180.0f);
  return degrees * (M_PI / 180.0f);
}

constexpr Mat4 perspective(const float fov, const float near, const float far,
                           const float aspect_ratio) {
  Mat4 result(0.0f);
  auto tanhalffov = std::tan(radians(fov) / 2.0f);
  result[0][0] = 1.0f / (aspect_ratio * tanhalffov);
  result[1][1] = 1.0f / tanhalffov;
  result[2][2] = (far + near) / (near - far);
  result[2][3] = -1.0f;
  result[3][2] = -(2.0f * far * near) / (far - near);
  return result;
}

/**
 * @brief Create a translation matrix.
 *
 * TODO check this is correct
 * @param v The translation vector.
 * @return A translation matrix.
 */
constexpr Mat4 translate(const Mat4& m, const Vec3& v) {
  Mat4 result(m);
  result[3][0] += v[0];
  result[3][1] += v[1];
  result[3][2] += v[2];
  return result;
}

}  // namespace lin

#endif  // LIN_H_
