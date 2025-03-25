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

void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Hello! You are using %s", request_header("User-Agent"));
        printf("Query string = %s", qs);
    }

    ROUTE_GET("/product")
    {
        const char *last_part;
        const char *lastSlash = strrchr(uri, '/');  // Find last '/'
        if (lastSlash) {
            last_part = lastSlash + 1;  // Move past the '/'
        }
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("%s", last_part);
    }

    ROUTE_POST("/product")
    {
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
  
    ROUTE_END()
}

void comment()
{
}
