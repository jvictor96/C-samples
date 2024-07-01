#include <stdio.h>

char* get_last_position1(char*);

int main(int argc, char *argv[]) {
    printf("Hello, World! There are %d command-line arguments.\n", argc);

    char * last_address;
    int i;
    for (i = 0; i < argc; i++) {
        if(i>0) {
            printf("size of argv[%d]: %d\n", i - 1, argv[i] - last_address - 1);
        }
        last_address = argv[i];
        printf("Argument %d: %s\n", i, argv[i]);
    }
    printf("size of argv[%d]: %d\n", i - 1, get_last_position1(argv[i-1]) - last_address);

    char * test = "teste";
    printf("%s\n", test);

    return 0;
}

char *get_last_position1(char * first_postion) {
    char *i = first_postion;
    for(i = first_postion; *i != '\0'; i++) {

    }
    return i;
}