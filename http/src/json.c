#include <json.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int LIST = 1;
int DICT = 2;
int STRING = 3;

int OPEN = 100;
int CLOSED = 101;


int getFirst(struct element tail, struct element **result) {
    // It returns the first element, not the anchor
    int list_id = tail.list_id;
    *result = &tail;
    //printf("   %p %d\n", *result, (*result)->list_id);
    while((*result)->prev && (*result)->prev->list_id == list_id) {
        *result = (*result)->prev;
        //printf("   %p %d\n", *result, (*result)->list_id);
    }
};


int getLast(struct element *head, struct element **result) {
    *result = head->list;
    while(*result && (*result)->next) {
        *result = (*result)->next;
    }
};

int getNth(struct element *head, int n, struct element **result) {
    *result = head->list;
    for(int i = 0; i < n; i++) {
        if (!(*result)->next) {
            return 1;  // Out of bounds
        }
        *result = (*result)->next;
    }
};

int addItem(struct element *head, struct element **item) {
    struct element *tail;
    getLast(head, &tail);
    tail->next = *item;
    (*item)->prev = tail;
    return 0;
}

int getValue(struct element *head, char * key, struct element **result) {
    if(head->type == LIST) {
        return 1;
    }

    *result = head->list;
    while (*result) {
        if (strcmp((*result)->value, key) == 0) {
                *result = (*result)->next;
            return 0;  // Found key
        }
        *result = (*result)->next;
        *result = (*result)->next;
    }
}

int addValue(struct element *head, struct element **key, struct element **value) {
    struct element *tail;
    getLast(head, &tail);
    tail->next = *key;
    (*key)->prev = tail;
    (*key)->next = *value;
    (*value)->prev = *key;
    return 0;
}

struct element element_builder(int type) {
    struct element new_e = {
        .next = NULL, 
        .prev = NULL, 
        .list = NULL,
        .value = NULL,
        .type = type
    };
    return new_e;
}

void putElementList(struct element **result, struct element **element_ptr, int type, int list_id) {
    struct element *new_elem = malloc(sizeof(struct element));
    new_elem->type = type;
    new_elem->size = 0;
    new_elem->prev = (*element_ptr);
    new_elem->next = NULL;
    new_elem->open = OPEN;

    if (*result == NULL) {
        new_elem->list_id = list_id;
        *result = new_elem;
        *element_ptr = new_elem;
        return;
    }

    if(((*element_ptr)->type == LIST || (*element_ptr)->type == DICT) 
    && !(*element_ptr)->list
    && (*element_ptr)->open == OPEN) {
        (*element_ptr)->list = new_elem;
        new_elem->list_id = list_id;
    } else if (((*element_ptr)->type == LIST || (*element_ptr)->type == DICT)
    && (*element_ptr)->open == OPEN){
        struct element *tail;
        getLast(*element_ptr, &tail);
        tail->next = new_elem;
        new_elem->list_id = tail->list_id;
    } else {
        (*element_ptr)->next = new_elem;
        new_elem->list_id = (*element_ptr)->list_id;
    }
    *element_ptr = new_elem;
}

int parseString(char *string, struct element **result) {
    if(string[0] == '\0') {
        return -1;
    }
    int offset = 0, list_id = 1;
    struct element *element_ptr = NULL;
    *result = NULL;

    while(string[offset] != '\0') {
        //printf("%c\n", string[offset]);
        if(string[offset] =='{') {
            putElementList(result, &element_ptr, DICT, list_id);
            list_id ++;
            //printf("%p  %d\n", element_ptr, element_ptr->list_id);
            offset++;
        } else if (string[offset] == '[') {
            putElementList(result, &element_ptr, LIST, list_id);
            list_id ++;
            //printf("%p  %d\n", element_ptr, element_ptr->list_id);
            offset++;
        } else if(string[offset] == '\"') {
            struct element *new_elem = malloc(sizeof(struct element));
            new_elem->type = STRING;
            new_elem->value = malloc(64);  // Allocate space for the string
            new_elem->next = NULL;

            offset++;  // Move past the opening quote
            int value_offset = 0;
            while (string[offset] != '\"' && string[offset] != '\0') {
                new_elem->value[value_offset++] = string[offset++];
            }
            offset++;  // Move past the closing quote
            new_elem->value[value_offset] = '\0';

            if((element_ptr->type == LIST || 
            element_ptr->type == DICT) && 
            !element_ptr->list && 
            element_ptr->open == OPEN) {
                element_ptr->list = new_elem;
                new_elem->list_id = list_id;
            } else {
                element_ptr->next = new_elem;
                new_elem->list_id = element_ptr->list_id;
            }
            new_elem->prev = element_ptr;
            element_ptr = new_elem;
        } else if (string[offset] == ']' || string[offset] == '}') {
            struct element *head;
            if(element_ptr->type == STRING ) {
                getFirst(*element_ptr, &head);
                element_ptr = head->prev;
                //printf("%p %d\n", element_ptr, element_ptr->list_id);
                if(element_ptr->prev) {
                    getFirst(*element_ptr, &head);
                    element_ptr = head->prev;
                }
            } else {
                element_ptr->open = CLOSED;
            }
            //printf("%p %d\n", element_ptr, element_ptr->list_id);
            offset++;
        }
        while (string[offset] == ' ' || 
        string[offset] == ':' || 
        string[offset] == '\n' || 
        string[offset] == ',' )
        {
            offset++;
        }
    }
}

void print_rec(struct element *head, int type, char end_line, char** result) {
    printf("print_rec\n");
    int n = 1;
    //printf("%s\n", *result);
    struct element *node_ptr = head;
    while(node_ptr) {
        if(node_ptr->list) {
            if (node_ptr->type == DICT)
            {
                sprintf(*result, "%s{%c", *result, end_line);
                print_rec(node_ptr->list, node_ptr->type, end_line, result);
                sprintf(*result, "%s%c} ", *result, end_line);
                if(node_ptr->next) {
                    sprintf(*result, "%s,%c", *result, end_line);
                }
            } else {
                sprintf(*result, "%s[", *result);
                print_rec(node_ptr->list, node_ptr->type, end_line, result);
                sprintf(*result, "%s%c] ", *result, end_line);
                if(node_ptr->next) {
                    sprintf(*result, "%s,", *result);
                }
            }
            
        }
        
        if(node_ptr->value) {
            sprintf(*result, "%s\"%s\"", *result, node_ptr->value);
            if(type == DICT) {
                if(n % 2 == 1) {
                    sprintf(*result, "%s: ", *result);
                } else if(node_ptr->next) {
                    sprintf(*result, "%s,%c", *result, end_line);
                } else {
                    sprintf(*result, "%s ", *result);
                }
            } else {
                sprintf(*result, "%s, ", *result);
            }
        }
        node_ptr = node_ptr->next;
        n++;
    }
}

void serialize(struct element *parsed_data, char end_line, char** result) {
    sprintf(*result, "{%c", end_line);
    print_rec(parsed_data->list, DICT, end_line, result);
    sprintf(*result, "%s%c}", *result, end_line);
    //printf("%s\n", *result);
}
