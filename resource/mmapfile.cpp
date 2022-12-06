#include "mmapfile.h"

MmapFile::MmapFile(const std::filesystem::path& filepath) noexcept {
  mp_addr = nullptr;
  auto fd = open(filepath.c_str(), O_RDWR);
  // if (fd == -1) { return; }
  m_len = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  mp_addr = mmap(nullptr, m_len, (PROT_READ | PROT_WRITE), MAP_PRIVATE, fd, 0);
  close(fd);
}

MmapFile::~MmapFile() { munmap(mp_addr, m_len); }
