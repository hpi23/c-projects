#include "./token.h"

char *display_tokenkind(TokenKind kind) {
  switch (kind) {
  case TOKENKIND_LBRACE:
    return "LBRACE";
  case TOKENKIND_RBRACE:
    return "RBRACE";
  case TOKENKIND_EOF:
    return "EOF";
  }
}


Token token_new_eof() {
  Token token = {.value = "EOF", .kind = TOKENKIND_EOF};
  return token;
}
