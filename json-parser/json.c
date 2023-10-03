#include "./json.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void json_print_object(JsonValueObject value) {
  assert(value.fields != NULL);
  printf("{\n");

  ListNode *keys = hashmap_keys(value.fields);
  int len = list_len(keys);
  for (int i = 0; i < len; i++) {
    ListGetResult key = list_at(keys, i);
    assert(key.found);
    printf("    %s: ", (char *)key.value);

    // TODO: print value somehow

    printf("\n");
  }

  printf("}\n");
}

void json_print_value(JsonValue value) {
  switch (value.type) {
  case JSON_TYPE_OBJECT:
    json_print_object(value.object);
    break;
  case JSON_TYPE_ARRAY:
  case JSON_TYPE_INT:
  case JSON_TYPE_FLOAT:
  case JSON_TYPE_BOOL:
  case JSON_TYPE_STRING:
    printf("not supported\n");
    exit(1);
  }
}
