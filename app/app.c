#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program_to_run>\n", argv[0]);
        return 1;
    }

    setenv("LD_PRELOAD", "./hook.so", 1);
    execvp(argv[1], &argv[1]);  // Run the given program with arguments

    perror("execvp failed");  // If execvp fails, print error
    return 1;
}
