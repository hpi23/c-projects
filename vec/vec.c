#include "./vec.h"
#include <stdio.h>
#include <stdlib.h>

void vec_set_size(Vec *vec, ssize_t new_size);

Vec *vec_new_with_capacity(ssize_t capacity) {
  Vec *vec_temp = (Vec *)malloc(sizeof(Vec));
  vec_temp->capacity = capacity;
  vec_temp->used = 0;
  vec_temp->values = malloc(sizeof(VALUE_TYPE) * vec_temp->capacity);
  return vec_temp;
}

Vec *vec_new() {
  Vec *vec_temp = vec_new_with_capacity(DEFAULT_CAPACITY);
  return vec_temp;
}

void vec_push(Vec *vec, VALUE_TYPE value) {
  assert(vec != NULL);
  if (vec->capacity <= vec->used) {
    ssize_t new_capacity = vec->capacity * 2;
    if (new_capacity == 0) {
      new_capacity = 1;
    }

    VEC_VERBOSE("vec: growing from old capacity %ld to new capacity: %ld\n",
                vec->capacity, new_capacity);
    vec_set_size(vec, new_capacity);
  }
  vec->values[vec->used] = value;
  vec->used++;
}

void vec_set_size(Vec *vec, ssize_t new_size) {
  vec->capacity = new_size;
  vec->values =
      (VALUE_TYPE *)realloc(vec->values, new_size * sizeof(VALUE_TYPE));
}

// Removes the last element of the vector without deallocating memory.
void vec_pop(Vec *vec) {
  if (vec->used == 0) {
    return;
  }
  vec->used--;
}

// Shrinks the vector as much as possible.
void vec_shrink_to_fit(Vec *vec) {
  // cannot shrink, everything is used
  if (vec->capacity == vec->used) {
    return;
  }

  ssize_t new_capacity = vec->used;
  if (new_capacity == 0) {
    new_capacity = 1;
  }

  VEC_VERBOSE("vec: shrinking from old size %ld to new size of %ld\n", vec->capacity, new_capacity);
  vec_set_size(vec, new_capacity);
}

void vec_print(Vec *vec) {
#if VALUE_TYPE == int
#define VEC_PRINT_FORMAT_SPECIFIER "%d"
#else
#define VEC_PRINT_FORMAT_SPECIFIER "%v"
#endif

  printf("[");
  for (int i = 0; i < vec->used; i++) {
    printf(VEC_PRINT_FORMAT_SPECIFIER, vec->values[i]);
    if (i + 1 < vec->used) {
      printf(", ");
    }
  }
  printf("]\n");
}

VALUE_TYPE vec_index(Vec *vec, ssize_t index) {
  if (index < 0 || index >= vec->used) {
    printf("vec_index(): Illegal index %ld\n", index);
    exit(1);
  }

  return vec->values[index];
}

void vec_free(Vec *vec) {
  free(vec->values);
  free(vec);
}
