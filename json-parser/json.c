#include "./json.h"
#include "../dynstring/dynstring.h"
#include "../hashmap/map.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void json_print_value(JsonValue value) { puts(json_value_to_string(value)); }

char *json_object_to_string(JsonValueObject object) {
  assert(object.fields != NULL);
  DynString *buf = dynstring_from("{\n");

  ListNode *keys_start = hashmap_keys(object.fields);
  int len = list_len(keys_start);

  ListNode *keys = keys_start;
  for (int i = 0; i < len; i++) {
    ListGetResult key = list_at(keys, i);
    assert(key.found);

    MapGetResult value = hashmap_get(object.fields, (char *)key.value);
    assert(value.found);

    char *value_buf = json_value_to_string(*(JsonValue *)value.value);
    dynstring_push_fmt(buf, "    \"%s\": %s", (char *)key.value, value_buf);
    free(value_buf);

    // prevent trailing comma
    if (i + 1 < len) {
      dynstring_push_char(buf, ',');
    }

    dynstring_push_char(buf, '\n');
  }

  list_free(keys_start);

  dynstring_push_string(buf, "}\n");
  char *c_str = dynstring_as_cstr(buf);
  dynstring_free(buf);
  return c_str;
}

char *json_array_to_string(JsonValueArray array) {
  DynString *buf = dynstring_from("[");
  ListNode *list = array.fields;
  while (list != NULL) {
    assert(list->value != NULL);
    JsonValue *value = (JsonValue *)list->value;
    char * value_buf = json_value_to_string(*value);
    dynstring_push_string(buf, value_buf);
    free(value_buf);
    if (list->next != NULL) {
      dynstring_push_string(buf, ", ");
    }
    list = list->next;
  }
  dynstring_push_char(buf, ']');
  char *c_str = dynstring_as_cstr(buf);
  dynstring_free(buf);
  return c_str;
}

char *json_value_to_string(JsonValue value) {
  switch (value.type) {
  case JSON_TYPE_OBJECT:
    return json_object_to_string(value.object);
  case JSON_TYPE_ARRAY:
    return json_array_to_string(value.array);
  case JSON_TYPE_INT: {
    char *buf;
    asprintf(&buf, "%ld", value.num_int);
    return buf;
  }
  case JSON_TYPE_FLOAT: {
    char *buf;
    asprintf(&buf, "%f", value.num_float);
    return buf;
  }
  case JSON_TYPE_BOOL: {
    char *buf = malloc(5);
    if (value.boolean) {
      strcpy(buf, "true");
      break;
    } else {
      strcpy(buf, "false");
      break;
    }
    return buf;
  }
  case JSON_TYPE_STRING: {
    char *buf;
    asprintf(&buf, "\"%s\"", value.string);
    return buf;
  }
  }

  puts("Unreachable: every value case is handled above.");
  exit(1);
}

//
// Helper functions
//

void json_value_object_free(JsonValueObject obj) {
  ListNode *keys_start = hashmap_keys(obj.fields);

  ListNode *keys = keys_start;
  while (keys != NULL) {
    char *key = (char *)keys->value;
    assert(key != NULL);

    MapGetResult value = hashmap_get(obj.fields, key);
    assert(value.found);

    JsonValue *json_value = (JsonValue *)value.value;
    json_value_free(*json_value);

    free(json_value);

    keys = keys->next;
  }

  list_free(keys_start);

  hashmap_free(obj.fields);
}

void json_value_array_free(JsonValueArray value) {
  ListNode *elements = value.fields;

  while (elements != NULL) {
    JsonValue *current = (JsonValue *)elements->value;
    json_value_free(*current);
    free(current);

    elements = elements->next;
  }

  list_free(value.fields);
}

void json_value_free(JsonValue value) {
  switch (value.type) {
  case JSON_TYPE_OBJECT:
    json_value_object_free(value.object);
    break;
  case JSON_TYPE_ARRAY:
    json_value_array_free(value.array);
    break;
  case JSON_TYPE_STRING:
    free(value.string);
  default:
    // These types do not need to be freed
    return;
  }
}
