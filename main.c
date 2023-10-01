#include "./list.h"
#include "./tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode *primfaktoren(int input) {
  int teiler = 2;
  struct ListNode *list = list_new();
  while (teiler <= input) {
    if (input % teiler == 0) {
      list_append(list, teiler);
      input /= teiler;
    } else {
      teiler++;
    }
  }

  return list;
}

void aufgabe_11() {
  int inputs[] = {
      60, 1024, 777, 1000, 77, 945252000,
  };

  for (int i = 0; i < sizeof(inputs) / sizeof(int); i++) {
    struct ListNode *res = primfaktoren(inputs[i]);
    printf("%d => \n", inputs[i]);
    list_print(res);
  }
}

void tree() {
  TreeNode *tree = tree_new("Vater");

  TreeNode *sohn2 = tree_new("Sohn 2");
  tree_insert_value(sohn2, "Sohn Kind 1");

  tree_insert_value(tree, "Sohn 1");
  tree_insert_node(tree, sohn2);
  tree_insert_value(tree, "Sohn 3");
  tree_print(tree, 0);

  assert(tree_delete_by_value(tree, "Sohn 2"));

  tree_print(tree, 0);
  tree_delete(tree);
}

unsigned short hash(void *ptr, unsigned int val_len) {
  unsigned short *tmp = (unsigned short *)ptr;
  unsigned int hash = tmp[0];

  for (int i = 1; i < val_len / 2; i++) {
      hash = tmp[i - 1] ^ tmp[i];
  }

  return hash;
}

int main(void) {
  // aufgabe_11();
  // tree();
  
    unsigned short res = hash((void *) "hallo1", 6);
    printf("%d\n", res);
}

