#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
    int fd1, fd2;
    fd1 = open("foo.txt", O_RDONLY, 0);
    printf("fd1= %d\n", fd1);
    close(fd1);
    fd2 = open("baz.txt", O_RDONLY, 0); // 因为fd1被关闭了, 因此被fd1的编号被fd2 复用了
    printf("fd2= %d\n", fd2);
    close(fd1);
    exit(0);
}
