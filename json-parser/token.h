#pragma once

typedef enum {
  TOKENKIND_LBRACE,
  TOKENKIND_RBRACE,
  TOKENKIND_LBRACKET,
  TOKENKIND_RBBRACKET,
  TOKENKIND_STRING,
  TOKENKIND_EOF,
} TokenKind;

typedef struct {
  TokenKind kind;
  char *value;
} Token;

char *display_tokenkind(TokenKind kind);
Token token_new_eof();
