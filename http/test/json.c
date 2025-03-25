#include <json.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
    struct element *parsed_data;
    char *result = malloc(1024);
    parseString("{\"name\": \"Alice\", \"age\": \"30\", \"children\":[{\"name\": \"Marcus\"}, {\"name\": \"Angela\"}], \"sex\": \"female\"}", &parsed_data);
    serialize(parsed_data, ' ', &result);

    parseString(result, &parsed_data);
    serialize(parsed_data, ' ', &result);

    parseString(result, &parsed_data);
    serialize(parsed_data, ' ', &result);

    struct element *name, *sex;
    getValue(parsed_data, "name", &name);
    getValue(parsed_data, "sex", &sex);

    printf("name %s", name->value);
    printf("sex %s", sex->value);
    return 0;
}