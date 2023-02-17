#ifndef LIN_H_
#define LIN_H_

#include <math.h>

#define IDENTITY_MAT4 {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

#ifdef __cplusplus
extern "C" {
#endif

typedef float mat4[4][4];
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

/**
 * @brief Convert an angle from degrees to radians. Not exact.
 *
 * @param degrees The angle in degrees.
 * @return A floating point representing the angle in radians.
 */
float radians(const float degrees);

/**
 * @brief Make a mat4 into an identity matrix.
 *
 * @param m The mat4.
 */
void identity_mat4(mat4 m);

/**
 * @brief Normalize a vec3.
 *
 * @param vec The vec3 to normalize.
 */
void normalize_vec3(vec3 vec);

/**
 * @brief Get the dot product of 2 vec3's.
 *
 * @param v1 The first vec3.
 * @param v2 The second vec3.
 * @return The dot product of v1 and v2.
 */
float dot_vec3(const vec3 v1, const vec3 v2);

/**
 * @brief Get the dot product of 2 vec4's
 *
 * @param v1 The first vec4.
 * @param v2 The second vec4.
 * @return The dot product of v1 and v2.
 */
float dot_vec4(const vec4 v1, const vec4 v2);

/**
 * @brief Cross 2 vec3's.
 *
 * @param dest The vec3 to store the result.
 * @param v1 The first vec3.
 * @param v2 The second vec3.
 */
void cross_vec3(vec3 dest, const vec3 v1, const vec3 v2);

/**
 * @brief Multiply 2 mat4's.
 *
 * @param dest The mat4 to store the result.
 * @param m1 The first mat4.
 * @param m2 The second mat4.
 */
void product_mat4(mat4 dest, const mat4 m1, const mat4 m2);

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
 * @param mat The matrix to store the result in.
 * @param eye The direction of the view.
 * @param ctr The position of the view.
 * @param up The up direction of the view.
 * @return A view matrix.
 */
void look_at(mat4 mat, const vec3 eye, const vec3 ctr, const vec3 up);

/**
 * @brief Translate a mat4 by a vec3.
 *
 * @param m The matrix to translate.
 * @param v The translation vector.
 */
void translate(mat4 m, const vec3 v);

/**
 * @brief Rotate a transformation matrix.
 *
 * @param dest The matrix to store the result.
 * @param m The matrix to apply the rotation to.
 * @param axis The axis of the rotation.
 * @param angle The rotation angle in degrees.
 */
void rotate(mat4 dest, const mat4 m, vec3 axis, const float angle);

#ifdef __cplusplus
}
#endif

#endif  // LIN_H_
