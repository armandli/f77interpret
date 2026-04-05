#ifndef F77INTERPRET_LEXER_TOKEN_H
#define F77INTERPRET_LEXER_TOKEN_H

#include <string_view>
#include <alias.h>

namespace f77i {

enum class TT : int {
  // keywords
  INTEGER = 1,
  REAL,
  DOUBLE,
  COMPLEX,
  LOGICAL,
  CHARACTER,
  IF,
  THEN,
  ELSE,
  ELSEIF,
  ENDIF,
  DO,
  ENDDO,
  CONTINUE,
  GOTO,
  STOP,
  PAUSE,
  END,
  PROGRAM,
  SUBROUTINE,
  FUNCTION,
  BLOCK,
  DATA,
  READ,
  WRITE,
  PRINT,
  TYPE,
  OPEN,
  CLOSE,
  REWIND,
  BACKSPACE,
  ENDFILE,
  FORMAT,
  COMMON,
  EQUIVALENCE,
  EXTERNAL,
  INTRINSIC,
  SAVE,
  PARAMETER,
  IMPLICIT,
  DIMENSION,
  NONE,
  ENTRY,
  INCLUDE,
  CALL,
  RETURN,

  // operators
  PLUS,
  MINUS,
  STAR,
  SLASH,
  DSTAR,
  DSLASH,

  // assignment
  ASSIGN,

  // relational operators
  EQ,
  NE,
  LT,
  LE,
  GT,
  GE,

  // logical operators
  AND,
  OR,
  NOT,
  EQV,
  NEQV,

  // logical literals
  TRUE,
  FALSE,

  // punctuation
  LPAREN,
  RPAREN,
  COMMA,
  COLON,
  SEMICOLON,
  SQUOTE,
  DQUOTE,

  // literals
  INT_LITERAL,
  DBL_LITERAL,
  RL_LITERAL,
  CMPX_LITERAL,
  CHR_LITERAL,

  // structural
  IDENTIFIER,
  NEWLINE,
  END_OF_FILE,
  UNKNOWN,
};

struct Token {
  Token() : type(TT::UNKNOWN), lno(0), llno(0), sv() {}
  Token(TT type, int lno, int llno, s::string_view sv)
      : type(type), lno(lno), llno(llno), sv(sv) {}
  TT             type;
  int            lno;   // physical line number
  int            llno;  // effective (logical) line number
  s::string_view sv;    // view into original source string
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_TOKEN_H
