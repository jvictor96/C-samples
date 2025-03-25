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

    printf("name %s\n", name->value);
    printf("sex %s\n", sex->value);

    struct element *children, *child, *child_name;
    getValue(parsed_data, "children", &children);
    getNth(children, 1, &child);
    getValue(child, "name", &child_name);
    printf("child name %s\n", child_name->value);

    struct element  *cash = malloc(sizeof(struct element));
    parseString("{\"cash\": \"1850.0\"}", &cash);
    addValue(parsed_data, &cash->list, &cash->list->next);

    getValue(parsed_data, "cash", &cash);
    printf("cash %s\n", cash->value);

    struct element *new_child;
    parseString("{\"name\": \"Vinicius\"}", &new_child);
    addItem(children, &new_child);

    getValue(parsed_data, "children", &children);
    getNth(children, 2, &child);
    getValue(child, "name", &child_name);
    printf("child name %s\n", child_name->value);
    return 0;
}