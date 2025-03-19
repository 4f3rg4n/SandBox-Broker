#include "hook.h"

static int (*real_open)(const char*, int, ...) = NULL;

int open_hook(const char *pathname, int flags, mode_t mode) {
    int op;
    if (!real_open)
        real_open = dlsym(RTLD_NEXT, "open");
    char name[] = "file\0";
    printf("is ok?\n> ");
    scanf("%d", &op);
    printf("[HOOK] open(\"%s\")\n", pathname);
    return (op == 0) ? real_open(pathname, flags, mode) :real_open(name, flags, mode);
}

int open(const char *pathname, int flags, ...) __attribute__((weak, alias("open_hook")));
