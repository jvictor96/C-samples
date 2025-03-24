#include <qs_parser.h>

int main(int argc,char *argv[])
{
    struct element *parsed_data;
    parse_qs("name=Alice&age=30&sex=female", &parsed_data);
    char *result;
    get_value("sex", &result, parsed_data);
    printf("%s", result);
    get_value("age", &result, parsed_data);
    printf("%s", result);
    return 0;
}