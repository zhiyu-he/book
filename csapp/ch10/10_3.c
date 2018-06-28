#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
    int fd;
    char c;
    fd = open("foobar.txt", O_RDONLY, 0);
    if (fork() == 0) {
       read(fd, &c, 1);
       exit(0);
    }
    wait(NULL);
    read(fd, &c, 1);
    printf("c = %c\n", c);  // 进程会复制descriptor table, 因此输出第二个字符 ch = 'o'

    exit(0);
}
