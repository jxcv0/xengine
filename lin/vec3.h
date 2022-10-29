#ifndef VEC3_H_
#define VEC3_H_

/**
 * @brief A 3 dimentional vector.
 */
class Vec3 {
 public:
  /**
   * @brief Get the x component of the Vec3
   *
   * @return A floating point value.
   */
  constexpr float x() const noexcept { return m_data[0]; }

  /**
   * @brief Get the y component of the Vec3
   *
   * @return A floating point value.
   */
  constexpr float y() const noexcept { return m_data[1]; }

  /**
   * @brief Get the z component of the Vec3
   *
   * @return A floating point value.
   */
  constexpr float z() const noexcept { return m_data[2]; }

 private:
  float m_data[3];
};

#endif  // VEC3_H_
