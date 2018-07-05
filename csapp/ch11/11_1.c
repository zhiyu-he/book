#include <stdio.h>
#include <stdlib.h>

char* int2hex(int num)
{
    char hex[9];
    hex[8]='\0';
    int i = 7;
    while (num)
    {   
        if (num & 0x01)
            hex[i]='1';
        else
            hex[i]='0';
        num >>=1;
        i--;
    }
    printf("%s\n", hex);
    return "";
}

int main()
{
    int2hex(205);
    printf("%s\t%s\n", "0x0", "0.0.0.0");
    printf("%s\t%s\n", "0xffffffff", "255.255.255.255");
    printf("%s\t%s\n", "", "205.188.160.121");
    exit(0);
}
