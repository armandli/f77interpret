#ifndef F77INTERPRET_LEXER_INTERACTIVE_SOURCE_FILE_H
#define F77INTERPRET_LEXER_INTERACTIVE_SOURCE_FILE_H

#include <deque>
#include <string>
#include <alias.h>
#include <source_base.h>

namespace f77i {

struct InteractiveSourceFile : SourceBase {
  explicit InteractiveSourceFile(const Config& conf);

  InteractiveSourceFile(const InteractiveSourceFile&)            = delete;
  InteractiveSourceFile& operator=(const InteractiveSourceFile&) = delete;

  void append(s::string_view text);
  int lineCount() const { return mNextLno; }
  const s::string& content() const override;

private:
  s::deque<s::string> mLines;
  int                 mNextLno = 0;
  mutable s::string   mContentCache;
  mutable bool        mContentDirty = true;
};

} // namespace f77i

#endif // F77INTERPRET_LEXER_INTERACTIVE_SOURCE_FILE_H
