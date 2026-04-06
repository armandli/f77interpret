#ifndef F77INTERPRET_LEXER_SOURCE_BASE_H
#define F77INTERPRET_LEXER_SOURCE_BASE_H

#include <string>
#include <vector>
#include <alias.h>
#include <config.h>
#include <token.h>

namespace f77i {

struct SourceBase {
  explicit SourceBase(const Config& conf) : mConf(conf), mTokens() {}
  virtual ~SourceBase() = default;
  SourceBase(const SourceBase&)            = delete;
  SourceBase& operator=(const SourceBase&) = delete;

  const s::vector<Token>& tokens() const { return mTokens; }
  virtual const s::string& content() const = 0;

protected:
  const Config&    mConf;
  s::vector<Token> mTokens;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_SOURCE_BASE_H
