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
  int fd = shm_open(name, O_RDONLY, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    std::cerr << "Failed to create shared memory. " << std::strerror(errno) << std::endl;
    return 1;
  }

  void* addr = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED)
  {
    std::cerr << "Failed to map shared memory.  " << std::strerror(errno) << std::endl;
    return 1;
  }

  std::string input;
  while (std::getline(std::cin, input))
  {
        if (input.length() > size)
        {
            std::cerr << "Input string size is longer that shared memory size\n";
            exit(1);
        }
	std::memcpy(addr, input.c_str(), input.length() + 1);

        std::cout << "Input written to shared memory. " << input << std::endl;
  }

    if (munmap(addr, size) == -1)
    {
        std::cerr << "Failed to unmap shared memory segment. " << strerror(errno) << std::endl;
    }
    if (close(fd) == -1) {
        std::cerr << "Failed to close shared memory fd. " << strerror(errno) << std::endl;
    }

    return 0;
}




