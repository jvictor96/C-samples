#include <json.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct element *parse_qs(char *qs, struct element **head) {
    int offset = 0;
    struct element *tail = NULL, *new_element = malloc(sizeof(struct element));
    int size = 0, key = 1;
    while(qs[offset] != '\n') {
        char *key = malloc(64), *value = malloc(256);
        if (key == 1) {
            if(qs[offset] != '=') {
                key[size] = qs[offset];
                size ++;
                offset ++;
            } else {
                new_element -> value = key;
                if(*head) {
                    tail->next = new_element;
                    tail = new_element;
                } else {
                    *head = new_element;
                    tail = new_element;
                }
                size = 0;
                key = 0;
                new_element = malloc(sizeof(struct element));
            }
        } else {
            if(qs[offset] != '&') {
                value[size] = qs[offset];
                size ++;
                offset ++;
            } else {
                size = 0;
                key = 1;
                new_element = malloc(sizeof(struct element));
                tail->next = new_element;
                tail = new_element;
            }
        }
    }
}

char *get_value(char *qs) {

}