#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <climits>
#include <queue>
#include "utils.h"
#include <vector>
#include <signal.h>

class ThreadPool
{
private:
    std::queue<Task*> taskQueue;
    std::vector<pthread_t*> threads;
    int threadsCount;
    pthread_mutex_t lock;
    pthread_cond_t hasFunction;

public:
    ThreadPool(int num)
    {
        if(pthread_mutex_init(&lock, nullptr) < 0)
        {
            perror("mutex init failed.");
        }
        if(pthread_cond_init(&hasFunction, nullptr) < 0)
        {
            perror("cond failed.");
        }
        this->threadsCount = num;

        for(int i = 0; i < this->threadsCount; ++i) {
            pthread_t *thread = new pthread_t;
            threads.push_back(thread);
            pthread_create(thread, nullptr, execute, this);
        }
    }

    ~ThreadPool()
    {
        pthread_cond_destroy(&hasFunction);
        pthread_mutex_destroy(&lock);
        for(int i = 0; i < this->threadsCount; ++i)
        {
            pthread_kill(*threads[i], SIGKILL);
            delete threads[i];
        }
    }

    void addTasks(Task* task)
    {
        pthread_mutex_lock(&lock);
        this->taskQueue.push(task);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&hasFunction);
    }

    // void stop()
    // {

    // }

    static void* execute(void* arg)
    {
        ThreadPool* threadpool = (ThreadPool*) arg;
        while(true)
        {
            pthread_mutex_lock(&threadpool->lock);
            while(threadpool->taskQueue.empty())
            {
                pthread_cond_wait(&threadpool->hasFunction, &threadpool->lock);
            }
            Task* task = threadpool->taskQueue.front();
            threadpool->taskQueue.pop();
            pthread_mutex_unlock(&threadpool->lock);
            task->execute_task();
        }
    }
};

int main()
{
    const int thNum = 5;
    ThreadPool thp(thNum);
    const char* sem_name1 = "/sem1";
    const char* sem_name2 = "/sem2";
    sem_t* sem1 = sem_open(sem_name1, O_CREAT, 0666, 0);

    if (sem1 == SEM_FAILED)
    {
        std::cerr << "Failed to create semaphore" << std::endl;
        exit(errno);
    }

    sem_t* sem2 = sem_open(sem_name2, O_CREAT, 0666, 0);
    if (sem2 == SEM_FAILED)
    {
        std::cerr << "Failed to create semaphore" << std::endl;
        exit(errno);
    }

    const char* filename = "/shm";
    int shm_fd = shm_open(filename, O_RDWR | O_CREAT, 0666);
    if (shm_fd == -1)
    {
        std::cerr << "Failed to create shared memory." << std::endl;
        exit(errno);
    }

    std::size_t shm_size = sizeof(Task);
    if (ftruncate(shm_fd, shm_size) == -1)
    {
        std::cerr << "Failed to resize shared memory." << std::endl;
        exit(errno);
    }

    Task* currTask = (Task*)mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (currTask == MAP_FAILED)
    {
        std::cerr << "Failed to map shared memory." << std::endl;
        exit(errno);
    }
    close(shm_fd);

    while(true)
    {
        sem_wait(sem1);
        thp.addTasks(currTask);
        sem_post(sem2);
    }

    munmap(currTask, shm_size);
    shm_unlink(filename);
    return 0;
}
