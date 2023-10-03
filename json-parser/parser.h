#include "./json.h"

typedef struct {
  char *error;
  JsonValueObject value;
} ParseResultObject;

typedef struct {
    Token curr_tok;
    Lexer lexer;
} Parser;


char * parser_next(Parser * parser);
ParseResult parse_json(Parser *, char *input);
ParseResultObject parse_object(Parser *parser);
ParseResult parse_json(Parser *, char *input);
