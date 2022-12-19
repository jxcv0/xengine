#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <cstring>

template <typename T>
class Component {
 public:
  Component(const T* t) { std::memcpy(m_data, t, sizeof(T)); }

#ifndef COMPONENT_GTEST
 private:
#endif

  T* (*get)(void*);
  char m_data[sizeof(T)];
};

#endif  // COMPONENT_H_
