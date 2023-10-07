#include "./parser.h"
#include "./lexer.h"
#include "./token.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hashmap/map.h"

char *parser_next(JsonParser *parser) {
  TokenResult result = lexer_next_token(&parser->lexer);
  parser->curr_tok = result.token;
  if (result.error != NULL) {
    return result.error;
  }
  return NULL;
}

NewJsonParserResult parser_new(char *input) {
  NewJsonParserResult result = {.error = NULL};

  Lexer lexer = lexer_new(input);
  JsonParser parser = {.lexer = lexer};
  char *err = parser_next(&parser);
  if (err != NULL) {
    result.error = err;
    return result;
  }

  result.parser = parser;
  return result;
}

JsonParseResult parse_json(JsonParser *parser) {
  JsonParseResult result = {.error = NULL};

  switch (parser->curr_tok.kind) {
  case TOKENKIND_LBRACE: {
    ParseResultObject obj_result = parse_object(parser);
    if (obj_result.error != NULL) {
      result.error = obj_result.error;
      return result;
    }
    result.value.type = JSON_TYPE_OBJECT;
    result.value.object = obj_result.value;
    break;
  }
  case TOKENKIND_LBRACKET: {
    ParseResultArray array_result = parse_array(parser);
    if (array_result.error != NULL) {
      result.error = array_result.error;
      return result;
    }
    result.value.type = JSON_TYPE_ARRAY;
    result.value.array = array_result.value;
    break;
  }
  case TOKENKIND_INT: {
    result.value.type = JSON_TYPE_INT;
    char *remaining_string;
    result.value.num_int = strtoll(parser->curr_tok.value, &remaining_string, 10);
    if (strlen(remaining_string) != 0 || errno != 0) {
      asprintf(&result.error, "Error: integer `%s` parse error", parser->curr_tok.value);
      return result;
    }

    char *error = parser_next(parser);
    if (error != NULL) {
      result.error = error;
      return result;
    }

    break;
  }
  case TOKENKIND_FLOAT: {
    result.value.type = JSON_TYPE_FLOAT;
    char *remaining_string;
    result.value.num_float = strtold(parser->curr_tok.value, &remaining_string);
    if (strlen(remaining_string) != 0 || errno != 0) {
      asprintf(&result.error, "Error: float `%s` parse error", parser->curr_tok.value);
      return result;
    }

    char *error = parser_next(parser);
    if (error != NULL) {
      result.error = error;
      return result;
    }

    break;
  }
  case TOKENKIND_STRING:
    result.value.type = JSON_TYPE_STRING;
    result.value.string = malloc(strlen(parser->curr_tok.value) + 1);
    strcpy(result.value.string, parser->curr_tok.value);

    char *error = parser_next(parser);
    if (error != NULL) {
      result.error = error;
      return result;
    }

    break;
  default:
    asprintf(&result.error, "Error: exected JSON value, got `%s`", display_tokenkind(parser->curr_tok.kind));
    return result;
  }

  return result;
}

ParseResultObjectField parse_object_field(JsonParser *parser) {
  ParseResultObjectField result = {.error = NULL};

  // expect a key (string)
  if (parser->curr_tok.kind != TOKENKIND_STRING) {
    asprintf(&result.error, "Error: expected `STRING`, got `%s`", display_tokenkind(parser->curr_tok.kind));
    return result;
  }

  result.key = malloc(strlen(parser->curr_tok.value) + 1);
  strcpy(result.key, parser->curr_tok.value);

  char *error = parser_next(parser);
  if (error != NULL) {
    result.error = error;
    return result;
  }

  // expect a `:`
  if (parser->curr_tok.kind != TOKENKIND_COLON) {
    asprintf(&result.error, "Error: expected `COLON`, got `%s`", display_tokenkind(parser->curr_tok.kind));
    return result;
  }

  error = parser_next(parser);
  if (error != NULL) {
    result.error = error;
    return result;
  }

  JsonParseResult value_res = parse_json(parser);
  if (value_res.error != NULL) {
    result.error = value_res.error;
    return result;
  }
  result.value = malloc(sizeof(JsonValue));
  *result.value = value_res.value;
  return result;
}

ParseResultObject parse_object(JsonParser *parser) {
  ParseResultObject result = {.error = NULL};

  // Skip the `{`
  char *err = parser_next(parser);
  if (err != NULL) {
    result.error = err;
    return result;
  }

  HashMap *field_map = hashmap_new();

  if (parser->curr_tok.kind != TOKENKIND_RBRACE && parser->curr_tok.kind != TOKENKIND_EOF) {
    // make initial field
    ParseResultObjectField field_res = parse_object_field(parser);
    if (field_res.error != NULL) {
      result.error = field_res.error;
      return result;
    }
    hashmap_insert(field_map, field_res.key, field_res.value);

    while (parser->curr_tok.kind == TOKENKIND_COMMA) {
      // skip comma
      char *err = parser_next(parser);
      if (err != NULL) {
        result.error = err;
        return result;
      }

      // prevent a trailing comma
      if (parser->curr_tok.kind == TOKENKIND_RBRACE) {
        result.error = "Error: trailing comma is not allowed";
        return result;
      }

      if (parser->curr_tok.kind == TOKENKIND_EOF) {
        break;
      }

      // make other fields
      ParseResultObjectField field_res = parse_object_field(parser);
      if (field_res.error != NULL) {
        result.error = field_res.error;
        return result;
      }
      hashmap_insert(field_map, field_res.key, field_res.value);
    }
  }

  if (parser->curr_tok.kind != TOKENKIND_RBRACE) {
    asprintf(&result.error, "Error: exected token `%s`, found `%s`", display_tokenkind(TOKENKIND_RBRACE), display_tokenkind(parser->curr_tok.kind));
    return result;
  }

  JsonValueObject object = {.fields = field_map};
  result.value = object;
  return result;
}

ParseResultArray parser_parse_array(JsonParser *parser) {
  JsonValueArray array = {.fields = list_new()};
  ParseResultArray result = {.value = array, .error = NULL};

  // skip the `[`
  char *err = parser_next(parser);
  if (err != NULL) {
    result.error = err;
    return result;
  }

  while (parser->curr_tok.kind == TOKENKIND_COMMA) {
    // skip the `,`
    char *err = parser_next(parser);
    if (err != NULL) {
      result.error = err;
      return result;
    }

    // prevent a trailing comma
    if (parser->curr_tok.kind == TOKENKIND_RBRACE) {
      result.error = "Error: trailing comma is not allowed";
      return result;
    }

    if (parser->curr_tok.kind == TOKENKIND_EOF) {
      break;
    }

    // make the value
    JsonParseResult value_result = parse_json(parser);
    if (value_result.error != NULL) {
      result.error = value_result.error;
      return result;
    }

    JsonValue *value = (JsonValue *)malloc(sizeof(JsonValue));
    *value = value_result.value;
    list_append(&array.fields, value);
  }

  if (parser->curr_tok.kind != TOKENKIND_RBRACKET) {
    asprintf(&result.error, "Error: exected token `%s`, found `%s`", display_tokenkind(TOKENKIND_RBRACKET), display_tokenkind(parser->curr_tok.kind));
    return result;
  }

  // skip the `]`
  err = parser_next(parser);
  if (err != NULL) {
    result.error = err;
    return result;
  }

  return result;
}
