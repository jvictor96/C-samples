#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <string.h> /* memcpy, memset */
#include <unistd.h> /* read, write, close */
#include <sql.h>

int ddl(int sockfd) {
    struct row *result;
    execute_query("CREATE TABLE IF NOT EXISTS product(id int, name varchar(99), price float)", sockfd, &result);
}

int post_product(char* name, char price, int sockfd) {
    struct row *result;
    execute_query("SELECT COUNT(id) FROM PRODUCT", sockfd, &result);
    int id = atoi(result->values[0]);
    free_result(&result);
    char insert[100];
    sprintf(insert, "INSERT INTO PRODUCT(id, name, price) VALUES(%d, '%s', %s)", id, name, price);
    execute_query(insert, sockfd, &result);
    return id;
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