#include "httpd.h"
#include "repository.h"
#include "json.h"
#include "sql.h"
#include <stdio.h>
#include <stdlib.h>

int sock_sql;

int main(int c, char** v)
{
    sock_sql = my_connect("localhost", 5432, "jose", "jose");
    ddl(sock_sql);
    serve_forever("12913");
    return 0;
}

int get_p() {
    printf("HTTP/1.1 200 OK\r\n\r\n");
    struct element *result;
    char *rb = malloc(2048);
    get_all(&result, sock_sql);
    serialize(result, ' ', &rb);
    printf("%s", rb);
    free(rb);
}

int post_p() {
    printf("HTTP/1.1 200 OK\r\n\r\n");
    struct element *product, *name, *price;
    parseString(payload, &product);
    getValue(product, "name", &name);
    getValue(product, "price", &price);
    int id = post_product(name->value, price->value, sock_sql);
    struct element *id_kv = malloc(sizeof(struct element));
    char *id_kv_c = malloc(32);
    sprintf(id_kv_c, "{\"id\": \"%d\"}", id);
    parseString(id_kv_c, &id_kv);
    addValue(product, &(id_kv->list), &(id_kv->list->next));
    char *result;
    serialize(product, ' ', &result);
    printf("%s\r\n", result);
}

void route()
{
    if ((strcmp("/product",uri)==0||strcmp("/product",shortened_uri)==0)&&strcmp("GET",method)==0) 
    get_p();
    if ((strcmp("/product",uri)==0||strcmp("/product",shortened_uri)==0)&&strcmp("POST",method)==0) 
    post_p();
}

void comment()
{
}
