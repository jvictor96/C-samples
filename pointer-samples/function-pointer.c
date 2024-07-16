// -> Capítulo 13 e 14 - pointers - 40 min

#include <stdio.h>

int rapper();

int return4();

void main(int narg, char **varg) {
    printf("function return: %d", rapper(return4));
}

int return4() {
    return 4;
}

int rapper(int fn()) {
    printf("invoking fn whatever it is");
    return fn();
}