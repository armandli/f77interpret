#ifndef F77INTERPRET_ALIAS_H
#define F77INTERPRET_ALIAS_H

#include <unordered_map>
#include <unordered_set>

namespace s = std;

template <typename K, typename V>
using umap = s::unordered_map<K, V>;

template <typename T>
using uset = s::unordered_set<T>;

#endif // F77INTERPRET_ALIAS_H
