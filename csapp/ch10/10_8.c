#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct stat st;
    char *type, *readok;
    
    int fd;
    fd = atoi(argv[1]);
    printf("argv[1]: %s  to_int: %d\n", argv[1], fd);
    fstat(fd, &st);

    if (S_ISREG(st.st_mode))
        type = "regular";
    else if (S_ISDIR(st.st_mode))
        type = "directory";
    else
        type = "other";

    if ((st.st_mode & S_IRUSR))
        readok = "yes";
    else
        readok = "no";

    printf("type: %s, read: %s\n", type, readok);
    exit(0);
}
