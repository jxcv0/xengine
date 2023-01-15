#include <collada-dom2.5/dae.h>
#include <collada-dom2.5/dae/daeAtomicType.h>
#include <iostream>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage:\n" << "collada <collada_file> <output_files_name>\n";
  }

  DAE file;
  auto *root = file.open(argv[1]);
  if (root == nullptr) {
    std::cout << "unable to import " << argv[1] << "\n";
    return 0;
  }

  return 0;
}
