#include <ast_debug_printer.h>

namespace f77i {

namespace {

constexpr int kIndentWidth = 2;

} // namespace

s::ostream& ASTPrinter::indent(s::ostream& out, int depth) const {
  for (int i = 0; i < depth * kIndentWidth; ++i)
    out << ' ';
  return out;
}

const char* ASTPrinter::ty_name(TY ty) const {
  switch (ty) {
    case TY::INTEGER:   return "INTEGER";
    case TY::REAL:      return "REAL";
    case TY::DOUBLE:    return "DOUBLE";
    case TY::COMPLEX:   return "COMPLEX";
    case TY::CHARACTER: return "CHARACTER";
    case TY::LOGICAL:   return "LOGICAL";
    case TY::VOID:      return "VOID";
    case TY::UNKNOWN:   return "UNKNOWN";
  }
  return "UNKNOWN";
}

const char* ASTPrinter::bop_name(BOP op) const {
  switch (op) {
    case BOP::ADD:  return "ADD";
    case BOP::SUB:  return "SUB";
    case BOP::MUL:  return "MUL";
    case BOP::DIV:  return "DIV";
    case BOP::POW:  return "POW";
    case BOP::EQ:   return "EQ";
    case BOP::NE:   return "NE";
    case BOP::GT:   return "GT";
    case BOP::GE:   return "GE";
    case BOP::LT:   return "LT";
    case BOP::LE:   return "LE";
    case BOP::AND:  return "AND";
    case BOP::OR:   return "OR";
    case BOP::NOT:  return "NOT";
    case BOP::EQV:  return "EQV";
    case BOP::NEQV: return "NEQV";
    case BOP::CAT:  return "CAT";
    case BOP::UNKNOWN: return "UNKNOWN";
  }
  return "UNKNOWN";
}

const char* ASTPrinter::uop_name(UOP op) const {
  switch (op) {
    case UOP::NEG:     return "NEG";
    case UOP::POS:     return "POS";
    case UOP::NOT:     return "NOT";
    case UOP::UNKNOWN: return "UNKNOWN";
  }
  return "UNKNOWN";
}

s::ostream& ASTPrinter::print_node(s::ostream& out, const ASTNode* node, int depth) const {
  if (node == nullptr) {
    indent(out, depth) << "<null>\n";
    return out;
  }
  if (const auto* n = dynamic_cast<const Prog*>(node))
    return print_prog(out, *n, depth);
  if (const auto* n = dynamic_cast<const Sub*>(node))
    return print_sub(out, *n, depth);
  if (const auto* n = dynamic_cast<const Function*>(node))
    return print_function(out, *n, depth);
  if (const auto* n = dynamic_cast<const If*>(node))
    return print_if(out, *n, depth);
  if (const auto* n = dynamic_cast<const Do*>(node))
    return print_do(out, *n, depth);
  if (const auto* n = dynamic_cast<const Assign*>(node))
    return print_assign(out, *n, depth);
  if (const auto* n = dynamic_cast<const BinaryOp*>(node))
    return print_binaryop(out, *n, depth);
  if (const auto* n = dynamic_cast<const UnaryOp*>(node))
    return print_unaryop(out, *n, depth);
  if (const auto* n = dynamic_cast<const Var*>(node))
    return print_var(out, *n, depth);
  if (const auto* n = dynamic_cast<const Star*>(node))
    return print_star(out, *n, depth);
  if (const auto* n = dynamic_cast<const Implicit*>(node))
    return print_implicit(out, *n, depth);
  if (const auto* n = dynamic_cast<const Goto*>(node))
    return print_goto(out, *n, depth);
  if (const auto* n = dynamic_cast<const Format*>(node))
    return print_format(out, *n, depth);
  if (const auto* n = dynamic_cast<const Data*>(node))
    return print_data(out, *n, depth);
  if (const auto* n = dynamic_cast<const Common*>(node))
    return print_common(out, *n, depth);
  if (const auto* n = dynamic_cast<const External*>(node))
    return print_external(out, *n, depth);
  if (const auto* n = dynamic_cast<const Dimension*>(node))
    return print_dimension(out, *n, depth);
  if (const auto* n = dynamic_cast<const Stop*>(node))
    return print_stop(out, *n, depth);
  if (const auto* n = dynamic_cast<const Pause*>(node))
    return print_pause(out, *n, depth);
  if (const auto* n = dynamic_cast<const Entry*>(node))
    return print_entry(out, *n, depth);
  if (const auto* n = dynamic_cast<const Equivalence*>(node))
    return print_equivalence(out, *n, depth);
  if (const auto* n = dynamic_cast<const Intrinsic*>(node))
    return print_intrinsic(out, *n, depth);
  if (const auto* n = dynamic_cast<const Param*>(node))
    return print_param(out, *n, depth);
  if (const auto* n = dynamic_cast<const Save*>(node))
    return print_save(out, *n, depth);
  if (const auto* n = dynamic_cast<const Close*>(node))
    return print_close(out, *n, depth);
  if (const auto* n = dynamic_cast<const Open*>(node))
    return print_open(out, *n, depth);
  if (const auto* n = dynamic_cast<const Read*>(node))
    return print_read(out, *n, depth);
  if (const auto* n = dynamic_cast<const Write*>(node))
    return print_write(out, *n, depth);
  if (const auto* n = dynamic_cast<const Print*>(node))
    return print_print(out, *n, depth);
  if (const auto* n = dynamic_cast<const Continue*>(node))
    return print_continue(out, *n, depth);
  if (const auto* n = dynamic_cast<const Return*>(node))
    return print_return(out, *n, depth);
  if (const auto* n = dynamic_cast<const Call*>(node))
    return print_call(out, *n, depth);
  if (const auto* n = dynamic_cast<const Lit*>(node))
    return print_lit(out, *n, depth);
  if (const auto* n = dynamic_cast<const Indexing*>(node))
    return print_indexing(out, *n, depth);
  if (const auto* n = dynamic_cast<const Decl*>(node))
    return print_decl(out, *n, depth);
  if (const auto* n = dynamic_cast<const IndexList*>(node))
    return print_indexlist(out, *n, depth);
  indent(out, depth) << "<unknown ASTNode>\n";
  return out;
}

s::ostream& ASTPrinter::print_cmpstmt(s::ostream& out, const CmpStmt& stmt, int depth) const {
  indent(out, depth) << "CmpStmt [\n";
  for (const ASTNode* node : stmt.stmts)
    print_node(out, node, depth + 1);
  indent(out, depth) << "]\n";
  return out;
}

s::ostream& ASTPrinter::print_prog(s::ostream& out, const Prog& node, int depth) const {
  indent(out, depth) << "Prog { name=\"" << node.name << "\"";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  print_cmpstmt(out, node.body, depth + 1);
  return out;
}

s::ostream& ASTPrinter::print_sub(s::ostream& out, const Sub& node, int depth) const {
  indent(out, depth) << "Sub { name=\"" << node.name << "\"";
  if (!node.parameters.empty()) {
    out << ", params=[";
    for (size_t i = 0; i < node.parameters.size(); ++i) {
      if (i > 0) out << ", ";
      out << node.parameters[i];
    }
    out << "]";
  }
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  print_cmpstmt(out, node.body, depth + 1);
  return out;
}

s::ostream& ASTPrinter::print_function(s::ostream& out, const Function& node, int depth) const {
  indent(out, depth) << "Function { name=\"" << node.name << "\"";
  if (!node.parameters.empty()) {
    out << ", params=[";
    for (size_t i = 0; i < node.parameters.size(); ++i) {
      if (i > 0) out << ", ";
      out << node.parameters[i];
    }
    out << "]";
  }
  out << ", return_type=" << ty_name(node.return_type.type);
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  print_cmpstmt(out, node.body, depth + 1);
  return out;
}

s::ostream& ASTPrinter::print_if(s::ostream& out, const If& node, int depth) const {
  indent(out, depth) << "If";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  indent(out, depth + 1) << "condition:\n";
  print_node(out, node.condition, depth + 2);
  indent(out, depth + 1) << "body:\n";
  print_cmpstmt(out, node.body, depth + 2);
  if (node.else_block != nullptr) {
    indent(out, depth + 1) << "else:\n";
    print_node(out, node.else_block, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_do(s::ostream& out, const Do& node, int depth) const {
  indent(out, depth) << "Do";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  indent(out, depth + 1) << "init:\n";
  print_node(out, node.init, depth + 2);
  indent(out, depth + 1) << "fin:\n";
  print_node(out, node.fin, depth + 2);
  if (node.step != nullptr) {
    indent(out, depth + 1) << "step:\n";
    print_node(out, node.step, depth + 2);
  }
  indent(out, depth + 1) << "body:\n";
  print_cmpstmt(out, node.body, depth + 2);
  return out;
}

s::ostream& ASTPrinter::print_assign(s::ostream& out, const Assign& node, int depth) const {
  indent(out, depth) << "Assign";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  indent(out, depth + 1) << "variable:\n";
  print_node(out, node.variable, depth + 2);
  indent(out, depth + 1) << "expr:\n";
  print_node(out, node.expr, depth + 2);
  return out;
}

s::ostream& ASTPrinter::print_binaryop(s::ostream& out, const BinaryOp& node, int depth) const {
  indent(out, depth) << "BinaryOp { op=" << bop_name(node.op)
                     << ", rtype=" << ty_name(node.rtype.type);
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  indent(out, depth + 1) << "left:\n";
  print_node(out, node.left, depth + 2);
  indent(out, depth + 1) << "right:\n";
  print_node(out, node.right, depth + 2);
  return out;
}

s::ostream& ASTPrinter::print_unaryop(s::ostream& out, const UnaryOp& node, int depth) const {
  indent(out, depth) << "UnaryOp { op=" << uop_name(node.op)
                     << ", rtype=" << ty_name(node.rtype.type);
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  indent(out, depth + 1) << "expr:\n";
  print_node(out, node.expr, depth + 2);
  return out;
}

s::ostream& ASTPrinter::print_var(s::ostream& out, const Var& node, int depth) const {
  indent(out, depth) << "Var { name=\"" << node.name
                     << "\", type=" << ty_name(node.rtype.type);
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_star(s::ostream& out, const Star& node, int depth) const {
  indent(out, depth) << "Star";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  return out;
}

s::ostream& ASTPrinter::print_implicit(s::ostream& out, const Implicit& node, int depth) const {
  indent(out, depth) << "Implicit";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  return out;
}

s::ostream& ASTPrinter::print_goto(s::ostream& out, const Goto& node, int depth) const {
  indent(out, depth) << "Goto { labels=[";
  for (size_t i = 0; i < node.labels.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.labels[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  if (node.var != nullptr) {
    indent(out, depth + 1) << "var:\n";
    print_node(out, node.var, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_format(s::ostream& out, const Format& node, int depth) const {
  indent(out, depth) << "Format { specification=[";
  for (size_t i = 0; i < node.specification.size(); ++i) {
    if (i > 0) out << ", ";
    out << "\"" << node.specification[i] << "\"";
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_data(s::ostream& out, const Data& node, int depth) const {
  indent(out, depth) << "Data { variables=[";
  for (size_t i = 0; i < node.variables.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.variables[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  if (!node.values.empty()) {
    indent(out, depth + 1) << "values:\n";
    for (const ASTNode* val : node.values)
      print_node(out, val, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_common(s::ostream& out, const Common& node, int depth) const {
  indent(out, depth) << "Common { name=\"" << node.name << "\", variables=[";
  for (size_t i = 0; i < node.variables.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.variables[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_external(s::ostream& out, const External& node, int depth) const {
  indent(out, depth) << "External { procedures=[";
  for (size_t i = 0; i < node.procedures.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.procedures[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_dimension(s::ostream& out, const Dimension& node, int depth) const {
  indent(out, depth) << "Dimension { variable=\"" << node.variable << "\"";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  if (!node.dimensions.empty()) {
    indent(out, depth + 1) << "dimensions:\n";
    for (const ASTNode* dim : node.dimensions)
      print_node(out, dim, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_stop(s::ostream& out, const Stop& node, int depth) const {
  indent(out, depth) << "Stop";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.message != nullptr) {
    indent(out, depth + 1) << "message:\n";
    print_node(out, node.message, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_pause(s::ostream& out, const Pause& node, int depth) const {
  indent(out, depth) << "Pause";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.message != nullptr) {
    indent(out, depth + 1) << "message:\n";
    print_node(out, node.message, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_entry(s::ostream& out, const Entry& node, int depth) const {
  indent(out, depth) << "Entry { name=\"" << node.name << "\"";
  if (!node.parameters.empty()) {
    out << ", params=[";
    for (size_t i = 0; i < node.parameters.size(); ++i) {
      if (i > 0) out << ", ";
      out << node.parameters[i];
    }
    out << "]";
  }
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_equivalence(s::ostream& out, const Equivalence& node, int depth) const {
  indent(out, depth) << "Equivalence { group=[";
  for (size_t i = 0; i < node.group.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.group[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_intrinsic(s::ostream& out, const Intrinsic& node, int depth) const {
  indent(out, depth) << "Intrinsic { functions=[";
  for (size_t i = 0; i < node.functions.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.functions[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_param(s::ostream& out, const Param& node, int depth) const {
  indent(out, depth) << "Param { name=\"" << node.name << "\"";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  indent(out, depth + 1) << "value:\n";
  print_node(out, node.value, depth + 2);
  return out;
}

s::ostream& ASTPrinter::print_save(s::ostream& out, const Save& node, int depth) const {
  indent(out, depth) << "Save { variables=[";
  for (size_t i = 0; i < node.variables.size(); ++i) {
    if (i > 0) out << ", ";
    out << node.variables[i];
  }
  out << "]";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_close(s::ostream& out, const Close& node, int depth) const {
  indent(out, depth) << "Close";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.unit != nullptr) {
    indent(out, depth + 1) << "unit:\n";
    print_node(out, node.unit, depth + 2);
  }
  if (node.status != nullptr) {
    indent(out, depth + 1) << "status:\n";
    print_node(out, node.status, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_open(s::ostream& out, const Open& node, int depth) const {
  indent(out, depth) << "Open";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.unit != nullptr) {
    indent(out, depth + 1) << "unit:\n";
    print_node(out, node.unit, depth + 2);
  }
  if (node.file != nullptr) {
    indent(out, depth + 1) << "file:\n";
    print_node(out, node.file, depth + 2);
  }
  if (node.status != nullptr) {
    indent(out, depth + 1) << "status:\n";
    print_node(out, node.status, depth + 2);
  }
  if (node.access != nullptr) {
    indent(out, depth + 1) << "access:\n";
    print_node(out, node.access, depth + 2);
  }
  if (node.form != nullptr) {
    indent(out, depth + 1) << "form:\n";
    print_node(out, node.form, depth + 2);
  }
  if (node.recl != nullptr) {
    indent(out, depth + 1) << "recl:\n";
    print_node(out, node.recl, depth + 2);
  }
  if (node.blank != nullptr) {
    indent(out, depth + 1) << "blank:\n";
    print_node(out, node.blank, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_read(s::ostream& out, const Read& node, int depth) const {
  indent(out, depth) << "Read";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.unit != nullptr) {
    indent(out, depth + 1) << "unit:\n";
    print_node(out, node.unit, depth + 2);
  }
  if (node.format != nullptr) {
    indent(out, depth + 1) << "format:\n";
    print_node(out, node.format, depth + 2);
  }
  if (!node.variables.empty()) {
    indent(out, depth + 1) << "variables:\n";
    for (const ASTNode* var : node.variables)
      print_node(out, var, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_write(s::ostream& out, const Write& node, int depth) const {
  indent(out, depth) << "Write";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.unit != nullptr) {
    indent(out, depth + 1) << "unit:\n";
    print_node(out, node.unit, depth + 2);
  }
  if (node.format != nullptr) {
    indent(out, depth + 1) << "format:\n";
    print_node(out, node.format, depth + 2);
  }
  if (!node.variables.empty()) {
    indent(out, depth + 1) << "variables:\n";
    for (const ASTNode* var : node.variables)
      print_node(out, var, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_print(s::ostream& out, const Print& node, int depth) const {
  indent(out, depth) << "Print";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.format != nullptr) {
    indent(out, depth + 1) << "format:\n";
    print_node(out, node.format, depth + 2);
  }
  if (!node.variables.empty()) {
    indent(out, depth + 1) << "variables:\n";
    for (const ASTNode* var : node.variables)
      print_node(out, var, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_continue(s::ostream& out, const Continue& node, int depth) const {
  indent(out, depth) << "Continue";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  return out;
}

s::ostream& ASTPrinter::print_return(s::ostream& out, const Return& node, int depth) const {
  indent(out, depth) << "Return";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  if (node.expr != nullptr) {
    indent(out, depth + 1) << "expr:\n";
    print_node(out, node.expr, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_call(s::ostream& out, const Call& node, int depth) const {
  indent(out, depth) << "Call { name=\"" << node.name
                     << "\", rtype=" << ty_name(node.rtype.type);
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  if (!node.arguments.empty()) {
    indent(out, depth + 1) << "arguments:\n";
    for (const ASTNode* arg : node.arguments)
      print_node(out, arg, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_lit(s::ostream& out, const Lit& node, int depth) const {
  indent(out, depth) << "Lit { type=" << ty_name(node.type.type)
                     << ", value=\"" << node.value << "\"";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  return out;
}

s::ostream& ASTPrinter::print_indexing(s::ostream& out, const Indexing& node, int depth) const {
  indent(out, depth) << "Indexing { rtype=" << ty_name(node.rtype.type);
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  indent(out, depth + 1) << "variable:\n";
  print_node(out, node.variable, depth + 2);
  if (node.indexes != nullptr) {
    indent(out, depth + 1) << "indexes:\n";
    print_indexlist(out, *node.indexes, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_decl(s::ostream& out, const Decl& node, int depth) const {
  indent(out, depth) << "Decl { type=" << ty_name(node.type.type)
                     << ", name=\"" << node.name << "\"";
  if (node.label != ASTNode::kInvalidLabel)
    out << ", label=" << node.label;
  out << " }\n";
  if (node.sizes != nullptr) {
    indent(out, depth + 1) << "sizes:\n";
    print_indexlist(out, *node.sizes, depth + 2);
  }
  return out;
}

s::ostream& ASTPrinter::print_indexlist(s::ostream& out, const IndexList& node, int depth) const {
  indent(out, depth) << "IndexList";
  if (node.label != ASTNode::kInvalidLabel)
    out << " { label=" << node.label << " }";
  out << "\n";
  for (const ASTNode* idx : node.indexes)
    print_node(out, idx, depth + 1);
  return out;
}

s::ostream& ASTPrinter::print(s::ostream& out, const ASTNode* root) const {
  return print_node(out, root, 0);
}

s::ostream& ASTPrinter::print(s::ostream& out, const CmpStmt& stmt) const {
  return print_cmpstmt(out, stmt, 0);
}

} // namespace f77i
