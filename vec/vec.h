#include <stdint.h>
#include <assert.h>

#def VALUE_TYPE int
#def DEFAULT_CAPACITY 0

typedef struct {
    VALUE_TYPE *values;
    ssize_t capacity;
    ssize_t used;
} Vec;


Vec * vec_new_with_capacity(ssize_t capacity) {
    Vec * vec_temp = (Vec *) malloc(sizeof(Vec));
    vec_temp->capacity = capacity;
    vec_temp->used = 0;
    vec_temp->values = malloc(sizeof(VALUE_TYPE) * vec_temp->capacity);
    return vec_temp;
}

Vec * vec_new() {
    Vec * vec_temp = vec_new_with_capacity(DEFAULT_CAPACITY);
    return vec_temp;
}

void vec_push(Vec * vec, VALUE_TYPE value) {
    assert(vec != NULL);
    if (vec->capacity <= vec->used) {
        vec_grow_to_size(vec, vec->capacity * 2);
    }
    void * ptr = &vec->values[vec->used];
    vec->used++;
    *ptr = value;
}
