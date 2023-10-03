#pragma once
#include <stdlib.h>
#include "./token.h"

typedef struct {
  ssize_t index;
} Location;

typedef struct {
  char *input;
  Location curr_loc;
  char curr_char;
} Lexer;

typedef struct {
  Token token;
  char *error;
} TokenResult;

void lexer_advance(Lexer *lexer);
Lexer lexer_new(char *input);
TokenResult lexer_next_token(Lexer *lexer);
