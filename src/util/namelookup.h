#ifndef F77INTERPRET_NAMELOOKUP_H
#define F77INTERPRET_NAMELOOKUP_H

#include <algorithm>
#include <cctype>
#include <optional>
#include <string>
#include <string_view>
#include <alias.h>

namespace f77i {

template <typename V>
inline s::optional<V> namelookup(
    s::string_view name, const umap<s::string, V>& source)
{
  s::string lower(name);
  s::transform(lower.begin(), lower.end(), lower.begin(),
               [](unsigned char c) { return s::tolower(c); });
  auto it = source.find(lower);
  if (it != source.end()) return it->second;
  return s::nullopt;
}

inline bool namelookup(s::string_view name, const uset<s::string>& source) {
  s::string lower(name);
  s::transform(lower.begin(), lower.end(), lower.begin(),
               [](unsigned char c) { return s::tolower(c); });
  return source.count(lower) > 0;
}

} // namespace f77i

#endif // F77INTERPRET_NAMELOOKUP_H
