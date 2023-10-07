#include "./dynstring.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_CAPACITY 1

void dynstring__internal_grow(DynString *string) {
  char *old_ptr = string->internal_str;
  string->internal_str = malloc(sizeof(char) * string->capacity + 1);
  strcpy(string->internal_str, old_ptr);
  free(old_ptr);
}

DynString *dynstring_new() {
  assert(DEFAULT_CAPACITY > 0);
  DynString *string = malloc(sizeof(DynString));
  string->length = 0;
  string->capacity = DEFAULT_CAPACITY;
  string->internal_str = malloc(sizeof(char) * string->capacity);
  return string;
}

DynString *dynstring_from(char *from) {
  DynString *string = dynstring_new();
  free(string->internal_str);

  ssize_t from_length = strlen(from);
  string->capacity = from_length;
  string->length = from_length;
  dynstring__internal_grow(string);
  strcpy(string->internal_str, from);
  return string;
}

void dynstring_push_char(DynString *string, char add) {
  // Check if the capacity of the string must be extended
  if (string->capacity < string->length + 1) {
    string->capacity = string->capacity * 2;
    dynstring__internal_grow(string);
  }

  string->internal_str[string->length] = add;
  string->internal_str[string->length + 1] = '\0';
}

void dynstring_push_string(DynString *string, char *add) {
  // Check if the capacity of the string must be extended
  bool size_changed = false;
  while (string->capacity < string->length + strlen(add)) {
    string->capacity = string->capacity * 2;
    size_changed = true;
  }
  if (size_changed) {
    dynstring__internal_grow(string);
  }
  strcat(string->internal_str, add);
}

char *dynstring_as_cstr();

void dynstring_print(DynString *string) {
    printf("%s\n", string->internal_str);
}
