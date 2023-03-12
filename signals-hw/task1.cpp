#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void killParentProcess()
{
	pid_t child = fork();
	if(child == 0)
	{
		std::cout << "Killing the parent\n";

		if(kill(getppid(), SIGKILL) == -1)
        {
            std::cout << "kill failed.\n";
            exit(errno);
        };
        std::cout << "I'm orphan\n";
	}
	else
	{
		wait(nullptr);
	}

}
int main()
{
    killParentProcess();
}