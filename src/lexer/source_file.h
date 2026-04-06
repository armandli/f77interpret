#ifndef F77INTERPRET_LEXER_SOURCE_FILE_H
#define F77INTERPRET_LEXER_SOURCE_FILE_H

#include <filesystem>
#include <string>
#include <vector>
#include <alias.h>
#include <config.h>
#include <token.h>

namespace f77i {

struct SourceFile {
  SourceFile(s::filesystem::path path, const Config& conf);
  SourceFile(const SourceFile& other);

  s::filesystem::path filename() const { return mPath; }
  const s::string& content() const { return mContent; }
  const s::vector<Token>& tokens() const { return mTokens; }

protected:
  void tokenize();

private:
  const Config& mConf;
  s::filesystem::path mPath;
  s::string mContent;
  s::vector<Token> mTokens;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_SOURCE_FILE_H
