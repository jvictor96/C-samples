#ifndef _REPO_H___
#define _REPO_H___

#include <sql.h>

int ddl(int sockfd);
int post_product(char* name, char* price, int sockfd);
int get_all(struct element **result, int sockfd);
int get_by_name(char* name, struct element **result, int sockfd);
int get_by_id(char* id, struct element **result, int sockfd);
int free_element(struct element *head);

#endif
