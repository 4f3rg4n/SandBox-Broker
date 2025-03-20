#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program_to_run>\n", argv[0]);
        return 1;
    }

    /* hook libs */
    setenv("LD_PRELOAD", "files/files_hook.so:network/network_hook.so", 1); 

    /* exec process under sandbox broker */
    execvp(argv[1], &argv[1]);  

    perror("sandbox failed");  //exec fail
    return 1;
}
