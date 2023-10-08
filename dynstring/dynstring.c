#include "./dynstring.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_CAPACITY 1

void dynstring__internal_grow(DynString *string) {
  char *old_ptr = string->internal_str;
  ssize_t old_len = string->length;
  string->internal_str = malloc(sizeof(char) * string->capacity);
  memcpy(string->internal_str, old_ptr, old_len);
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
  assert(from != NULL);
  ssize_t from_length = strlen(from);

  DynString * string = malloc(sizeof(DynString));
  string->capacity = from_length;
  string->length = from_length;
  string->internal_str = malloc(sizeof(char) * from_length);

  memcpy(string->internal_str, from, from_length);
  return string;
}

void dynstring_push_char(DynString *string, char add) {
  assert(string != NULL);
  // Check if the capacity of the string must be extended
  if (string->capacity < string->length + 1) {
    string->capacity = string->capacity * 2;
    dynstring__internal_grow(string);
  }

  string->internal_str[string->length] = add;
  string->length++;
}

void dynstring_push_string(DynString *string, char *add) {
  assert(string != NULL);
  // Check if the capacity of the string must be extended
  bool size_changed = false;
  ssize_t add_len =  strlen(add);

  while (string->capacity < string->length + add_len) {
    string->capacity = string->capacity * 2;
    size_changed = true;
  }
  if (size_changed) {
    dynstring__internal_grow(string);
  }
  memcpy(&string->internal_str[string->length], add, add_len);
  string->length += add_len;
}

char *dynstring_as_cstr(DynString *string) {
    assert(string != NULL);
    char * c_str = malloc(sizeof(char) * string->length + 1);
    memcpy(c_str, string->internal_str, string->length);
    c_str[string->length] = '\0';
    return c_str;
}

void dynstring_print(DynString *string) {
    assert(string != NULL);
    for (int i = 0; i < string->length; i++) {
        printf("%c", string->internal_str[i]);
    }
    printf("\n");
}

ssize_t dynstring_length(DynString *string) {
    return string->length;
}

void dynstring_free(DynString *string) {
    assert(string != NULL);
    assert(string->internal_str != NULL);
    free(string->internal_str);
    free(string);
}
