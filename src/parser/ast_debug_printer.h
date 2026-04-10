#ifndef F77INTERPRET_PARSER_AST_DEBUG_PRINTER_H
#define F77INTERPRET_PARSER_AST_DEBUG_PRINTER_H

#include <ostream>
#include <ast.h>

namespace f77i {

struct ASTPrinter {
  s::ostream& print(s::ostream& out, const ASTNode* root) const;
  s::ostream& print(s::ostream& out, const CmpStmt& stmt) const;

protected:
  s::ostream& indent(s::ostream& out, int depth) const;
  s::ostream& print_node(s::ostream& out, const ASTNode* node, int depth) const;
  s::ostream& print_cmpstmt(s::ostream& out, const CmpStmt& stmt, int depth) const;

  s::ostream& print_sub(s::ostream& out, const Sub& node, int depth) const;
  s::ostream& print_prog(s::ostream& out, const Prog& node, int depth) const;
  s::ostream& print_if(s::ostream& out, const If& node, int depth) const;
  s::ostream& print_do(s::ostream& out, const Do& node, int depth) const;
  s::ostream& print_function(s::ostream& out, const Function& node, int depth) const;
  s::ostream& print_assign(s::ostream& out, const Assign& node, int depth) const;
  s::ostream& print_binaryop(s::ostream& out, const BinaryOp& node, int depth) const;
  s::ostream& print_unaryop(s::ostream& out, const UnaryOp& node, int depth) const;
  s::ostream& print_var(s::ostream& out, const Var& node, int depth) const;
  s::ostream& print_star(s::ostream& out, const Star& node, int depth) const;
  s::ostream& print_implicit(s::ostream& out, const Implicit& node, int depth) const;
  s::ostream& print_goto(s::ostream& out, const Goto& node, int depth) const;
  s::ostream& print_format(s::ostream& out, const Format& node, int depth) const;
  s::ostream& print_data(s::ostream& out, const Data& node, int depth) const;
  s::ostream& print_common(s::ostream& out, const Common& node, int depth) const;
  s::ostream& print_external(s::ostream& out, const External& node, int depth) const;
  s::ostream& print_dimension(s::ostream& out, const Dimension& node, int depth) const;
  s::ostream& print_stop(s::ostream& out, const Stop& node, int depth) const;
  s::ostream& print_pause(s::ostream& out, const Pause& node, int depth) const;
  s::ostream& print_entry(s::ostream& out, const Entry& node, int depth) const;
  s::ostream& print_equivalence(s::ostream& out, const Equivalence& node, int depth) const;
  s::ostream& print_intrinsic(s::ostream& out, const Intrinsic& node, int depth) const;
  s::ostream& print_param(s::ostream& out, const Param& node, int depth) const;
  s::ostream& print_save(s::ostream& out, const Save& node, int depth) const;
  s::ostream& print_close(s::ostream& out, const Close& node, int depth) const;
  s::ostream& print_open(s::ostream& out, const Open& node, int depth) const;
  s::ostream& print_read(s::ostream& out, const Read& node, int depth) const;
  s::ostream& print_write(s::ostream& out, const Write& node, int depth) const;
  s::ostream& print_print(s::ostream& out, const Print& node, int depth) const;
  s::ostream& print_continue(s::ostream& out, const Continue& node, int depth) const;
  s::ostream& print_return(s::ostream& out, const Return& node, int depth) const;
  s::ostream& print_call(s::ostream& out, const Call& node, int depth) const;
  s::ostream& print_lit(s::ostream& out, const Lit& node, int depth) const;
  s::ostream& print_indexing(s::ostream& out, const Indexing& node, int depth) const;
  s::ostream& print_decl(s::ostream& out, const Decl& node, int depth) const;
  s::ostream& print_indexlist(s::ostream& out, const IndexList& node, int depth) const;

  const char* ty_name(TY ty) const;
  const char* bop_name(BOP op) const;
  const char* uop_name(UOP op) const;
};

} // namespace f77i

#endif // F77INTERPRET_PARSER_AST_DEBUG_PRINTER_H
