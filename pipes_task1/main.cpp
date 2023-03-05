#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <sys/wait.h>

int main() {
	int pipe_fd1[2];
    int pipe_fd2[2];

	if (pipe(pipe_fd1) != 0){
		std::cout << "Cannot create pipe.\n";
		exit(errno);
	}

    if (pipe(pipe_fd2) != 0){
        std::cout << "Cannot create pipe.\n";
        exit(errno);
    }

    pid_t cpid1 = fork();
    if(cpid1 == -1){
        std::cout << "Fork failed.\n";
        exit(errno);
    }

    if(cpid1 == 0)
    {
        close(pipe_fd1[0]);
        close(pipe_fd2[1]);

        pid_t pid1 = getpid();
        std::cout << "First child's pid: " << pid1 << "\n";
        if(write(pipe_fd1[1], &pid1, sizeof(pid_t)) == -1)
        {
            std::cout << "Cannot write.\n";
            exit(errno);
        }

        pid_t pid2;
        if(read(pipe_fd2[0], &pid2, sizeof(pid_t)) == -1)
        {
            std::cout << "Cannot read.\n";
            exit(errno);
        }
        std::cout << "Second child's pid in the first child's body: " << pid2 << "\n";
        return 0;
    }

    pid_t cpid2 = fork();

    if(cpid2 == -1){
        std::cout << "Fork failed.\n";
        exit(errno);
    }

    if(cpid2 == 0)
    {
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);

        pid_t pid2 = getpid();
        std::cout << "Second child's pid: " << pid2 << "\n";
        if(write(pipe_fd2[1], &pid2, sizeof(pid_t)) == -1)
        {
            std::cout << "Cannot write.\n";
            exit(errno);
        }

        pid_t pid1;
        if(read(pipe_fd1[0], &pid1, sizeof(pid_t)) == -1)
        {
            std::cout << "Cannot read.\n";
            exit(errno);
        }
        std::cout << "First child's pid in the second child's body: " << pid1 << "\n";
        return 0;
    }

    wait(nullptr);
    wait(nullptr);
    return 0;
}

