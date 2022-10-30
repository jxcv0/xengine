#ifndef MAT4_H_
#define MAT4_H_

#include <cmath>
#include <initializer_list>
#include <ostream>

/**
 * @brief A 4 x 4 dimensional matrix.
 */
class Mat4 {
 public:
  Mat4() = default;
  constexpr inline Mat4(float f) {
    m_data{f};
  }
 private:
  float m_data[4][4] = {0};
};

#endif  // MAT4_H_
