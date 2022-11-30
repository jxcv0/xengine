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
constexpr Mat4 perspective(const float fov, const float aspect_ratio,
                           const float near, const float far) {
  const auto t = std::tan(fov / 2.0f);

  Mat4 result(0.0f);
  result[0][0] = 1.0f / (aspect_ratio * t);
  result[1][1] = 1.0f / t;
  result[2][2] = -(far + near) / (far - near);
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
inline Mat4 look_at(const Vec3 &eye, const Vec3 &ctr, const Vec3 &up) {
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
constexpr Mat4 translate(const Mat4 &m, const Vec3 &v) {
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
inline Mat4 rotate(const Mat4 &m, const Vec3 &v, float angle) {
  const auto a = angle;
  const auto c = std::cos(a);
  const auto s = std::sin(a);

  Mat4 result;
  Vec3 axis = v.normalize();

  result[0][0] = c + (1.0f - c) * axis.x() * axis.x();
  result[0][1] = (1.0f - c) * axis.x() * axis.y() + s * axis.z();
  result[0][2] = (1.0f - c) * axis.x() * axis.z() - s * axis.y();
  result[0][3] = 0.0f;

  result[1][0] = (1.0f - c) * axis.y() * axis.x() - s * axis.z();
  result[1][1] = c + (1.0f - c) * axis.y() * axis.y();
  result[1][2] = (1.0f - c) * axis.y() * axis.z() + s * axis.x();
  result[1][3] = 0.0f;

  result[2][0] = (1.0f - c) * axis.z() * axis.x() + s * axis.y();
  result[2][1] = (1.0f - c) * axis.z() * axis.y() - s * axis.x();
  result[2][2] = c + (1.0f - c) * axis.z() * axis.z();
  result[2][3] = 0.0f;

  result[3][0] = 0.0f;
  result[3][1] = 0.0f;
  result[3][2] = 0.0f;
  result[3][3] = 1.0f;
  return m * result;
}

}  // namespace lin

#endif  // LIN_H_
