#ifndef F77INTERPRET_EXCEPTION_H
#define F77INTERPRET_EXCEPTION_H

#include <exception>
#include <string>
#include <alias.h>

namespace f77i {

struct undefined_name : s::exception {
  explicit undefined_name(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
  s::string msg;
};

struct type_error : s::exception {
  explicit type_error(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
  s::string msg;
};

struct syntax_error : s::exception {
  explicit syntax_error(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
  s::string msg;
};

struct file_not_found : s::exception {
  explicit file_not_found(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
  s::string msg;
};

struct oob_error : s::exception {
  explicit oob_error(s::string m) : msg(s::move(m)) {}
  const char* what() const noexcept override { return msg.c_str(); }
  s::string msg;
};

} // namespace f77i

#endif // F77INTERPRET_EXCEPTION_H
