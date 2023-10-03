#include "./lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void lexer_advance(Lexer *lexer) {
  lexer->curr_char = lexer->input[lexer->curr_loc.index]; // TODO: is this OK?
  lexer->curr_loc.index++;
}

Lexer lexer_new(char *input) {
  Location loc = {.index = -1};
  Lexer lexer = {.input = input, .curr_loc = loc, .curr_char = '0'};
  lexer_advance(&lexer);
  return lexer;
}

TokenResult lexer_make_string(Lexer *lexer) {
  Token string_tok = {.kind = TOKENKIND_STRING, .value = NULL};
  TokenResult result = {.error = NULL, .token = string_tok};
  // skip opening quote
  lexer_advance(lexer);

  ssize_t string_inner_start_idx = lexer->curr_loc.index;

  while (lexer->curr_char != '"' && lexer->curr_char != '\0') {
    lexer_advance(lexer);
  }

  // skip closing quote
  if (lexer->curr_char != '"') {
    asprintf(&result.error, "Error: missing closing quote: expected `\"`, found `%c`", lexer->curr_char);
    return result;
  }
  lexer_advance(lexer);

  ssize_t inner_str_len = lexer->curr_loc.index - string_inner_start_idx;
  malloc(sizeof(char) * inner_str_len + 1);
  memcpy(result.token.value, lexer->input, inner_str_len);
  result.token.value[inner_str_len - 1] = '\0';

  return result;
}

TokenResult lexer_next_token(Lexer *lexer) {
  TokenResult result = {.error = NULL, .token = token_new_eof()};

  // EoF is reached, stop here
  if (lexer->curr_loc.index >= strlen(lexer->input)) {
    return result;
  }

  switch (lexer->curr_char) {
  case '{':
    result.token.kind = TOKENKIND_LBRACE;
    result.token.value = "{";
    return result;
  case '}':
    result.token.kind = TOKENKIND_LBRACE;
    result.token.value = "{";
    return result;
  case '"':
    return lexer_make_string(lexer);
  default:
    asprintf(&result.error, "Error: illegal character `%c`", lexer->curr_char);
    return result;
  }
}
