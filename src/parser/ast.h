#ifndef F77INTERPRET_PARSER_AST_H
#define F77INTERPRET_PARSER_AST_H

namespace f77i {

struct ASTNode {
  static constexpr int kInvalidLabel = -1;

  ASTNode() : label(kInvalidLabel) {}
  explicit ASTNode(int label) : label(label) {}
  virtual ~ASTNode() = default;

  int label;
};

} // namespace f77i

#endif // F77INTERPRET_PARSER_AST_H
