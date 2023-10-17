#include "./dynstring.h"
#include "../list/list.h"
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

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

  DynString *string = malloc(sizeof(DynString));
  string->capacity = from_length;
  string->length = from_length;
  string->internal_str = malloc(sizeof(char) * from_length);

  memcpy(string->internal_str, from, from_length);
  return string;
}

DynString *dynstring_from_memcpy(char *from, ssize_t amount) {
  assert(amount > 0);
  assert(from != NULL);
  ssize_t from_length = amount;

  DynString *string = malloc(sizeof(DynString));
  string->capacity = from_length;
  string->length = from_length;
  string->internal_str = malloc(sizeof(char) * from_length);

  memcpy(string->internal_str, from, from_length);
  return string;
}

void dynstring_push(DynString *string, DynString *add) {
  assert(string != NULL && add != NULL);
  while (string->capacity < string->length + add->length) {
    string->capacity *= 2;
  }

  dynstring__internal_grow(string);
  memcpy(&string->internal_str[string->length], add->internal_str, add->length);
  string->length = string->length + add->length;
}

void dynstring_push_char(DynString *string, char add) {
  assert(string != NULL);
  // Check if the capacity of the string must be extended
  if (string->capacity < string->length + 1) {
    string->capacity *= 2;
    dynstring__internal_grow(string);
  }

  string->internal_str[string->length] = add;
  string->length++;
}

void dynstring_push_string(DynString *string, char *add) {
  assert(string != NULL);
  // Check if the capacity of the string must be extended
  bool size_changed = false;
  ssize_t add_len = strlen(add);

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

// void dynstring_push_fmt(DynString *string, const char *fmt, ...) {
//   char *buf;
//   asprintf(&buf, fmt, ARGS);
//   dynstring_push_string(string, buf);
//   free(buf);
// }

char *dynstring_as_cstr(DynString *string) {
  assert(string != NULL);
  char *c_str = malloc(sizeof(char) * string->length + 1);
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

ssize_t dynstring_length(DynString *string) { return string->length; }

void dynstring_repeat(DynString *string, ssize_t n) {
  if (n == 0) {
    dynstring_clear(string);
    return;
  }

  if (n == 1) {
    return;
  }

  ssize_t new_size = string->length * n;
  while (new_size < string->capacity) {
    string->capacity *= 2;
  }
  dynstring__internal_grow(string);

  for (int i = 0; i < n; i++) {
    memcpy(&string->internal_str[i], string->internal_str, string->length);
  }

  string->length = new_size;
}

DynStringParseInt dynstring_parse_int64(DynString *string) {
  DynStringParseInt result = {.error = NULL};

  char *c_str = dynstring_as_cstr(string);

  char *remaining_string;
  result.num = strtoll(c_str, &remaining_string, 10);
  if (strlen(remaining_string) != 0 || errno != 0) {
    asprintf(&result.error, "Error: integer `%s` parse error", c_str);
    return result;
  }

  free(c_str);

  return result;
}

DynStringParseDouble dynstring_parse_double(DynString *string) {
  DynStringParseDouble result = {.error = NULL};

  char *c_str = dynstring_as_cstr(string);

  char *remaining_string;
  result.num = strtold(c_str, &remaining_string);
  if (strlen(remaining_string) != 0 || errno != 0) {
    asprintf(&result.error, "Error: double `%s` parse error", c_str);
    return result;
  }

  free(c_str);

  return result;
}

bool dynstring_strcmp(DynString *left, DynString *right) {
  if (left->length != right->length) {
    return false;
  }

  for (int i = 0; i < left->length; i++) {
    if (left->internal_str[i] != right->internal_str[i]) {
      return false;
    }
  }

  return true;
}

// TODO: improve this implementation?
ListNode *__dynstring_split_cstr_internal(DynString *base, char *delimeter, ssize_t delimeter_len, ssize_t limit) {
  ListNode *res = list_new();

  if (delimeter_len == 0 || delimeter_len > base->length) {
    return res;
  }

  ssize_t last_match_pos = 0;
  ssize_t matches = 0;

  for (ssize_t char_idx = 0; char_idx < base->length && (matches < limit || limit <= 0); char_idx++) {
    ssize_t match_idx = char_idx;
    ssize_t matched = 0;

    while (delimeter[match_idx - char_idx] == base->internal_str[match_idx]) {
      match_idx++;
      matched++;
    }

    if (matched > 0) {
      DynString *before = dynstring_from_memcpy(&base->internal_str[last_match_pos], match_idx - last_match_pos - matched);
      list_append(res, before);

      ssize_t slice_len = base->length - match_idx;
      if (slice_len > 0) {
        DynString *remaining = dynstring_from_memcpy(&base->internal_str[match_idx], slice_len);
        free(base);
        base = remaining;
        char_idx = -1;
        matches++;
      } else {
          dynstring_clear(base);
      }
    }
  }

  list_append(res, base);

  return res;
}

ListNode *dynstring_split_cstr(DynString *base, char *delimeter, ssize_t limit) {
  return __dynstring_split_cstr_internal(base, delimeter, strlen(delimeter), limit);
}

ListNode *dynstring_split(DynString *base, DynString *delimeter, ssize_t limit) {
  char *delimeter_temp = dynstring_as_cstr(delimeter);
  ListNode *res = __dynstring_split_cstr_internal(base, delimeter_temp, delimeter->length, limit);
  free(delimeter_temp);
  return res;
}

DynString dynstring_join(ListNode * list, DynString * delim) {
    // TODO: implement this
}

void dynstring_replace(DynString *base, DynString *what, ssize_t with) {
    ListNode * components = dynstring_split(base, what, 0);
}

// TODO: test this
void dynstring_set(DynString *string, char *content) {
  assert(string->internal_str != NULL);
  free(string->internal_str);

  ssize_t new_len = strlen(content);

  if (new_len == 0) {
    string->capacity = DEFAULT_CAPACITY;
  } else {
    string->capacity = new_len;
  }

  string->internal_str = malloc(string->capacity);

  if (new_len != 0) {
    memcpy(string->internal_str, content, new_len);
  }

  string->length = new_len;
}

void dynstring_clear(DynString *string) {
  assert(string->internal_str != NULL);
  free(string->internal_str);

  string->capacity = DEFAULT_CAPACITY;
  string->internal_str = malloc(string->capacity);
  string->length = 0;
}

void dynstring_free(DynString *string) {
  assert(string != NULL);
  assert(string->internal_str != NULL);
  free(string->internal_str);
  free(string);
}
