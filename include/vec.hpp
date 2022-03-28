#ifndef _VEC_HPP_
#define _VEC_HPP_

namespace ge {

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
         * @brief Sum a vector by another vector
         * 
         * @param rhs the other vector
         */
        void operator+=(const vec3 &rhs);

        /**
         * @brief Subtract from a vector by another vector
         * 
         * @param rhs the other vector
         */
        void operator-=(const vec3 &rhs);

        /**
         * @brief Multiply the vector by another vector
         * 
         * @param rhs the other vector
         */
        void operator*=(const vec3 &rhs);
        
        /**
         * @brief Multiply the vector by a scalar
         * 
         * @param rhs the scalar value
         */
        void operator*=(const float &rhs);
    };
} // namespace ge

#endif // _VEC_HPP_