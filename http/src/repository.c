#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <string.h> /* memcpy, memset */
#include <unistd.h> /* read, write, close */
#include <sql.h>

int ddl(int sockfd) {
    struct row *result;
    execute_query("CREATE TABLE product(id int, name varchar(99), price float)", sockfd, &result);
}

int post_product(char* name, float price, int sockfd) {
    struct row *result;
    execute_query("SELECT COUNT(id) FROM PRODUCT", sockfd, &result);
    int id = atoi(result->values[0]);
    free_result(&result);
    char insert[100];
    sprintf(insert, "INSERT INTO PRODUCT(id, name, price) VALUES(%d, '%s', %f)", id, name, price);
    execute_query(insert, sockfd, &result);
}

int get_all(struct row **result, int sockfd) {
    execute_query("SELECT * FROM PRODUCT", sockfd, result);
}

int get_by_name(char* name, struct row **result, int sockfd) {
    char* base_query = "SELECT * FROM PRODUCT WHERE name = '%s'";
    char* final_query = malloc(strlen(base_query) + strlen(name) + 1);
    sprintf(final_query, base_query, name);
    execute_query(final_query, sockfd, result);
    free(final_query);
}


int main(int argc,char *argv[])
{
    int portno = 5432;
    char *host = "localhost";
    char *user = argv[1];
    char *database = argv[2];

    int sockfd = my_connect(host, portno, user, database);

    ddl(sockfd);
    post_product("cocacola", 4, sockfd);
    post_product("itaipava", 3, sockfd);
    post_product("heineken", 7.5, sockfd);
    struct row *result;
    get_all(&result, sockfd);
    free_result(&result);
    get_by_name("cocacola", &result, sockfd);
    free_result(&result);
    close(sockfd);

    return 0;
}