#include "./vec.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// struct ListNode *primfaktoren(int input) {
//   int teiler = 2;
//   struct ListNode *list = list_new();
//   while (teiler <= input) {
//     if (input % teiler == 0) {
//       list_append(list, teiler);
//       input /= teiler;
//     } else {
//       teiler++;
//     }
//   }
//
//   return list;
// }

// void aufgabe_11() {
//   int inputs[] = {
//       60, 1024, 777, 1000, 77, 945252000,
//   };
//
//   for (int i = 0; i < sizeof(inputs) / sizeof(int); i++) {
//     struct ListNode *res = primfaktoren(inputs[i]);
//     printf("%d => \n", inputs[i]);
//     list_print(res);
//     list_delete(res);
//   }
// }

int main(void) {
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
