#include "./parser.h"
#include <stdio.h>

#define TEST_FILE "test.json"

char *read_test_file() {
  FILE *ptr;
  char ch;

  // Opening file in reading mode
  ptr = fopen(TEST_FILE, "r");

  if (NULL == ptr) {
    printf("File can't be opened \n");
    exit(1);
  }

  // Printing what is written in file
  // character by character using loop.
  do {
    ch = fgetc(ptr);
    printf("%c", ch);

    // Checking if character is not EOF.
    // If it is EOF stop reading.
  } while (ch != EOF);

  // Closing the file
  fclose(ptr);
  return 0;
}

int main() {

  char *input = "{ \"a_key\": 42, \"another\": 3.1415, \"a_string\": \"foo-bar\" }";
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
