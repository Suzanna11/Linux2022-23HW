#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
  const char* name = "/sh_mem";
  if (shm_unlink(name) == -1) {
    std::cerr << "Error unlinking shared memory: " << std::strerror(errno) << std::endl;
    return 1;
  }
  std:: cout << "Shared memory is marked as a segment to be deleted.\n";
  return 0;
}

