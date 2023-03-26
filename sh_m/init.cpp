#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() 
{
  const char* name = "/sh_mem";
  const int size = 4096;
  int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if (fd == -1) 
  {
    std::cerr << "Cannot create shared memory. " << std::strerror(errno) << std::endl;
    return 1;
  }

  if (ftruncate(fd, size) == -1)
  {
    std::cerr << "Cannot resize shared memory" << std::strerror(errno) << std::endl;
    return 1;
  }
  
  void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) 
  {
    std::cerr << "Cannot map shared memory: " << std::strerror(errno) << std::endl;
    return 1;
  }
  close(fd);
  std::cout << "Shared memory created.\n";
  return 0;
}

