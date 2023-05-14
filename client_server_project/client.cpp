#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>     
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <climits>
#include "utils.h"



int main()
{
    const char* filename = "/shm";
    int shm_fd = shm_open(filename, O_RDWR, 0);
    if (shm_fd == -1)
    {
        std::cerr << "Failed to open shared memory." << std::endl;
        exit(errno);
    }

    std::size_t shm_size = sizeof(Function);
    

    Function* func = (Function*)mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (func == MAP_FAILED)
    {
        std::cerr << "Failed to map shared memory." << std::endl;
        exit(errno);
    }
    close(shm_fd);

    const char* sem_name1 = "/sem1";
    
    sem_t* sem1 = sem_open(sem_name1, 0);
    
    if (sem1 == SEM_FAILED)
    {
        std::cerr << "Failed to create semaphore" << std::endl;
        exit(errno);
    }
    
    const char* sem_name2 = "/sem2";
    
    sem_t* sem2 = sem_open(sem_name2, 0);
    
    if (sem2 == SEM_FAILED)
    {
        std::cerr << "Failed to create semaphore" << std::endl;
        exit(errno);
    }

    std::size_t functionNumber = 0;
    std::cout << "Enter the function number (0: add, 1: sub, 2: mul, 3: div): ";
    std::cin >> functionNumber;
    func->id = functionNumber;
    sem_post(sem1);
    sem_wait(sem2);
    
    if (func->id == INT_MAX)
        exit(EXIT_FAILURE);

    std::cout << "Enter 2 arguments: ";
    int number1;
    int number2;
    std::cin >> number1;
    std::cin >> number2;
    
    func->arg_1 = number1;
    func->arg_2 = number2;
    
    sem_post(sem1);
    sem_wait(sem2);

    if (func->result == INT_MAX)
        exit(EXIT_FAILURE);

    std::cout << "The result is: " << func->result << std::endl;

    sem_close(sem1);
    sem_close(sem2);
    munmap(func, shm_size);
    return 0;
}
