#ifndef F77INTERPRET_LEXER_TOKEN_DEBUG_PRINTER_H
#define F77INTERPRET_LEXER_TOKEN_DEBUG_PRINTER_H

#include <token.h>

namespace f77i {

struct TokenDebugPrinter {
  void print(const Token& token) const;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_TOKEN_DEBUG_PRINTER_H
