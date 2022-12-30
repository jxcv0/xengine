#ifndef LIN_H_
#define LIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

typedef float mat4[4][4];
typedef float vec3[3];

/**
 * @brief Normalize a vec3.
 *
 * @param vec The vec3 to normalize.
 */
void normalize_vf3(vec3 vec);

/**
 * @brief Convert an angle from degrees to radians. Not exact.
 *
 * @param degrees The angle in degrees.
 * @return A floating point representing the angle in radians.
 */
float radians(const float degrees);

/**
 * @brief Create perspective matrix.
 *
 * @param mat The float[16] to store the resutlting matrix in.
 * @param fov The field of view in degrees.
 * @param near The near clip distance.
 * @param far The far clip distance.
 * @param aspect_ratio The aspect ratio of the frustum
 */
void perspective(mat4 mat, const float fov, const float aspect_ratio,
                 const float near, const float far);

/**
 * @brief Create a view matrix.
 *
 * @param eye The direction of the view.
 * @param ctr The position of the view.
 * @param up The up direction of the view.
 * @return A view matrix.
 */
void look_at(mat4 mat, const vec3 eye, const vec3 ctr, const vec3 up);

// /**
//  * @brief Create a translation matrix.
//  *
//  * TODO check this is correct
//  * @param v The translation vector.
//  * @return A translation matrix.
//  */
// constexpr Mat4 translate(const Mat4 &m, const Vec3 &v) {
//   Mat4 result(m);
//   result[3][0] += v[0];
//   result[3][1] += v[1];
//   result[3][2] += v[2];
//   return result;
// }
//
// /**
//  * @brief Rotate a matrix about a vector.
//  *
//  * @param m The matrix to apply the rotation to.
//  * @param axis The axis of the rotation.
//  * @param angle The rotation angle in degrees.
//  * @return A rotation matrix.
//  */
// inline Mat4 rotate(const Mat4 &m, const Vec3 &v, float angle) {
//   const auto a = angle;
//   const auto c = std::cos(a);
//   const auto s = std::sin(a);
//
//   Mat4 result;
//   Vec3 axis = v.normalize();
//
//   result[0][0] = c + (1.0f - c) * axis.x() * axis.x();
//   result[0][1] = (1.0f - c) * axis.x() * axis.y() + s * axis.z();
//   result[0][2] = (1.0f - c) * axis.x() * axis.z() - s * axis.y();
//   result[0][3] = 0.0f;
//
//   result[1][0] = (1.0f - c) * axis.y() * axis.x() - s * axis.z();
//   result[1][1] = c + (1.0f - c) * axis.y() * axis.y();
//   result[1][2] = (1.0f - c) * axis.y() * axis.z() + s * axis.x();
//   result[1][3] = 0.0f;
//
//   result[2][0] = (1.0f - c) * axis.z() * axis.x() + s * axis.y();
//   result[2][1] = (1.0f - c) * axis.z() * axis.y() - s * axis.x();
//   result[2][2] = c + (1.0f - c) * axis.z() * axis.z();
//   result[2][3] = 0.0f;
//
//   result[3][0] = 0.0f;
//   result[3][1] = 0.0f;
//   result[3][2] = 0.0f;
//   result[3][3] = 1.0f;
//   return m * result;
// }

#ifdef __cplusplus
}
#endif

#endif  // LIN_H_
