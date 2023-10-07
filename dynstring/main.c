#include "./dynstring.h"

int main() {
    DynString * string = dynstring_from("Hello World");
    dynstring_print(string);
    dynstring_push_char(string, '!');
    dynstring_print(string);
    dynstring_push_string(string, " Mom");
    dynstring_print(string);

    dynstring_push_string(string, " Dad!");
    char * temp = dynstring_as_cstr(string);
    printf("%s\n", temp);
    return 0;
}
