#ifndef F77INTERPRET_PARSER_AST_H
#define F77INTERPRET_PARSER_AST_H

#include <string_view>
#include <vector>
#include <alias.h>

#define DELETE_IF(ptr) do { if ((ptr) != nullptr) delete (ptr); } while (false)

namespace f77i {

enum class TY : int {
  INTEGER = 0, REAL, DOUBLE, COMPLEX, CHARACTER, LOGICAL, VOID, UNKNOWN
};

enum class VT : int {
  VARLEN, UNKNOWN
};

struct FunType {
  TY type;
  size_t size;
  bool varlen;

  explicit FunType(TY type, VT vt = VT::UNKNOWN)
    : type(type), size(vt == VT::VARLEN ? 0 : 1), varlen(vt == VT::VARLEN) {}
  FunType(TY type, size_t size)
    : type(type), size(size), varlen(false) {}
};

struct ASTNode {
  static constexpr int kInvalidLabel = -1;

  ASTNode() : label(kInvalidLabel) {}
  explicit ASTNode(int label) : label(label) {}
  virtual ~ASTNode() = default;

  int label;
};

struct CmpStmt : ASTNode {
  s::vector<ASTNode*> stmts;

  CmpStmt() = default;
  CmpStmt(CmpStmt&& other) : ASTNode(other.label), stmts(s::move(other.stmts)) {}
  ~CmpStmt() override {
    for (ASTNode* node : stmts) {
      DELETE_IF(node);
    }
  }
};

struct Sub : ASTNode {
  s::string_view name;
  s::vector<s::string_view> parameters;
  CmpStmt body;

  Sub(s::string_view name, s::vector<s::string_view> parameters, CmpStmt&& body, int label = kInvalidLabel)
    : ASTNode(label), name(name), parameters(s::move(parameters)), body(s::move(body)) {}
  ~Sub() override = default;
};

struct Prog : ASTNode {
  s::string_view name;
  CmpStmt body;

  Prog(s::string_view name, CmpStmt&& body, int label = kInvalidLabel)
    : ASTNode(label), name(name), body(s::move(body)) {}
  ~Prog() override = default;
};

struct If : ASTNode {
  ASTNode* condition;
  ASTNode* else_block;
  CmpStmt body;

  If() : ASTNode(), condition(nullptr), else_block(nullptr) {}
  If(ASTNode* condition, ASTNode* else_block, CmpStmt&& body, int label = kInvalidLabel)
    : ASTNode(label), condition(condition), else_block(else_block), body(s::move(body)) {}
  ~If() override {
    DELETE_IF(condition);
    DELETE_IF(else_block);
  }
};

struct Do : ASTNode {
  ASTNode* init;
  ASTNode* fin;
  ASTNode* step;
  CmpStmt body;

  Do() : ASTNode(), init(nullptr), fin(nullptr), step(nullptr) {}
  Do(ASTNode* init, ASTNode* fin, CmpStmt&& body, int label = kInvalidLabel)
    : ASTNode(label), init(init), fin(fin), step(nullptr), body(s::move(body)) {}
  Do(ASTNode* init, ASTNode* fin, ASTNode* step, CmpStmt&& body, int label = kInvalidLabel)
    : ASTNode(label), init(init), fin(fin), step(step), body(s::move(body)) {}
  ~Do() override {
    DELETE_IF(init);
    DELETE_IF(fin);
    DELETE_IF(step);
  }
};

struct Function : ASTNode {
  s::string_view name;
  s::vector<s::string_view> parameters;
  FunType return_type;
  CmpStmt body;

  Function(s::string_view name, s::vector<s::string_view> parameters, FunType return_type, CmpStmt&& body, int label = kInvalidLabel)
    : ASTNode(label), name(name), parameters(s::move(parameters)), return_type(return_type), body(s::move(body)) {}
  ~Function() override = default;
};

struct Decl : ASTNode {
  FunType type;
  s::string_view name;
  IndexList* sizes;

  Decl(FunType type, s::string_view name, IndexList* sizes, int label = kInvalidLabel)
    : ASTNode(label), type(type), name(name), sizes(sizes) {}
  ~Decl() override {
    DELETE_IF(sizes);
  }
};

struct IndexList : ASTNode {
  s::vector<ASTNode*> indexes;

  IndexList(s::vector<ASTNode*>&& indexes, int label = kInvalidLabel)
    : ASTNode(label), indexes(s::move(indexes)) {}
  ~IndexList() override {
    for (ASTNode* node : indexes) {
      DELETE_IF(node);
    }
  }
};

} // namespace f77i

#endif // F77INTERPRET_PARSER_AST_H
