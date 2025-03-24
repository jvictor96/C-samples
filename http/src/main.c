#include "httpd.h"

int main(int c, char** v)
{
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

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("%s\r\n", payload);
    }
  
    ROUTE_END()
}
