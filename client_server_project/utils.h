#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cerrno>
#include <cstdlib>
#include <limits.h>

typedef int (*operation)(int, int);

int addition(int number1, int number2)
{
    return number1 + number2;
}

int substraction(int number1, int number2)
{
    return number1 - number2;
}

int multiplication(int number1, int number2)
{
    return number1 * number2;
}

int division(int number1, int number2)
{
    if (number2 == 0)
        return UINT_MAX;
    return number1 / number2;
}

class Task
{
public:
    int id;
    int arg_1;
    int arg_2;
    int result;

    Task(int arg_1, int arg_2, int id)
    {
        this->id = id;
        this->arg_1 = arg_1;
        this->arg_2 = arg_2;
    }

    void execute_task()
    {
        operation op;
        switch (this->id)
        {
		case 0:
                    op = addition;
                    this->result = op(this->arg_1, this->arg_2);
                    break;
                case 1:
                    op = substraction;
                    this->result = op(this->arg_1, this->arg_2);
                    break;
                case 2:
                    op = multiplication;
                    this->result = op(this->arg_1, this->arg_2);
                    break;
                case 3:
                    op = division;
                    this->result = op(this->arg_1, this->arg_2);
                    break;
           }

        }
};
