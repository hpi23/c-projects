#include "./dynstring.h"
#include <stdio.h>

int main() {
  DynString *string = dynstring_from("Hello World");
  dynstring_print(string);
  dynstring_push_char(string, '!');
  dynstring_print(string);
  dynstring_push_string(string, " Mom");
  dynstring_print(string);

  dynstring_push_string(string, " Dad!");
  char *temp = dynstring_as_cstr(string);
  printf("%s\n", temp);
  free(temp);

  dynstring_push_fmt(string, "%d", 42);
  dynstring_print(string);

  dynstring_set(string, ".");
  dynstring_print(string);
  dynstring_repeat(string, 4);
  dynstring_print(string);
  dynstring_repeat(string, 0);
  dynstring_print(string);


  dynstring_set(string, "Hello World!");
  dynstring_print(string);
  dynstring_clear(string);
  char * c_str = dynstring_as_cstr(string);
  printf("STR `%s`\n", c_str);

  dynstring_free(string);

  return 0;
}
