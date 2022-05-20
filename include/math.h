#ifndef MATH_H
#define MATH_H

#include <iostream>

// TODO SIMD

namespace xen
{
    // take a wild guess
    float radians(float dg)
    {
        return dg * 0.0174533;
    }

    // 2 dimenstional vector
    struct Vec2
    {
        float x = 0;
        float y = 0;
    };

    // 3 dimentional vector
    struct Vec3
    {
        float operator*(float f) const
        {
            return (x * f) + (y * f) + (z * f);
        }

        float x = 0;
        float y = 0;
        float z = 0;
    };

    struct Vec4
    {
        float operator*(float f) const
        {
            return (x * f) + (y * f) + (z * f) + (z * a);
        }

        float x = 0;
        float y = 0;
        float z = 0;
        float a = 0;
    };

    struct Mat3
    {
        auto& operator[](size_t n)
        {
            static_assert(n < 3);
            return vals[n];
        }

        float vals[3][3];
    };

    // 4 dimensional matrix
    struct Mat4
    {
        auto& operator[](size_t n)
        {
            static_assert(n < 4);
            return vals[n];
        }

        Vec3 operator*(const Vec3& v) const
        {
            return {
                // TODO
            };
        }

        float vals[4][4]; // row then column
    };

    // get the dot product of 2 Vec3s
    float dot(const Vec3& a, const Vec3& b)
    {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    // get the cross product of 2 Vec3s
    Vec3 cross(const Vec3& a, const Vec3& b)
    {
        return {
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        };
    }

    // Mat4 translate()
    // {
        // return 
    // }

    // rotate a matrix about x
    Vec3 rotate_x(const Vec3& v, const float theta)
    {
        Mat3 rot;
        rot[0][0] = cos(theta);
        rot[0][1] = -sin(theta);
        rot[0][2] = 0;
        
        rot[1][0] = sin(theta);
        rot[1][1] = cos(theta);
        rot[1][2] = 0;

        rot[2][0] = 0;
        rot[2][1] = 0;
        rot[2][2] = 1.0f;

        return rot * v;
    }

    Vec3 rotate_y(const Vec3& v, const float theta)
    {
    }

    Vec3 rotate_z(const Vec3& v, const float theta)
    {
    }
} // namespace xen

#endif // MATH_H
