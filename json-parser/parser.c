#include "./parser.h"
#include "./lexer.h"
#include "./token.h"
#include <stdio.h>

char *parser_next(Parser *parser) {
  TokenResult result = lexer_next_token(&parser->lexer);
  parser->curr_tok = result.token;
  if (result.error != NULL) {
    return result.error;
  }
  return NULL;
}

ParseResult parse_json(Parser *, char *input) {
  ParseResult result = {.error = NULL};

  Lexer lexer = lexer_new(input);
  TokenResult curr = lexer_next_token(&lexer);
  if (curr.error != NULL) {
    result.error = curr.error;
    return result;
  }

  switch (curr.token.kind) {
  case TOKENKIND_LBRACE:
    // OBJECT
    break;
  case TOKENKIND_LBRACKET:
    // ARRAY
    break;
  default:
    asprintf(&result.error, "Error: exected either `LBRACE` or `LBRACKET`, got `%s`", display_tokenkind(curr.token.kind));
    return result;
  }
};

ParseResultObject parse_object(Parser *parser) {
  ParseResultObject result = {.error = NULL};

  // Skip the `{`
  parser_next(parser);

  if (parser->curr_tok.kind != TOKENKIND_RBRACE && parser->curr_tok.kind != TOKENKIND_EOF) {
    TokenResult tok_res = lexer_next_token(&parser->lexer);
    if (tok_res.error != NULL) {
      result.error = tok_res.error;
      return result;
    }

    // make a key-value pair
  }

  if (parser->curr_tok.kind != TOKENKIND_RBRACE) {
    asprintf(&result.error, "Error: exected token `%s`, found `%s`", display_tokenkind(TOKENKIND_RBRACE), display_tokenkind(parser->curr_tok.kind));
    return result;
  }

  JsonValueObject object = {}; // TODO: fill this
  return result;
}
