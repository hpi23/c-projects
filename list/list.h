#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

struct ListNode {
  bool is_initialized;
  void * value;
  struct ListNode *next;
};

typedef struct ListNode ListNode;

struct ListNode *list_new();

void list_append(struct ListNode *list, void * value);
void list_print(struct ListNode *list);
bool list_at(struct ListNode *list, uint64_t index, void **result);
ssize_t list_len(struct ListNode *list);
void list_delete(struct ListNode *list);
