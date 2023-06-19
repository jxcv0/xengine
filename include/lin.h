#ifndef LIN_H_
#define LIN_H_

#include <math.h>
#include <stddef.h>

#define IDENTITY_MAT4_INITIALIZER                            \
  {                                                          \
    {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, { 0, 0, 0, 1 } \
  }

typedef struct mat4 {
  union {
    float elem[4][4];
    float *ptr;
  };
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
    float *ptr;
  };
} vec4_t;

struct vec3 {
  union {
    struct {
      float x;
      float y;
      float z;
    };
    float elem[3];
    float *ptr;
  };
} vec3_t;

struct vec2 {
  union {
    struct {
      float x;
      float y;
    };
    float elem[2];
    float *ptr;
  };
} vec2_t;

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
 * @return A normalized vec.
 */
vec3_t normalize_vec3(const vec3_t vec);

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
 * @param v1 The first vec3.
 * @param v2 The second vec3.
 * @return The cross product of v1 and v2.
 */
vec3_t cross_vec3(const vec3_t v1, const vec3_t v2);

/**
 * @brief Multiply 2 mat4's.
 *
 * @param m1 The first mat4.
 * @param m2 The second mat4.
 * @return The product of m1 and m2.
 */
mat4_t product_mat4(const mat4_t m1, const mat4_t m2);

/**
 * @brief TODO
 */
void product_vec4(float dest[4], float mat[4][4], float vec[4]);

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
mat4_t look_at(const vec3_t eye, const vec3_t ctr, const vec3_t up);

/**
 * @brief Translate a mat4 by a vec3.
 *
 * @param m The matrix to translate.
 * @param v The translation vector.
 */
void translate(mat4 m, const vec3 v);

/**
 * @brief Create a rotation matrix.
 *
 * @param dest The matrix to store the result.
 * @param m The matrix to apply the rotation to.
 * @param axis The axis of the rotation.
 * @param angle The rotation angle in degrees.
 */
void rotate(float dest[4][4], float m[4][4], float axis[3], float angle);

/**
 * @brief Scale a transformation matrix by a vector v.
 *
 * @param dest The matrix to store the result.
 * @param m The matrix to transform.
 * @param v The scaling vector.
 */
void scale(float dest[4][4], float m[4][4], float v[3]);

#endif  // LIN_H_
