#include "./lexer.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static inline bool lexer_char_is_ascii_digit(char input) { return input >= 0x30 && input <= 0x39; }

void lexer_advance(Lexer *lexer) {
  lexer->curr_char = lexer->input[lexer->curr_loc.index]; // TODO: is this OK?
  lexer->curr_loc.index++;
}

Lexer lexer_new(char *input) {
  Location loc = {.index = 0};
  Lexer lexer = {.input = input, .curr_loc = loc, .curr_char = '0'};
  lexer_advance(&lexer);
  return lexer;
}

void lexer_free(Lexer *lexer) {
    free(lexer->input);
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
  result.token.value = malloc(sizeof(char) * inner_str_len + 1);
  memcpy(result.token.value, &lexer->input[string_inner_start_idx - 1], inner_str_len);
  result.token.value[inner_str_len - 1] = '\0';

  return result;
}

TokenResult lexer_make_number(Lexer *lexer) {
  Token num_tok = {.kind = TOKENKIND_INT, .value = NULL};
  TokenResult result = {.error = NULL, .token = num_tok};

  ssize_t num_start_idx = lexer->curr_loc.index;

  bool is_float = false;

  ssize_t prev_idx = 0;

  while (lexer_char_is_ascii_digit(lexer->curr_char)) {
    prev_idx = lexer->curr_loc.index;
    lexer_advance(lexer);
  }

  if (lexer->curr_char == '.') {
    is_float = true;
    lexer_advance(lexer);

    while (lexer_char_is_ascii_digit(lexer->curr_char) && lexer->curr_char != '\0') {
      prev_idx = lexer->curr_loc.index;
      lexer_advance(lexer);
    }
  }

  ssize_t num_str_len = prev_idx - num_start_idx + 1;
  result.token.value = malloc(sizeof(char) * num_str_len + 1);
  memcpy(result.token.value, &lexer->input[num_start_idx - 1], num_str_len);
  result.token.value[num_str_len] = '\0';

  if (is_float) {
    result.token.kind = TOKENKIND_FLOAT;
  }

  return result;
}

TokenResult lexer_next_token(Lexer *lexer) {
  TokenResult result = {.error = NULL, .token = token_new_eof()};

  // EoF is reached, stop here
  if (lexer->curr_loc.index >= strlen(lexer->input)) {
    return result;
  }

  while (lexer->curr_char == ' ' || lexer->curr_char == '\n' || lexer->curr_char == '\t') {
    lexer_advance(lexer);
  }

  switch (lexer->curr_char) {
  case '{':
    result.token.kind = TOKENKIND_LBRACE;
    result.token.value = "{";
    break;
  case '}':
    result.token.kind = TOKENKIND_RBRACE;
    result.token.value = "}";
    break;
  case '[':
    result.token.kind = TOKENKIND_LBRACKET;
    result.token.value = "[";
    break;
  case ']':
    result.token.kind = TOKENKIND_RBRACKET;
    result.token.value = "]";
    break;
  case ':':
    result.token.kind = TOKENKIND_COLON;
    result.token.value = ":";
    break;
  case ',':
    result.token.kind = TOKENKIND_COMMA;
    result.token.value = ",";
    break;
  case '"':
    return lexer_make_string(lexer);
  case '\0':
    result.token.kind = TOKENKIND_EOF;
    result.token.value = "EOF";
    break;
  default:
    if (lexer_char_is_ascii_digit(lexer->curr_char)) {
      return lexer_make_number(lexer);
    }

    asprintf(&result.error, "Error: illegal character `%c`", lexer->curr_char);
    return result;
  }

  lexer_advance(lexer);
  return result;
}
