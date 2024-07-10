#include "forker.c"
#include <string.h>

char** parse_args(int argc, char *argv[]);

int main(int argc, char *argv[]) { 
    char** args = parse_args(argc, argv);
    
    forker(args[0], args);

    free(args);

    return 0; 
}

char** parse_args(int argc, char *argv[]) {
    char** args = (char**)malloc(argc * sizeof(char*));

    //usage message
    if(argc < 2){
        printf("Usage: ./virtualizer {process name} {args .,.,.}\n");
    }

    //get the process args
    for(int i = 0; i < (argc - 1); i++){
        args[i] = argv[i + 1];
    }

    args[argc - 1] = NULL;

    return args;
}