#include "vec.hpp"

#include <cmath>

/**
 * @brief Sum a vector by another vector
 * 
 * @param rhs the other vector
 */
void ge::vec::vec3::operator+=(const vec3 &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
}

/**
 * @brief Get the sum of two vectors
 * 
 * @param rhs the other vector
 * @return vec3 the new sum vector
 */
ge::vec::vec3 ge::vec::vec3::operator+(const vec3 &rhs) {
    vec3 v{
        this->x + rhs.x,
        this->y + rhs.y,
        this->z + rhs.z
    };
    return v;
}

/**
 * @brief Subtract from a vector by another vector
 * 
 * @param rhs the other vector
 */
void ge::vec::vec3::operator-=(const vec3 &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
}

/**
 * @brief Ger the difference of two vectors
 * 
 * @param rhs the other vector
 * @return vec3 the new difference vector
 */
ge::vec::vec3 ge::vec::vec3::operator-(const vec3 &rhs) {
    vec3 v{
        this->x - rhs.x,
        this->y - rhs.y,
        this->z - rhs.z
    };
    return v;
}

/**
 * @brief Multiply the vector by another vector
 * 
 * @param rhs the other vector
 */
void ge::vec::vec3::operator*=(const vec3 &rhs) {
    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
}

/**
 * @brief Get the product of two vectors
 * 
 * @param rhs the other vector
 * @return vec3 the new product vector
 */
ge::vec::vec3 ge::vec::vec3::operator*(const vec3 &rhs) {
    vec3 v{
        this->x * rhs.x,
        this->y * rhs.y,
        this->z * rhs.z
    };
    return v;
}

/**
 * @brief Get the product of a vector and a scalar
 * 
 * @param s the scalar value
 * @return the new scaled vector
 */
ge::vec::vec3 ge::vec::vec3::operator*(const float s) {
    vec3 v{
        this->x * s,
        this->y * s,
        this->z * s
    };
    return v;
}
        
/**
 * @brief Multiply the vector by a scalar
 * 
 * @param s the scalar value
 */
void ge::vec::vec3::operator*=(const float s) {
    this->x *= s;
    this->y *= s;
    this->z *= s;
}

/**
 * @brief Calculate the dot product of 2 vec3's
 * 
 * @param v1 the first vector
 * @param v2 the second vector
 * @return float the scalar dot product
 */
float ge::vec::dot_prod(const vec3 &v1, const vec3 &v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

/**
 * @brief Calculate the dot product of a vector and itself
 * 
 * @param v the vector
 * @return float the vectors dot product
 */
float ge::vec::dot_prod(const vec3 &v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);   
}

/**
 * @brief linearly interpolate a vector from two other vectors
 * 
 * @param v1 the first vector
 * @param v2 the second vector
 * @param b the weight
 * @return the new interpolated vector
 */
ge::vec::vec3 ge::vec::lerp(const vec3 &v1, const vec3 &v2, float b) {
    // TODO
}