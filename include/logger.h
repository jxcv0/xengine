#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <string>

/**
 * @brief Log a message if using debug build.
 */
static inline void log_debug_impl(const char *file, const char *func, int line,
                                  std::string str) {
#ifndef NDEBUG
  std::cout << file << ": " << func << ": " << line << "\n\t--> " << str
            << "\n";
#endif
}

#define log_debug(msg) log_debug_impl(__FILE__, __func__, __LINE__, msg)

#endif  // LOGGER_H_
