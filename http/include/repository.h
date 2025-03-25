#ifndef _REPO_H___
#define _REPO_H___

int ddl(int sockfd);
int post_product(char* name, char* price, int sockfd);
int get_all(struct row **result, int sockfd);
int get_by_name(char* name, struct row **result, int sockfd);

#endif
