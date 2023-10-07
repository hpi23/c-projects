#include <stdlib.h>

typedef struct {
  char *internal_str;
  ssize_t capacity;
  ssize_t length;
} DynString;

DynString * dynstring_new();

DynString * dynstring_from(char * from);

// Appends a char to the end of this string
void dynstring_push_char(DynString * string, char add);

// Appends a string to the end of this string
void dynstring_push_string(DynString * string, char * add);

// Converts the underlying string to a c-string.
// Adds a NULL-terminator byte.
char * dynstring_as_cstr();

// Prints the string.
void dynstring_print(DynString *string);
