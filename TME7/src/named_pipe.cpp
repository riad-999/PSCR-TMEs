#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <iostream>


int main(int argc, char* argv[]) {
    char* list1[argc]; 
    char* list2[argc];
    int index;

    memset(list1, 0, argc * sizeof(char*));
    memset(list2, 0, argc * sizeof(char*));

    for(index = 1; index < argc; index++) {
        if (strcmp(argv[index], "|") == 0) {
            index++;
            break;
        } else {
            list1[index - 1] = argv[index];
        }
    }

    for(int i = 0; index < argc; index++, i++) {
        list2[i] = argv[index];
    }

    int pdesc[2];
    pipe(pdesc);

    if (fork() == 0) {
        dup2(pdesc[1], STDOUT_FILENO);
        close(pdesc[0]);
        close(pdesc[1]);
        int i = 0;
        while(list1[i] != nullptr) {
            std::cerr << list1[i] << " ";
            i++;
        }
        std::cerr << std::endl;
        if(execv(list1[0], list1) == -1) {
            std::cerr << "something went wrong" << std::endl;
            exit(1);
        }
    }
    if(fork () == 0) {
        dup2(pdesc[0], STDIN_FILENO);
        close(pdesc[0]);
        close(pdesc[1]);
        int i = 0;
        while(list2[i] != nullptr) {
            std::cerr << list2[i] << " ";
            i++;
        }
        std::cerr << std::endl;
        execv(list2[0], list2);
        if(execv(list2[0], list2) == -1) {
            std::cerr << "something went wrong" << std::endl;
            exit(1);
        }
    }

    close(pdesc[0]);
    close(pdesc[1]);

    wait(nullptr);
    wait(nullptr);

    return 0;
}   