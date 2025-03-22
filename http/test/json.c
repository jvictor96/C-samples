#include <json.h>

int main(int argc,char *argv[])
{
    struct element *parsed_data;
    char *result = malloc(1024);
    parseString("{\"name\": \"Alice\", \"age\": \"30\", \"children\":[{\"name\": \"Marcus\"}, {\"name\": \"Angela\"}], \"sex\": \"female\"}", &parsed_data);
    serialize(parsed_data, '\n', &result);

    parseString(result, &parsed_data);
    serialize(parsed_data, '\n', &result);

    parseString(result, &parsed_data);
    serialize(parsed_data, '\n', &result);
    return 0;
}