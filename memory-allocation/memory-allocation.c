#include <stdio.h>

struct linked_element {
    void *value;
    struct linked_element* next_element; 
};

void main(int narg, char **varg) {
    char *input;
    scanf("%s", input);
    struct linked_element* last_element;
    last_element = malloc(sizeof(struct linked_element));
    *last_element = (struct linked_element) { input, last_element };

    while (*input != "exit")
    {
        struct linked_element* tmp_element = malloc(sizeof(struct linked_element));
        *tmp_element = (struct linked_element) { input, last_element };
        scanf("%s", input);
    }
    
}