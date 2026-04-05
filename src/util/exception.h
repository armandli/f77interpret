#ifndef F77INTERPRET_EXCEPTION_H
#define F77INTERPRET_EXCEPTION_H

#include <exception>
#include <string>
#include <alias.h>

namespace f77i {

struct undefined_name : s::exception {
  s::string msg;
  explicit undefined_name(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
};

struct type_error : s::exception {
  s::string msg;
  explicit type_error(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
};

struct syntax_error : s::exception {
  s::string msg;
  explicit syntax_error(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
};

struct file_not_found : s::exception {
  s::string msg;
  explicit file_not_found(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
};

} // namespace f77i

#endif // F77INTERPRET_EXCEPTION_H
