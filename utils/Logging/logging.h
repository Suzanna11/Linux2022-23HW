#include <iostream>
#include <string.h>
#include <unistd.h>

void log_fatal(int errnum)
{
    std::cerr << strerror(errnum) << "\n";
    exit(EXIT_FAILURE);
}
