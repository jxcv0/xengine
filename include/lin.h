#ifndef LIN_H_
#define LIN_H_

#include <math.h>
#include <stddef.h>

#define IDENTITY_MAT4_INITIALIZER                            \
  {                                                          \
    {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, { 0, 0, 0, 1 } \
  }

typedef struct mat4 {
  float elem[4][4];
} mat4_t;

typedef struct vec4 {
  union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };
    float elem[4];
  };
} vec4_t;

typedef struct vec3 {
  union {
    struct {
      float x;
      float y;
      float z;
    };
    float elem[3];
  };
} vec3_t;

typedef struct vec2 {
  union {
    struct {
      float x;
      float y;
    };
    float elem[2];
  };
} vec2_t;

/**
 * @brief Print a matrix to stdout.
 *
 * @param m The matrix to print.
 */
void printm4(const mat4_t m);

/**
 * @brief Print the contents of a vector.
 *
 * @param v The elements of the vector.
 * @param n The number of dimensions of the vector.
 */
void printvn(const float *v, size_t n);

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
mat4_t identitym4(void);

/**
 * @brief Normalize a vec3.
 *
 * @param vec The vec3 to normalize.
 * @return A normalized vec.
 */
vec3_t normalizev3(const vec3_t vec);

/**
 * @brief Get the dot product of 2 vec3's.
 *
 * @param v1 The first vec3.
 * @param v2 The second vec3.
 * @return The dot product of v1 and v2.
 */
float dotv3(const vec3_t v1, const vec3_t v2);

/**
 * @brief Compare 2 vec3s
 * @param v1
 * @param v2
 * @return 1 if the vectors are the same, otherwise 0.
 */
int cmpv3(const vec3_t v1, const vec3_t v2);

/**
 * @brief Compare 2 vec2s
 * @param v1
 * @param v2
 * @return 1 if the vectors are the same, otherwise 0.
 */
int cmpv2(const vec2_t v1, const vec2_t v2);

/**
 * @brief Get the dot product of 2 vec4's
 *
 * @param v1 The first vec4.
 * @param v2 The second vec4.
 * @return The dot product of v1 and v2.
 */
float dotv4(const vec4_t v1, const vec4_t v2);

/**
 * @brief Cross 2 vec3's.
 *
 * @param v1 The first vec3.
 * @param v2 The second vec3.
 * @return The cross product of v1 and v2.
 */
vec3_t crossv3(const vec3_t v1, const vec3_t v2);

/**
 * @brief Get the product of 2 4 x 4 matrices
 *
 * @param m1 The first mat4.
 * @param m2 The second mat4.
 * @return The product of m1 and m2.
 */
mat4_t productm4(const mat4_t m1, const mat4_t m2);

/**
 * @brief Get the product of a 4x4 matrix and a vec4.
 *
 * @param m The matrix.
 * @param v The vector.
 * @return The product of m and v
 */
vec4_t productm4v4(const mat4_t m, const vec4_t v);

/**
 * @brief TODO
 */
mat4_t perspective(const float fov, const float aspect_ratio, const float near,
                   const float far);

/**
 * @brief Create a view matrix.
 *
 * @param eye The direction of the view.
 * @param ctr The position of the view.
 * @param up The up direction of the view.
 * @return A view matrix.
 */
mat4_t look_at(const vec3_t eye, const vec3_t ctr, const vec3_t up);

/**
 * @brief Translate a mat4 by a vec3.
 *
 * @param m The matrix to translate.
 * @param v The translation vector.
 * @return The translated matrix.
 */
mat4_t translate(const mat4_t m, const vec3_t v);

/**
 * @brief Rotate a matrix about and axis (using unit quaternion).
 *
 * @param m The matrix to apply the rotation to.
 * @param axis The axis of the rotation.
 * @param rads The rotation angle in radians.
 * @return The rotated matrix.
 */
mat4_t rotate(const mat4_t m, const vec3_t axis, const float rads);

/**
 * @brief Scale a transformation matrix by a vector v.
 *
 * @param m The matrix to transform.
 * @param v The scaling vector.
 * @return The scaled matrix.
 */
mat4_t scale(const mat4_t m, const vec3_t v);

#endif  // LIN_H_
