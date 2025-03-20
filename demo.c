#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("a", O_RDONLY);
    char buf[1024];
    int bytes = read(fd, buf, sizeof(buf) - 1);
    buf[bytes] = 0;
    printf("%s", buf);
}
