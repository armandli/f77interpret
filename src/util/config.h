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
};

} // namespace f77i

#endif // F77INTERPRET_CONFIG_H
