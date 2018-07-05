#include <stdio.h>
#include <stdlib.h>


int code_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void dd2hex(char *dd, char (*hex)[])
{
    (*hex)[0] = '0';
    (*hex)[1] = 'x';
    (*hex)[10] = '\0';
    int hex_idx = 2;
    char *p = dd;
    int cnt = 0;
    int cur_idx = 0;
    while (*p != '\0')
    {
        if (*p == '.')
        {   
            char ch[3] = {'0', '0', '0'};
            for (int i = 0; i < cnt; i++)
                ch[2-i] = dd[cur_idx - i - 1];
            int num = 0;
            num += (ch[0] - '0') * 100;
            num += (ch[1] - '0') * 10;
            num += (ch[2] - '0');
            (*hex)[hex_idx+1] = code_table[(num >> 4) & 0xF];
            (*hex)[hex_idx] = code_table[(num & 0xF)];
            // clean
            hex_idx += 2;
            cnt = 0;
        } else {
            cnt += 1;
        }
        cur_idx += 1;
        p++;
    }

    
    char ch[3] = {'0', '0', '0'};
    for (int i = 0; i < cnt; i++)
        ch[2-i] = dd[cur_idx - i - 1];
    int num = 0;
    num += (ch[0] - '0') * 100;
    num += (ch[1] - '0') * 10;
    num += (ch[2] - '0');
    (*hex)[hex_idx+1] = code_table[(num >> 4) & 0xF];
    (*hex)[hex_idx] = code_table[(num & 0xF)];
}

int main(int argc, char **argv)
{
    char hex[11];
    dd2hex(argv[1], &hex);
    printf("%s\n", hex);
    exit(0);
}
