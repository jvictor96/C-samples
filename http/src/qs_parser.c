#include <json.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct element *parse_qs(char *qs, struct element **head) {
    int offset = 0;
    struct element *tail = NULL;
    int iskey = 1;
    *head = NULL;
    while(qs[offset] != '\0') {
        //printf("%c\n", qs[offset]);
        if (iskey == 1) {
            struct element *new_element = malloc(sizeof(struct element));
            new_element -> value = malloc(64);
            int size = 0;
            while (qs[offset] != '=') {
                //printf("%c\n", qs[offset]);
                new_element -> value[size] = qs[offset];
                size ++;
                offset++;
            }
            new_element -> value[size] = '\0';
            if(*head) {
                //printf("last");
                tail->next = new_element;
                tail = new_element;
            } else {
                //printf("first");
                *head = new_element;
                tail = new_element;
            }
            iskey = 0;
            
        } else {
            struct element *new_element = malloc(sizeof(struct element));
            new_element -> value = malloc(256);
            int size = 0;
            while(qs[offset] != '&' && qs[offset] != '\0') {
                //printf("%c\n", qs[offset]);
                new_element -> value[size] = qs[offset];
                size ++;
                offset++;
            }
            new_element -> value[size] = '\0';
            iskey = 1;
            tail->next = new_element;
            tail = new_element;
        }
        if(qs[offset] != '\0') {
            offset ++;
        }
    }
}

char *get_value(char *key, char** result, struct element *element) {
    while (element)
    {
        if (strcmp(element->value,key) == 0) {
            break;
        }
        element = element->next;
        element = element->next;
    }
    *result = element->next->value;
}

int main(int argc,char *argv[])
{
    struct element *parsed_data;
    parse_qs("name=Alice&age=30&sex=female", &parsed_data);
    char *result;
    get_value("sex", &result, parsed_data);
    printf("%s", result);
    get_value("age", &result, parsed_data);
    printf("%s", result);
    return 0;
}