#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 1
#define VEC_FORMAT_SPECIFIER "%d"
#define VEC_VALUE_TYPE int

#define VEC_VERBOSE(...) printf(__VA_ARGS__)
// #define VEC_VERBOSE(...)

typedef struct {
  VEC_VALUE_TYPE *values;
  ssize_t capacity;
  ssize_t used;
} Vec;

Vec *vec_new_with_capacity(ssize_t capacity);
Vec *vec_new();
void vec_push(Vec *vec, VEC_VALUE_TYPE value);
void vec_pop(Vec *vec);
void vec_pop_front(Vec *vec);
void vec_shrink_to_fit(Vec *vec);
void vec_print(Vec *vec);
VEC_VALUE_TYPE vec_index(Vec *vec, ssize_t index);
void vec_free(Vec *vec);
