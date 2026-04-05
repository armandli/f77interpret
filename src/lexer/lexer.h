#ifndef F77INTERPRET_LEXER_LEXER_H
#define F77INTERPRET_LEXER_LEXER_H

#include <string>
#include <string_view>
#include <vector>
#include <cstddef>
#include <alias.h>
#include <config.h>
#include <token.h>

namespace f77i {

class Lexer {
  Config mConf;

  void tokenizeCode(s::string_view code, int lno, int llno, s::vector<Token>& tokens) const;
  s::size_t tryParseComplexLiteral(s::string_view code, s::size_t i) const;
  s::size_t tryParseSignedNumber(s::string_view code, s::size_t i) const;
  TT parseDotToken(s::string_view code, s::size_t i, s::size_t& out_end) const;

public:
  explicit Lexer(const Config& conf);
  s::vector<Token> tokenize(const s::string& source_code, int start_lno = 0);
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_LEXER_H
