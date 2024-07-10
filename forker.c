#include "forker.h"

pid_t forker(const char* path, char* const args[]) { 
  
    //fork new process
    pid_t p = fork(); 
    
    if(p == -1){
        perror("fork error"); //error
    }
    else if(p == 0){
        printf("child: %d\n", getpid());
        execv(path, args); //run the process
        exit(0);
    }

    return p; 
}