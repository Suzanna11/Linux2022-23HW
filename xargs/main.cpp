#include <unistd.h>
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char **argv)
{
    std::vector<std::string> arguments;
    std::string argument;
    while (std::cin >> argument)
    {
        arguments.emplace_back(std::move(argument));
    }

    std::vector<char*> argsVec(arguments.size(), nullptr);

    for (int j = 0; j < arguments.size(); j++)
    {
        argsVec[j] = strdup(arguments[j].c_str());
    }
    argsVec.emplace_back(nullptr);

    pid_t child = fork();
    if(child < 0)
    {
        perror("Fork failed.");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    {
        if(execvp(argv[0], argsVec.data()) < 0)
        {
            perror("exec failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}
