#include "./json.h"
#include "../hashmap/map.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void json_print_object(JsonValueObject object) {
  assert(object.fields != NULL);
  printf("{\n");

  ListNode *keys = hashmap_keys(object.fields);
  int len = list_len(keys);
  for (int i = 0; i < len; i++) {
    ListGetResult key = list_at(keys, i);
    assert(key.found);
    printf("    %s: ", (char *)key.value);

    MapGetResult value = hashmap_get(object.fields, (char *) key.value);
    assert(value.found);
    json_print_value(*(JsonValue *)value.value);

    printf("\n");
  }

  printf("}\n");
}

void json_print_array(JsonValueArray array) {
    ListNode * list = &array.fields;
    while (list->next != NULL) {
        assert(list->value != NULL);
        JsonValue * value = (JsonValue *) list->value;
        json_print_value(*value);
        list = list->next;
    }
}

void json_print_value(JsonValue value) {
  switch (value.type) {
  case JSON_TYPE_OBJECT:
    json_print_object(value.object);
    break;
  case JSON_TYPE_ARRAY:
    json_print_array(value.array);
  case JSON_TYPE_INT:
    printf("%ld", value.num_int);
    break;
  case JSON_TYPE_FLOAT:
    printf("%f", value.num_float);
    break;
  case JSON_TYPE_BOOL:
    if (value.boolean) {
        printf("true");
    } else {
        printf("false");
    }
    break;
  case JSON_TYPE_STRING:
    printf("%s", value.string);
    break;
  }
}
