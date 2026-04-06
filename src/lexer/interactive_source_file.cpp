#include <interactive_source_file.h>

#include <lexer.h>

namespace f77i {

InteractiveSourceFile::InteractiveSourceFile(const Config& conf)
    : SourceBase(conf), mLines(), mNextLno(0), mContentCache(), mContentDirty(true) {}

void InteractiveSourceFile::append(s::string_view text) {
  s::string block(text);
  if (block.empty() || block.back() != '\n')
    block += '\n';
  mLines.push_back(s::move(block));
  const s::string& stable_ref = mLines.back();
  Lexer lexer(mConf);
  s::vector<Token> new_tokens = lexer.tokenize(stable_ref, mNextLno);
  mNextLno += lexer.count_lines(stable_ref);
  for (auto& tok : new_tokens)
    mTokens.push_back(tok);
  mContentDirty = true;
}

const s::string& InteractiveSourceFile::content() const {
  if (mContentDirty) {
    mContentCache.clear();
    for (const auto& line : mLines)
      mContentCache += line;
    mContentDirty = false;
  }
  return mContentCache;
}

} // namespace f77i
