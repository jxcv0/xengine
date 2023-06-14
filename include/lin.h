#ifndef LIN_H_
#define LIN_H_

#include <math.h>
#include <stddef.h>

#define IDENTITY_MAT4_INITIALIZER                            \
  {                                                          \
    {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, { 0, 0, 0, 1 } \
  }
static const float IDENTITY_MAT4[4][4] = IDENTITY_MAT4_INITIALIZER;

typedef float mat4[4][4];
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

/**
 * @brief Print a matrix to stdout.
 *
 * @param m The matrix to print.
 */
void print_mat4(const mat4 m);

/**
 * @brief TODO
 */
void print_vec(const float *v, size_t n);

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
 * @brief Copy a vec3 at src into dest.
 * @param dest The destination vector.
 * @param src The source vector.
 */
void copy_vec3(vec3 dest, const vec3 src);

/**
 * @brief Compare 2 vec3s
 * @param v1
 * @param v2
 * @return 1 if the vectors are the same, otherwise 0.
 */
int cmp_vec3(vec3 v1, vec3 v2);

/**
 * @brief Copy a vec2 at src into dest.
 * @param dest The destination vector.
 * @param src The source vector.
 */
void copy_vec2(vec2 dest, const vec2 src);

/**
 * @brief Compare 2 vec2s
 * @param v1
 * @param v2
 * @return 1 if the vectors are the same, otherwise 0.
 */
int cmp_vec2(vec2 v1, vec2 v2);

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
void product_mat4(float dest[4][4], const float m1[4][4], const float m2[4][4]);

/**
 * @brief TODO
 */
void product_vec4(float dest[4], const float mat[4][4], const float vec[4]);

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
void rotate(float dest[4][4], const float m[4][4], float axis[3], float angle);

/**
 * @brief Scale a tramsformation matrix by a vector v.
 *
 * @param dest The matrix to store the result.
 * @param m The matrix to transform.
 * @param v The scaling vector.
 */
void scale(mat4 dest, const mat4 m, const vec3 v);

#endif  // LIN_H_
