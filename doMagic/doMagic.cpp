#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils/logging.h"

void do_magic()
{
    const int  new_fd = open("new_pts.txt", O_RDONLY);
    if(new_fd < 0)
    {
        log_fatal(errno);
    }
    const int d2 = dup2(new_fd, 0);
    if(d2 < 0)
    {
        log_fatal(errno);
    }
}
