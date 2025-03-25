#ifndef __MJSON_H__
#define __MJSON_H__

struct element {
    struct element *next, *prev, *list;
    char *value;
    int type, size, list_id, open;
};

extern int LIST,
        DICT,
        STRING,
        OPEN,
        CLOSED;

int getNth(struct element*, int, struct element **);
int getValue(struct element*, char *, struct element **);
int parseString(char *, struct element **);
void serialize(struct element *parsed_data, char end_line, char** result);
int addValue(struct element *head, struct element **key, struct element **value);
int addItem(struct element *head, struct element **item);

#endif