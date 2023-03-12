#include <iostream>
#include <signal.h>
#include <cstring>
#include <unistd.h>

int count = 0;

void usr2_hand(int sig)
{
    count++;
}
void signal_handler(int signum, siginfo_t *info, void *context) {
    std::cout << "\nthe total number of SIGUSR2 signals received : " << count << "\n";
    exit(0);
}

int main()
{
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, usr2_hand);

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signal_handler;

    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &sa, nullptr) == -1) {
        std::cout << "Error setting up signal handler" << std::endl;
        exit(1);
    }
    while (true) {
        std::cout << "I'm still alive.\n";
        sleep(5);
    }
}