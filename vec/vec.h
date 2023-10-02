#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define VALUE_TYPE int
#define DEFAULT_CAPACITY 1

#define VEC_VERBOSE(...) printf(__VA_ARGS__)
//#define VEC_VERBOSE(...)

typedef struct {
  VALUE_TYPE *values;
  ssize_t capacity;
  ssize_t used;
} Vec;

Vec *vec_new_with_capacity(ssize_t capacity);
Vec *vec_new();
void vec_push(Vec *vec, VALUE_TYPE value);
void vec_pop(Vec *vec);
void vec_shrink_to_fit(Vec *vec);
void vec_print(Vec *vec);
VALUE_TYPE vec_index(Vec *vec, ssize_t index);
void vec_free(Vec * vec);
