#ifndef F77INTERPRET_LEXER_SOURCE_FILE_H
#define F77INTERPRET_LEXER_SOURCE_FILE_H

#include <filesystem>
#include <string>
#include <alias.h>
#include <config.h>
#include <source_base.h>

namespace f77i {

struct SourceFile : SourceBase {
  SourceFile(s::filesystem::path path, const Config& conf);
  SourceFile(const SourceFile& other);

  s::filesystem::path filename() const { return mPath; }
  const s::string& content() const override { return mContent; }

private:
  void tokenize();
  s::filesystem::path mPath;
  s::string           mContent;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_SOURCE_FILE_H
