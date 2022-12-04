#ifndef MMAPFILE_H_
#define MMAPFILE_H_

#include <sys/mman.h>
#include <filesystem>

class MmapFile {
 public:
   MmapFile(const std::filesystem::path& filepath) : m_filepath(filepath) {
     // mp_device = mmap(
   }
 private:
   std::filesystem::path m_filepath;
  void *mp_device;
};

#endif // MMAPFILE_H_
