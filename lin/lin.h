#ifndef LIN_H_
#define LIN_H_

#include <cmath>
#include <functional>
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

/**
 * @brief Create perspective matrix.
 *
 * @param fov The field of view in degrees.
 * @param near The near clip distance.
 * @param far The far clip distance.
 * @param aspect_ratio The aspect ratio of the frustum
 */
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
 * @brief Create a view matrix.
 *
 * @param eye The direction of the view.
 * @param ctr The position of the view.
 * @param up The up direction of the view.
 * @return A view matrix.
 */
inline Mat4 look_at(const Vec3& eye, const Vec3& ctr, const Vec3& up) {
  auto f = (ctr - eye).normalize();
  auto s = (f * up).normalize();
  auto u = s * f;

  Mat4 mat(1.0f);
  mat[0][0] = s[0];
  mat[1][0] = s[1];
  mat[2][0] = s[2];

  mat[0][1] = u[0];
  mat[1][1] = u[1];
  mat[2][1] = u[2];

  mat[0][2] = -f[0];
  mat[1][2] = -f[1];
  mat[2][2] = -f[2];

  mat[3][0] = -dot(s, eye);
  mat[3][1] = -dot(u, eye);
  mat[3][2] = dot(f, eye);

  return mat;
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

/**
 * @brief Rotate a matrix about a vector.
 *
 * @param m The matrix to apply the rotation to.
 * @param axis The axis of the rotation.
 * @param angle The rotation angle in degrees.
 * @return A rotation matrix.
 */
constexpr Mat4 rotate(const Mat4& m, const Vec3& axis, float angle) {
  auto a = radians(angle);
  float c = std::cos(angle);
  float s = std::sin(angle);
  float t = 1.0f - c;
  auto temp = axis * t;

  Mat4 q(0.0f);
  q[0][0] = c + temp[0] * axis[0];
  q[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
  q[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

  q[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
  q[1][1] = c + temp[1] * axis[1];
  q[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

  q[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
  q[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
  q[2][2] = c + temp[2] * axis[2];

  auto r = m * q;
  r[3][0] = m[3][0];
  r[3][1] = m[3][1];
  r[3][2] = m[3][2];
  r[3][3] = m[3][3];

  return r;
}

}  // namespace lin

#endif  // LIN_H_
