#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
    int fd1, fd2;
    char c;
    fd1 = open("foobar.txt", O_RDONLY, 0);
    fd2 = open("foobar.txt", O_RDONLY, 0);
    read(fd1, &c, 1);
    read(fd2, &c, 1);

    printf("c = %c\n", c);  // 由于分别持有一个fd, 所以各自的偏移量, 单独计算, 所以c='f'

    exit(0);
}
