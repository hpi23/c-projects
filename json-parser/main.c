#include "./parser.h"
#include <stdio.h>

int main() {
  char *input = "{ \"a_key\": 42 }";
  NewJsonParserResult result = parser_new(input);
  if (result.error != NULL) {
    printf("%s\n", result.error);
    exit(1);
  }

  JsonParser parser = result.parser;
  JsonParseResult parse_result = parse_json(&parser);
  if (parse_result.error != NULL) {
    printf("%s\n", parse_result.error);
    exit(2);
  }

  json_print_value(parse_result.value);

  return 0;
}
