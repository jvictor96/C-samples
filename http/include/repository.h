#ifndef _PSQL_H___
#define _PSQL_H___

int ddl(int sockfd);
int post_product(char* name, float price, int sockfd);
int get_all(struct row **result, int sockfd);
int get_by_name(char* name, struct row **result, int sockfd);

#endif
