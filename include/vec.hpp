#ifndef _VEC_HPP_
#define _VEC_HPP_

namespace ge::vec {

    /**
     * @brief A 3 dimentional vector.
     * 
     */
    struct vec3 {

        float x;    // x magnitude
        float y;    // y magnitude
        float z;    // z magnitude

        /**
         * @brief Check the equality of two vectors
         * 
         * @param rhs the other vector
         * @return true if the magnitude and direction of the vectors are equal
         * @return false if the magnitude and direction of the vectors are not equal
         */
        friend bool operator==(const vec3 &lhs, const vec3 &rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }
        
        /**
         * @brief Increment a vector by another vector
         * 
         * @param rhs the other vector
         */
        void operator+=(const vec3 &rhs);

        /**
         * @brief Get the sum of two vectors
         * 
         * @param rhs the other vector
         * @return vec3 the new sum vector
         */
        vec3 operator+(const vec3 &rhs);

        /**
         * @brief Decrement the vector by another vector
         * 
         * @param rhs the other vector
         */
        void operator-=(const vec3 &rhs);

        /**
         * @brief Ger the difference of two vectors
         * 
         * @param rhs the other vector
         * @return vec3 the new difference vector
         */
        vec3 operator-(const vec3 &rhs);

        /**
         * @brief Multiply the vector by another vector
         * 
         * @param rhs the other vector
         */
        void operator*=(const vec3 &rhs);

        /**
         * @brief Get the product of two vectors
         * 
         * @param rhs the other vector
         * @return vec3 the new product vector
         */
        vec3 operator*(const vec3 &rhs);
        
        /**
         * @brief Get the product of a vector and a scalar
         * 
         * @param s the scalar value
         * @return the new scaled vector
         */
        vec3 operator*(const float s);

        /**
         * @brief Multiply the vector by a scalar
         * 
         * @param rhs the scalar value
         */
        void operator*=(const float rhs);

    };

    /**
     * @brief Calculate the dot product of 2 vec3's
     * 
     * @param v1 the first vector
     * @param v2 the second vector
     * @return float the scalar dot product
     */
    float dot_prod(const vec3 &v1, const vec3 &v2);

    /**
     * @brief Calculate the dot product of a vector and itself
     * 
     * @param v the vector
     * @return float the vectors dot product
     */
    float dot_prod(const vec3 &v);

    /**
     * @brief linearly interpolate a vector from two other vectors
     * 
     * @param v1 the first vector
     * @param v2 the second vector
     * @param b the weight
     * @return the new interpolated vector
     */
    vec3 lerp(const vec3 &v1, const vec3 &v2, float b);
} // namespace ge::vec

#endif // _VEC_HPP_