#include "./json.h"

typedef struct {
  char *error;
  char *key;
  JsonValue *value;
} ParseResultObjectField;

typedef struct {
  char *error;
  JsonValueObject value;
} ParseResultObject;

typedef struct {
  char *error;
  JsonValueArray value;
} ParseResultArray;

typedef struct {
  Token curr_tok;
  Lexer lexer;
} JsonParser;

typedef struct {
  char *error;
  JsonParser parser;
} NewJsonParserResult;

typedef struct {
  char *error;
  JsonValue value;
} JsonParseResult;

char *parser_next(JsonParser *parser);
NewJsonParserResult parser_new(char *input);
void parser_free(JsonParser * parser);

JsonParseResult parse_json(JsonParser * parser);
void json_parse_result_free(JsonParseResult result);

ParseResultObject parse_object(JsonParser *parser);
ParseResultArray parser_parse_array(JsonParser *parser);
