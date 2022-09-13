#ifndef SHADEREXCEPTIONS_H_
#define SHADEREXCEPTIONS_H_

#include <exception>

/* \brief Runtime exception to be thrown when shader fails to compile.
 */
class ShaderCompileException : public std::runtime_error {
    std::string m_message;
public:
    const char* what() override {
        return m_message.c_str();
    }
};

/* \brief Runtime exception to be thrown when shader fails to link.
 */
class ShaderLinkException : public std::runtime_error {
    std::string m_message;
public:
    const char* what() override {
        return m_message.c_str();
    }
};

#endif // SHADEREXCEPTIONS_H_
