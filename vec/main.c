#include "./vec.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void vec_normal() {
  Vec *vec = vec_new();
  for (int i = 0; i < 10; i++)
    vec_push(vec, 10 - i);
  vec_print(vec);
  printf("%d\n", vec_index(vec, 2));

  vec_pop(vec);
  vec_pop(vec);
  vec_pop(vec);
  vec_pop(vec);

  vec_print(vec);
  vec_shrink_to_fit(vec);
  vec_print(vec);
  vec_push(vec, 42);
  vec_print(vec);
  vec_free(vec);
}

int main2(void) {
  vec_normal();
  printf("======\n");
  Vec *vec = vec_new_with_capacity(10);
  for (int i = 0; i < 10; i++) {
    vec_push(vec, i);
  }

  vec_print(vec);
  vec_pop(vec);
  vec_print(vec);
  vec_pop_front(vec);
  vec_pop_front(vec);
  vec_print(vec);

  vec_free(vec);
  return 0;
}

int main() {
  printf("======\n");
  Vec *vec = vec_new_with_capacity(10);
  for (int i = 0; i < 10; i++) {
    vec_push(vec, i);
  }

  vec_print(vec);

  for (int i = 0; i < vec->used; i++) {
    if (vec_index(vec, i) % 2 == 0) {
        vec_remove(vec, i);
    }
  }

  vec_print(vec);

  vec_remove(vec, vec->used-1);

  vec_print(vec);

  vec_free(vec);
}
