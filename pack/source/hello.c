#include <stdio.h>

int main(int narg, char **varg) {
    printf("You've passed %d arguments\n", narg - 1);
    return 0;
}