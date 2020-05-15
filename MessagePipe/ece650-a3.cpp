#include <iostream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){


    std::vector<pid_t> kids;
    int rgenToA1[2];
    pipe(rgenToA1);
    pid_t a3_rgen;
    a3_rgen = fork();

    //execute rgen.cpp
    if(a3_rgen == 0){
        dup2(rgenToA1[1], STDOUT_FILENO);
        close(rgenToA1[0]);
        close(rgenToA1[1]);

        if(execv("./rgen", argv) == -1){
            std::cerr << "Error: an error occurred in execvp rgen.cpp" << std::endl;
        }
    }
    kids.push_back(a3_rgen);

    int A1toA2[2];
    pipe(A1toA2);
    pid_t rgen_a1;
    rgen_a1 = fork();

    // execute a1
    if(rgen_a1 == 0){
        dup2(rgenToA1[0], STDIN_FILENO);
        close(rgenToA1[1]);
        close(rgenToA1[0]);

        dup2(A1toA2[1], STDOUT_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);

        if(execlp("python2.7", "python", "ece650-a1.py", NULL) == -1){
            std::cerr << "Error: an error occurred in execvp ece650-a1.cpp" << std::endl;
        }
    }
    kids.push_back(rgen_a1);

    pid_t a1_a2;
    a1_a2 = fork();
    if(a1_a2 == 0){

        dup2(A1toA2[0], STDIN_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);
        if(execlp("./ece650-a2","./ece650-a2", NULL) == -1){
            std::cerr << "Error: an error occurred in execvp ece650-a2.cpp" << std::endl;
        }
    }
    kids.push_back(a1_a2);

    pid_t a2_out;
    a2_out = fork();

    if(a2_out == 0){
        dup2(A1toA2[1], STDOUT_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);

        while (!std::cin.eof()) {
            std::string line;
            std::getline(std::cin, line);
            std::cout << line << std::endl;
        }
        for (pid_t k : kids) {
            int status;
            kill (k, SIGTERM);
            waitpid(k, &status, 0);
        }
    }
    kids.push_back(a2_out);

    for (pid_t k : kids){
        int status;
        waitpid(k, &status, 0);
    }

    return 0;
}







