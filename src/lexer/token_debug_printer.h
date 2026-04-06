#ifndef F77INTERPRET_LEXER_TOKEN_DEBUG_PRINTER_H
#define F77INTERPRET_LEXER_TOKEN_DEBUG_PRINTER_H

#include <ostream>
#include <token.h>

namespace f77i {

struct TokenDebugPrinter {
  s::ostream& print(s::ostream& out, const Token& token) const;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_TOKEN_DEBUG_PRINTER_H
