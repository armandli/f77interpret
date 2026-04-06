#include <source_file.h>

#include <fstream>
#include <sstream>
#include <lexer.h>

namespace f77i {

SourceFile::SourceFile(s::filesystem::path path, const Config& conf)
    : mConf(conf), mPath(s::move(path)), mContent(), mTokens() {
  s::ifstream file(mPath);
  s::ostringstream ss;
  ss << file.rdbuf();
  mContent = ss.str();
  tokenize();
}

SourceFile::SourceFile(const SourceFile& other)
    : mConf(other.mConf), mPath(other.mPath), mContent(other.mContent), mTokens() {
  tokenize();
}

void SourceFile::tokenize() {
  Lexer lexer(mConf);
  mTokens = lexer.tokenize(mContent);
  lexer.append_eof(mTokens);
}

} // namespace f77i
