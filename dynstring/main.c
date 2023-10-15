#include "./dynstring.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
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
  char *c_str = dynstring_as_cstr(string);

  dynstring_free(string);

  DynString *num_int = dynstring_from("42a");
  DynStringParseInt res1 = dynstring_parse_int64(num_int);
  if (res1.error != NULL) {
    printf("%s\n", res1.error);
  }
  printf("int parse result: %ld\n", res1.num);

  DynString *num_float = dynstring_from("3.1415");
  DynStringParseDouble res2 = dynstring_parse_double(num_float);
  if (res2.error != NULL) {
    printf("%s\n", res2.error);
  }
  printf("double parse result: %f\n", res2.num);

  DynString * base = dynstring_from("CONCAT: Hello");
  DynString * add = dynstring_from(" World!");

  dynstring_push(base, add);
  dynstring_print(base);

  DynString * left = dynstring_from("Foo");
  DynString * right = dynstring_from("Foo");

  bool cmp_res = dynstring_strcmp(left, right);
  printf("Comparison result: %d\n", cmp_res);

  return 0;
}
