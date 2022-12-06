#ifndef MMAPFILE_H_
#define MMAPFILE_H_

#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string_view>

/**
 * @brief Memory mapped RAII class. Opens a file and maps it to a memory
 * address.
 */
class MmapFile {
 public:
  /**
   * @brief Construct an MmapFile using a filepath. Once the mapping is
   *        successful, the file is closed. If initialization fails the
   *        instances address is set to nullptr. Validity can be queried with
   *        MmapFile::valid.
   *
   * @param filepath The filepath to the file to open.
   */
  MmapFile(const std::filesystem::path& filepath) noexcept;

  /**
   * @brief Destructor. Unmap the memory address.
   */
  ~MmapFile();

  /**
   * @brief Print the contents of the address.
   */
  friend std::ostream& operator<<(std::ostream& os, const MmapFile& mmf) {
    auto c = static_cast<char*>(mmf.mp_addr);
    for (size_t i = 0; i < mmf.m_len; i++) {
      os << c[i];
    }
    return os;
  }

  /**
   * @brief Check that the memory address is valid.
   *
   * @return boolean True if address is not nullptr.
   */
  inline auto valid() const { return mp_addr != nullptr; }

  /**
   * @brief Get mapped memory address.
   *
   * @return A pointer to the memory address.
   */
  const char* data() const { return static_cast<const char*>(mp_addr); }

  /**
   * @brief Get the size of the memory mapped to the file.
   *
   * @return The size of the memory.
   */
  auto len() const { return m_len; }

  auto get_line() {
  }

 private:
  void *mp_addr;
  std::size_t m_len;
};

#endif  // MMAPFILE_H_
