#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <cassert>

// TODO SIMD

namespace xen
{
    // take a wild guess
    float radians(float dg)
    {
        return dg * 0.0174533;
    }

    template<int n>
    struct Vec
    {
        auto& operator[](size_t index){ return vals[index]; }

        float operator*(float f) 
        {
            return (vals[0] * f) + (vals[1] * f) + (vals[2] * f);
        }

        Vec<n> operator+(Vec<n>&& v)
        {
            Vec<n> res;
            for (size_t i = 0; i < n; i++)
            {
                res[i] = (this->vals[i] + v[i]);
            }
        }

        float vals[n];
    };

    template<int m, int n>
    struct Mat
    {
        auto& operator[](size_t index) { return vals[index]; }
        float vals[m][n];
    };

    // get the dot product of 2 Vec<3>s
    inline float dot(Vec<3>& a, Vec<3>& b)
    {
        return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
    }

    // get the cross product of 2 Vec<3>s
    inline Vec<3> cross(Vec<3>& a, Vec<3>& b)
    {
        Vec<3> res;
        res[0] = (a[1] * b[2]) - (a[2] * b[1]);
        res[1] = (a[2] * b[0]) - (a[0] * b[2]);
        res[2] = (a[0] * b[1]) - (a[1] * b[0]);
        return res;
    }

    inline Vec<3> cross(Vec<3>& v, Mat<3, 3>& m)
    {
        Vec<3> res;
        res[0] = (v[0] * m.vals[0][0]) + (v[1] * m.vals[0][1]) + (v[2] * m.vals[0][2]);
        res[1] = (v[0] * m.vals[1][0]) + (v[1] * m.vals[1][1]) + (v[2] * m.vals[1][2]);
        res[2] = (v[0] * m.vals[2][0]) + (v[1] * m.vals[2][1]) + (v[2] * m.vals[2][2]);
        return res;
    }

    // get the cross product of 2 Mat<4, 4>s
    inline Mat<4, 4> cross(Mat<4, 4>& a, Mat<4, 4>& b)
    {
        Mat<4,4> res;
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                res[i][j] = a[i][j] * b[j][i];
            }
        }
        return res;
    }

    // add translation values to a 4x4 matrix
    inline Mat<4, 4> translate(Mat<4, 4>& mat, Vec<3>& v)
    {
        Mat<4, 4> res;
        res[3][0] = v[0];
        res[3][1] = v[0];
        res[3][2] = v[0];
        res[3][3] = 1.0f;
        return res;
    }

    inline Mat<4, 4> create_translation_matrix(Vec<3>& v)
    {
        Mat<4, 4> mat;
        mat[0][0] = 1.0f;
        mat[1][1] = 1.0f;
        mat[2][2] = 1.0f;
        mat[3][3] = 1.0f;

        return translate(mat, v);
    }

    // add rotation values to a 4x4 matrix
    inline Mat<4, 4> rotate(Mat<4, 4>& mat, float theta, Vec<3>& axis)
    {
        // radians?
        auto angle = radians(theta);
        auto c = cos(angle);
        auto s = sin(angle);
        auto t = axis * (1 - c);

        Vec<3> temp;
        temp[0] = t;
        temp[1] = t;
        temp[2] = t;

        Mat<4, 4> q;
        q[0][0] = c + temp[0] * axis[0];
		q[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
		q[0][2] = 0 + temp[0] * axis[2] - s * axis[0];

		q[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
		q[1][1] = c + temp[1] * axis[1];
		q[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

		q[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
		q[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
		q[2][2] = c + temp[2] * axis[2];

        Mat<4, 4> res;

        return cross(res, q);
    }
} // namespace xen

#endif // MATH_H
