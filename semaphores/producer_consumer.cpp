#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <vector>

int main()
{
    const int buf_size = 10;
    const int num = 100;
    const char* sh_m = "my_shared_memory";
    const int map_size = buf_size * sizeof(int);

    int shm_fd = shm_open(sh_m, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("Failed to create shared memory region");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, map_size) == -1)
    {
        perror("Failed to resize shared memory region");
        exit(EXIT_FAILURE);
    }

    int *addr = (int*)(mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    if (addr == MAP_FAILED)
    {
        perror("Failed to map shared memory region");
        exit(EXIT_FAILURE);
    }

    const char* empty = "empty";
    const char* full = "full";
    const char* mutex = "mutex";

    sem_t* empty_sem_ptr = sem_open(empty, O_CREAT, 0666, buf_size);
    if (empty_sem_ptr == SEM_FAILED)
    {
        perror("Failed to create empty semaphore.");
        exit(EXIT_FAILURE);
    }

    sem_t* full_sem_ptr = sem_open(full, O_CREAT, 0666, 0);
    if (full_sem_ptr == SEM_FAILED)
    {
        perror("Failed to create full semaphore");
        exit(EXIT_FAILURE);
    }
    sem_t* mutex_sem_ptr = sem_open(mutex, O_CREAT, 0666, 1);
    if (mutex_sem_ptr == SEM_FAILED)
    {
        perror("Failed to create mutex semaphore.");
        exit(EXIT_FAILURE);
    }

    pid_t cpid = fork();
    if (cpid == -1)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }
    else if (cpid == 0)
    {
        // consumer
        int item;
        for (int i = 0; i < num; ++i) {
            sem_wait(full_sem_ptr);

            sem_wait(mutex_sem_ptr);

            item = addr[2 + addr[0]];
            std::cout << item << std::endl;
            addr[0] = (addr[0] + 1) % buf_size;

            sem_post(mutex_sem_ptr);

            sem_post(empty_sem_ptr);
        }
    }
    else
    {
        // producer
        std::vector<int> items(num);

        for (int i = 0; i < num; ++i)
            items[i] = i + 1;

        int ind = 0;
        for (int i = 0; i < num; ++i)
        {
            sem_wait(empty_sem_ptr);

            sem_wait(mutex_sem_ptr);

            addr[2 + addr[1]] = items[ind];
            addr[1] = (addr[1] + 1) % buf_size;
            ++ind;

            sem_post(mutex_sem_ptr);

            sem_post(full_sem_ptr);
        }
    }

    munmap(addr, map_size);
    shm_unlink(sh_m);

    sem_close(empty_sem_ptr);
    sem_unlink(empty);
    sem_close(full_sem_ptr);
    sem_unlink(full);
    sem_close(mutex_sem_ptr);
    sem_unlink(mutex);

    return 0;
}
