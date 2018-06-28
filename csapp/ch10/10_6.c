#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // file control options
#include <unistd.h>

int main()
{
    int fd1, fd2;
    fd1 = open("foo.txt", O_RDONLY, 0);
    fd2 = open("bar.txt", O_RDONLY, 0);
    printf("fd2 = %d\n", fd2);
    int code = 0;
    code = close(fd2);
    printf("close(fd2): %d\n", code);

    fd2 = open("baz.txt", O_RDONLY, 0);
    printf("fd2 = %d\n", fd2);      // 与第一个fd2是同一个值, 因为已经被os回收了, 当baz.txt不存在时, 返回 -1
    return 0;
}
