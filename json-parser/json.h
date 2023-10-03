#include "../hashmap/map.h"
#include "../list/list.h"
#include "./lexer.h"
#include "./token.h"
#include <stdint.h>
#include <sys/types.h>

typedef enum {
  JSON_TYPE_OBJECT,
  JSON_TYPE_ARRAY,
  JSON_TYPE_INT,
  JSON_TYPE_FLOAT,
  JSON_TYPE_BOOL,
  JSON_TYPE_STRING,
} JsonType;

typedef struct {
  HashMap * fields;
} JsonValueObject;

typedef struct {
  ListNode fields;
} JsonValueArray;

typedef struct {
  JsonType type;
  JsonValueObject object;
  JsonValueArray array;
  double num_float;
  int64_t num_int;
} JsonValue;

void json_print_value(JsonValue value);
