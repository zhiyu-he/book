#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>


int const shift = sizeof(int) * CHAR_BIT - 1;

unsigned int abs1(int v) {
    return v < 0 ? -v: v;
}

unsigned int abs2(int v) {
    int const mask = v >> shift;
    return (v+mask)^mask;
}

unsigned int abs3(int v) {
    int const mask = v >> shift;
    return (v^mask)-mask;

}


int main() {
    
    clock_t start, finish;
    int sample[1000000] = {0};
    for (int i = 0; i < 1000000; i++) {
        sample[i] = rand();
    }
    
    start = clock();
    for (int i = 0; i < 1000000; i++) {
        abs1(sample[i]);
    }
    finish = clock();
    printf("abs1-cost: %ld\n", finish-start);


    start = clock();
    for (int i = 0; i < 1000000; i++) {
        abs2(sample[i]);
    }
    finish = clock();
    printf("abs2-cost: %ld\n", finish-start);

    return 0;
}
