// -> Capítulo 13 e 14 - pointers - 40 min

#include <stdio.h>

void main(int narg, char **varg) {
    for (int i = 0; i < narg; i++) {
        printf("Argument %d: %s\n", i, varg[i]);
    }
}