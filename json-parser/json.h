#include "./token.h"
#include "./lexer.h"

typedef enum {
  JSON_TYPE_OBJECT,
  JSON_TYPE_ARRAY,
  JSON_TYPE_INT,
  JSON_TYPE_FLOAT,
  JSON_TYPE_BOOL,
  JSON_TYPE_STRING,
} JsonType;


typedef struct {
  JsonType type;
} JsonValue;

typedef struct {
  char *error;
  JsonValue value;
} ParseResult;

typedef struct {
} JsonValueObject;
