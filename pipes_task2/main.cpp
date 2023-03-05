#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <sys/wait.h>

bool isPrime(int n);
int nthPrime(int n);

int main() {
    int pipe_fd1[2];
    int pipe_fd2[2];

    if(pipe(pipe_fd1) == -1)
    {
        std::cout << "Cannot create pipe.\n";
        exit(errno);
    }

    if(pipe(pipe_fd2) == -1)
    {
        std::cout << "Cannot create pipe.\n";
        exit(errno);
    }

    pid_t child = fork();
    if(child == -1)
    {
        std::cout << "Fork failed.\n";
        exit(errno);
    }

    if(child == 0)
    {
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);
        while(true)
        {
            int m;
            if(read(pipe_fd1[0], &m, sizeof(int)) == -1)
            {
                std::cout << "Cannot read.\n";
                exit(errno);
            }
            if(m == -1) break;
            int res = nthPrime(m);
            if(write(pipe_fd2[1], &res, sizeof(int)) == -1)
            {
                std::cout << "Cannot write.\n";
                exit(errno);
            }
        }

    }
    else
    {
        close(pipe_fd1[0]);
        close(pipe_fd2[1]);

        std::string n;
        while(true)
        {
            std::getline (std::cin, n);
            if(n == "exit")
            {
                int x = -1;
                if(write(pipe_fd1[1], &x, sizeof(int)) == -1)
                {
                    std::cout << "Cannot write.\n";
                    exit(errno);
                }
                break;
            }

            int n1 = std::stoi(n);
            if(write(pipe_fd1[1], &n1, sizeof(n1)) == -1)
            {
                std::cout << "Cannot write.\n";
                exit(errno);
            }

            int resultFromChild;
            if(read(pipe_fd2[0], &resultFromChild, sizeof(int)) == -1)
            {
                std::cout << "Cannot read.\n";
                exit(errno);
            }
            std::cout << "expected prime number : " << resultFromChild << "\n";
        }
         wait(nullptr);
    }
}
bool isPrime(int n) {
    if (n < 2)
        return false;

    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int nthPrime(int n)
{
    int p = 2;
    while(n > 0)
    {
        if(isPrime(p))
            n--;
        p++;
    }
    p -= 1;
    return p;
}
