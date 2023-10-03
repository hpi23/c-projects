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
  case TOKENKIND_RBRACE:
  case TOKENKIND_LBRACKET:
  case TOKENKIND_RBRACKET:
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
  case TOKENKIND_FLOAT:
  case TOKENKIND_STRING:
  case TOKENKIND_COLON:
  case TOKENKIND_COMMA:
  case TOKENKIND_EOF:
  default:
    asprintf(&result.error, "Error: exected either `LBRACE` or `LBRACKET`, got `%s`", display_tokenkind(parser->curr_tok.kind));
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

  HashMap * field_map = hashmap_new();

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
