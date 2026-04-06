#ifndef F77INTERPRET_CONFIG_H
#define F77INTERPRET_CONFIG_H

#include <string>
#include <vector>
#include <alias.h>

namespace f77i {

enum class Codepoint : int { ASCII, UTF8, CODEPAGE, };

struct Config {
  s::string source_file;
  s::vector<s::string> include_paths;
  Codepoint codepoint = Codepoint::ASCII;
  int SCSrcCol = 6;   // 0-indexed start of code field (column 7 in 1-indexed)
  int SCMaxCol = 71;  // 0-indexed last valid code column inclusive (column 72 in 1-indexed)
  bool debug_tokenizer = false;
};

} // namespace f77i

#endif // F77INTERPRET_CONFIG_H
