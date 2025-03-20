#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct linked_element {
    void *value;
    struct linked_element* next; 
};

int main(int narg, char **varg) {
    char input[100];
    struct linked_element dummy = {NULL, NULL},
                          *tail = &dummy,
                          *tmp;

    while (scanf("%s", input) && strcmp(input, "exit")) {
        tmp = malloc(sizeof(struct linked_element));
        tmp->value = strdup(input);
        tmp->next = NULL;
        tail->next = tmp;
        tail = tmp;
    }

    for (tmp = dummy.next; tmp; tmp = tmp->next)
        printf("%s\n", tmp->value);

    while (dummy.next) {
        tmp = dummy.next;
        dummy.next = tmp->next;
        free(tmp->value);
        free(tmp);
    }

    return 0;
}