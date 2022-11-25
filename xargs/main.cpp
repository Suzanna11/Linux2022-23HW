#include <cstdio>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    const char * path = "/usr//bin/ls";
    char *args[] = {argv[0], argv[1], argv[2], nullptr};
    execv(path, args);

}