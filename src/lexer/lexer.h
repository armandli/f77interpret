#ifndef F77INTERPRET_LEXER_LEXER_H
#define F77INTERPRET_LEXER_LEXER_H

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include <alias.h>
#include <config.h>
#include <token.h>

namespace f77i {

struct Lexer {
  explicit Lexer(const Config& conf) : mConf(conf) {}
  s::vector<Token> tokenize(s::string_view source_code, int start_lno = 0);

protected:
  void tokenizeCode(
      s::string_view code,
      int lno,
      int llno,
      s::vector<Token>& tokens) const;
  s::size_t tryParseComplexLiteral(
      s::string_view code, s::size_t i) const;
  s::size_t tryParseSignedNumber(
      s::string_view code, s::size_t i) const;
  TT parseDotToken(
      s::string_view code, s::size_t i, s::size_t& out_end) const;

private:
  Config mConf;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_LEXER_H
