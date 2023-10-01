#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct ListNode {
  bool is_initialized;
  long long value;
  struct ListNode *next;
};

struct ListNode *list_new();

void list_append(struct ListNode *list, long long value);
void list_print(struct ListNode *list);
bool list_at(struct ListNode *list, uint64_t index, long long *result);
