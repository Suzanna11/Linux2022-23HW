#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>     
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <climits>
#include "utils.h"

typedef int (*operation)(int, int);

int main()
{
	const char* sem_name1 = "/sem1";
	
	sem_t* sem1 = sem_open(sem_name1, O_CREAT, 0666, 0);
	
	if (sem1 == SEM_FAILED)
	{
		std::cerr << "Failed to create semaphore." << std::endl;
		exit(errno);
	}
	
	const char* sem_name2 = "/sem2";
	
	sem_t* sem2 = sem_open(sem_name2, O_CREAT, 0666, 0);
	
	if (sem2 == SEM_FAILED)
	{
		std::cerr << "Failed to create semaphore." << std::endl;
		exit(errno);
	}
	
	const char* filename = "/shm";
	int shm_fd = shm_open(filename, O_RDWR | O_CREAT, 0666);
	if (shm_fd == -1)
	{
		std::cerr << "Failed to create shared memory." << std::endl;
		exit(errno);
	}

	std::size_t shm_size = sizeof(Function);
	if (ftruncate(shm_fd, shm_size) == -1)
	{
		std::cerr << "Failed to resize shared memory." << std::endl;
		exit(errno);
	}

	Function* func = (Function*)mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if (func == MAP_FAILED)
	{
		std::cerr << "Failed to map shared memory." << std::endl;
		exit(errno);
	}
	close(shm_fd);

	operation op;
	
	while(true)
	{
		sem_wait(sem1);
		if (func->id > 3 || func->id < 0)
		{
			std::cerr << "Wrong input. Function number must be from 0 to 3" << std::endl;
		   	func->id = INT_MAX;
		}
		else
		{	
		    	switch (func->id)
		   	{
			case 0:
			    	op = addition;
			    	func->result = op(func->arg_1, func->arg_2);
			    	break;

			case 1:
			    	op = substraction;
			    	func->result = op(func->arg_1, func->arg_2);
			    	break;

			case 2:
			    	op = multiplication;
			    	func->result = op(func->arg_1, func->arg_2);
			    	break;

			case 3:
			    	op = division;
			    	func->result = op(func->arg_1, func->arg_2);
			    	break;
		    	}
		    	
		    	if(func->result == INT_MAX)
				std::cerr << "Cannot divide by zero." << std::endl;
		}
		
		sem_post(sem2);
	}
	
	sem_close(sem1);
	sem_unlink(sem_name1);
	sem_close(sem2);
	sem_unlink(sem_name2);
	munmap(func, shm_size);
	shm_unlink(filename);
	return 0;
}
