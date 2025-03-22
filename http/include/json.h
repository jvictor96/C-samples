#ifndef _MJSON___
#define _MJSON___

struct element {
    struct element *next, *prev, *list;
    char *value;
    int type, size, list_id, open;
};

const int LIST = 1;
const int DICT = 2;
const int STRING = 3;

const int OPEN = 100;
const int CLOSED = 101;

int getNth(struct element, int, struct element **);
int getValue(struct element, char *, struct element **);
int parseString(char *, struct element **);
void serialize(struct element *parsed_data, char end_line, char** result);

#endif