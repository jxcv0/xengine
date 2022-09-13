#ifndef SHADEREXCEPTIONS_H_
#define SHADEREXCEPTIONS_H_

#include <string>
#include <stdexcept>

/* \brief Runtime exception to be thrown when shader fails to compile.
 */
class ShaderCompileError: public std::runtime_error {
  std::string m_message;
public:
  virtual const char* what() const noexcept {
    return m_message.c_str();
  }
};

/* \brief Runtime exception to be thrown when shader fails to link.
 */
class ShaderLinkError : public std::runtime_error {
  std::string m_message;
public:
  virtual const char* what() const noexcept {
    return m_message.c_str();
  }
};

#endif // SHADEREXCEPTIONS_H_
