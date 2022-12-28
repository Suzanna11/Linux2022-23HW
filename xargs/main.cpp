#include <unistd.h>
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char **argv)
{
    std::vector<std::string> arguments(100);
    int i = 0;
    std::cin >> arguments[i];
    while(arguments[i] != "CTRL+D")
    {
        ++i;
        std::cin >> arguments[i];
    }

    char** args = new char*[arguments.size()];
    for (int j = 0; j < arguments.size(); j++)
    {
        args[j] = const_cast<char*>(arguments[j].c_str());
    }

    pid_t child = fork();
    if(child < 0)
    {
        perror("Fork failed.");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    {
        if(execvp(argv[0], args) < 0)
        {
            perror("exec failed.\n");
            exit(EXIT_FAILURE);
        }
    }


}
