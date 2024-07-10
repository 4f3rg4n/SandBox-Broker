#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>

pid_t forker(const char* path, char*  const args[]);
