#include <stdlib.h>

typedef struct {
  char *internal_str;
  ssize_t capacity;
  ssize_t length;
} DynString;

DynString *dynstring_new();

DynString *dynstring_from(char *from);

// Appends a char to the end of this string
void dynstring_push_char(DynString *string, char add);

// Appends a string to the end of this string
void dynstring_push_string(DynString *string, char *add);

// This needs to be a macro because of weird varargs in c.
#define dynstring_push_fmt(dynstring, fmt, ...)                                \
  char *__internal_buf;                                                        \
  asprintf(&__internal_buf, fmt, ##__VA_ARGS__);                               \
  dynstring_push_string(dynstring, __internal_buf);                            \
  free(__internal_buf);

// Converts the underlying string to a c-string.
// Adds a NULL-terminator.
char *dynstring_as_cstr(DynString *string);

// Prints the string.
void dynstring_print(DynString *string);

// Repeats the contents of the string n times.
void dynstring_repeat(DynString *string, ssize_t n);

// Updates the value of the string.
void dynstring_set(DynString *string, char * content);

// Clears the string (sets the string back to default)
void dynstring_clear(DynString *string);

void dynstring_free(DynString *string);
