#include "./list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode *list_new() {
  struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
  node->is_initialized = false;
  return node;
}

void list_append(struct ListNode *list, long long value) {
  assert(list != NULL);

  while (list->next != NULL) {
    list = list->next;
  }

  if (list->is_initialized) {
    list->next = (struct ListNode *)malloc(sizeof(struct ListNode));
    list->next->value = value;
    list->next->is_initialized = true;
  } else {
    list->value = value;
    list->is_initialized = true;
  }
}

void list_print(struct ListNode *list) {
  printf("[");
  while (list != NULL && list->is_initialized) {
    printf("%lld", list->value);
    list = list->next;
    if (list != NULL) {
      printf(", ");
    }
  }
  printf("]\n");
}

// if this returns false, the index does not exist
bool list_at(struct ListNode *list, uint64_t index, long long *result) {
  for (int count = 0; (list != NULL && list->is_initialized) && count <= index; count++) {
    if (index == count) {
      assert(result != NULL);
      *result = list->value;
      return true;
    }

    list = list->next;
  }

  return false;
}
