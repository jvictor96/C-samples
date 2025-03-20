#include <stdio.h>
// -> Capítulo 16 - structures - 1:30 horas

typedef struct person
{
    char *name;
    int age;
    void (*greet)(struct person);
} person;

void greetImpl(person p);

void main(int narg, char **varg) {
    person jose = {
        "jose",
        27,
        greetImpl
    };

    person *pptr = &jose;

    pptr -> age = 28;
    pptr -> greet(*pptr);
}

void greetImpl(person p) {
    printf("My name is %s, I'm %d", p.name, p.age);
}