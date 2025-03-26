#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <string.h> /* memcpy, memset */
#include <unistd.h> /* read, write, close */
#include <sql.h>

int ddl(int sockfd) {
    struct row *result;
    execute_query("DROP TABLE IF EXISTS product", sockfd, &result);
    execute_query("CREATE TABLE IF NOT EXISTS product(id int, name varchar(99), price float)", sockfd, &result);
}

int post_product(char* name, char* price, int sockfd) {
    struct row *result;
    execute_query("SELECT COUNT(id) FROM PRODUCT", sockfd, &result);
    int id = atoi(result->values[0]);
    free_result(&result);
    char insert[100];
    sprintf(insert, "INSERT INTO PRODUCT(id, name, price) VALUES(%d, '%s', %s)", id, name, price);
    execute_query(insert, sockfd, &result);
    return id;
}

int to_product_json(struct row *head, struct element **result) {
    char *headers[] = {"id", "name", "price"};
    printf("to_json\n");
    to_json(head, headers, result);
    printf("to_json\n");
}

int get_all(struct element **result, int sockfd) {
    struct row *rows;
    execute_query("SELECT * FROM PRODUCT", sockfd, &rows);
    to_product_json(rows, result);
}

int get_by_name(char* name, struct element **result, int sockfd) {
    struct row *rows;
    char* base_query = "SELECT * FROM PRODUCT WHERE name = '%s'";
    char* final_query = malloc(strlen(base_query) + strlen(name) + 1);
    sprintf(final_query, base_query, name);
    execute_query(final_query, sockfd, &rows);
    free(final_query);
    to_product_json(rows, result);
}

int get_by_id(char* id, struct element **result, int sockfd) {
    struct row *rows;
    char* base_query = "SELECT * FROM PRODUCT WHERE id = '%s'";
    char* final_query = malloc(strlen(base_query) + strlen(id) + 1);
    sprintf(final_query, base_query, id);
    execute_query(final_query, sockfd, &rows);
    free(final_query);
    to_product_json(rows, result);
}