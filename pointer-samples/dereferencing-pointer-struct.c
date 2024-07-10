#include <stdio.h>
// -> Capítulo 16 - structures - 1:30 horas

typedef struct person
{
    char *name;
    int age;
    void (*method)(person);
} person;

void greet(person p);

void main(int narg, char **varg) {
    person jose = {
        "jose",
        27,
        greet
    };

    person *pptr = &jose;

    pptr -> age = 28;
    pptr -> method(*pptr);
}

void greet(person p) {
    printf("My name is %s, I'm %d", p.name, p.age);
}