#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

struct ListNode {
  bool is_initialized;
  void *value;
  struct ListNode *next;
};

typedef struct ListNode ListNode;

typedef struct {
  bool found;
  void *value;
} ListGetResult;

struct ListNode *
list_new();

void list_append(struct ListNode *list, void *value);
void list_print(struct ListNode *list);
ListGetResult list_at(struct ListNode *list, uint64_t index);
ssize_t list_len(struct ListNode *list);
void list_delete(struct ListNode *list);
