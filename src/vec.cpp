#include "vec.hpp"

#include <cmath>

/**
 * @brief Sum a vector by another vector
 * 
 * @param rhs the other vector
 */
void ge::vec3::operator+=(const vec3 &rhs) {
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
ge::vec3 ge::vec3::operator+(const vec3 &rhs) {
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
void ge::vec3::operator-=(const vec3 &rhs) {
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
ge::vec3 ge::vec3::operator-(const vec3 &rhs) {
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
void ge::vec3::operator*=(const vec3 &rhs) {
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
ge::vec3 ge::vec3::operator*(const vec3 &rhs) {
    vec3 v{
        this->x * rhs.x,
        this->y * rhs.y,
        this->z * rhs.z
    };
    return v;
}
        
/**
 * @brief Multiply the vector by a scalar
 * 
 * @param s the scalar value
 */
void ge::vec3::operator*=(const float &s) {
    this->x *= s;
    this->y *= s;
    this->z *= s;
}

/**
 * @brief Get the magnitude of the vector
 * 
 * @return float the magnitude
 */
float ge::vec::mag_sqd(const vec3 &rhs) {
    return (rhs.x * rhs.x) + (rhs.y * rhs.y) + (rhs.z * rhs.z);
}