#include "vec.hpp"

// /**
//  * @brief Check the equality of two vectors
//  * 
//  * @param rhs the other vector
//  * @return true if the magnitude and direction of the vectors are equal
//  * @return false if the magnitude and direction of the vectors are not equal
//  */
// bool ge::operator==(const vec3 &lhs, const vec3 &rhs) {
//     return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
// }

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
 * @brief Multiply the vector by a scalar
 * 
 * @param rhs the scalar value
 */
void ge::vec3::operator*=(const float &rhs) {
    this->x *= rhs;
    this->y *= rhs;
    this->z *= rhs;
}