#ifndef _PSQL_H___
#define _PSQL_H___

#include <json.h>

struct row {
    int column_amount;
    char **values;
    struct row *next_row;
};

int my_connect(char* host, int portno, char* user, char* database);
void execute_query(char* input, int sockfd, struct row **result);
void free_result(struct row **result);
void to_json(struct row *head, char** header, struct element **result);

#endif
