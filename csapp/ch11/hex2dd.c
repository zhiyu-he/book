#include <stdio.h>
#include <stdlib.h>

int hex2int(char ch)
{
    // 一个十六进制的字符, 有3种case, 数字[0~9], [a~z], [A~Z]
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else if (ch >= 'a' && ch <= 'z') {
        return ch - 'a';
    } else if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A';
    } else {
        return -1;
    }
}

void hex2dd(char* hex, int (*p)[])
{   
    int loop = 0;
    for (int i = 2; i <= 8; i+=2)
    {
        int i1 = hex2int(hex[i]);
        int i2 = hex2int(hex[i+1]);
        (*p)[loop++] = i1 << 4 | i2;
    }
}

// 我们做一些约束
// 1. 输入的格式为0x8002c2f2
// 2. 必须以0x为prefix
// 3. prefix后, 一定为8位
int main(int argc, char **argv)
{   
    int dd[4];
    hex2dd(argv[1], &dd);
    printf("%d.%d.%d.%d\n", dd[0], dd[1], dd[2], dd[3]);
    exit(0);
}
