#ifndef F77INTERPRET_PARSER_AST_H
#define F77INTERPRET_PARSER_AST_H

#include <cassert>
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

enum class BOP : int {
  ADD, SUB, MUL, DIV, POW, EQ, NE, GT, GE, LT, LE, AND, OR, NOT, EQV, NEQV, CAT, UNKNOWN
};

enum class UOP : int {
  NEG, POS, NOT, UNKNOWN
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

struct Assign : ASTNode {
  ASTNode* variable;
  ASTNode* expr;

  Assign(ASTNode* variable, ASTNode* expr, int label = kInvalidLabel)
    : ASTNode(label), variable(variable), expr(expr) {}
  ~Assign() override {
    DELETE_IF(variable);
    DELETE_IF(expr);
  }
};

struct BinaryOp : ASTNode {
  BOP op;
  ASTNode* left;
  ASTNode* right;
  FunType rtype;

  BinaryOp(BOP op, ASTNode* left, ASTNode* right, FunType rtype, int label = kInvalidLabel)
    : ASTNode(label), op(op), left(left), right(right), rtype(rtype) {
    assert(op != BOP::UNKNOWN);
  }
  ~BinaryOp() override {
    DELETE_IF(left);
    DELETE_IF(right);
  }
};

struct UnaryOp : ASTNode {
  UOP op;
  ASTNode* expr;
  FunType rtype;

  UnaryOp(UOP op, ASTNode* expr, FunType rtype, int label = kInvalidLabel)
    : ASTNode(label), op(op), expr(expr), rtype(rtype) {
    assert(op != UOP::UNKNOWN);
  }
  ~UnaryOp() override {
    DELETE_IF(expr);
  }
};

struct Var : ASTNode {
  s::string_view name;
  FunType rtype;

  Var(s::string_view name, FunType rtype, int label = kInvalidLabel)
    : ASTNode(label), name(name), rtype(rtype) {}
  ~Var() override = default;
};

struct Star : ASTNode {
  Star(int label = kInvalidLabel) : ASTNode(label) {}
  ~Star() override = default;
};

struct Implicit : ASTNode {
  Implicit(int label = kInvalidLabel) : ASTNode(label) {}
  ~Implicit() override = default;
};

struct Goto : ASTNode {
  ASTNode* var;
  s::vector<int> labels;

  Goto(ASTNode* var, s::vector<int> labels, int label = kInvalidLabel)
    : ASTNode(label), var(var), labels(s::move(labels)) {}
  ~Goto() override {
    DELETE_IF(var);
  }
};

struct Format : ASTNode {
  s::vector<s::string_view> specification;

  Format(s::vector<s::string_view> specification, int label = kInvalidLabel)
    : ASTNode(label), specification(s::move(specification)) {}
  ~Format() override = default;
};

struct Data : ASTNode {
  s::vector<s::string_view> variables;
  s::vector<ASTNode*> values;

  Data(s::vector<s::string_view> variables = {}, s::vector<ASTNode*> values = {}, int label = kInvalidLabel)
    : ASTNode(label), variables(s::move(variables)), values(s::move(values)) {}
  ~Data() override {
    for (ASTNode* node : values) {
      DELETE_IF(node);
    }
  }
};

struct Common : ASTNode {
  s::string_view name;
  s::vector<s::string_view> variables;

  Common(s::string_view name = {}, s::vector<s::string_view> variables = {}, int label = kInvalidLabel)
    : ASTNode(label), name(name), variables(s::move(variables)) {}
  ~Common() override = default;
};

struct External : ASTNode {
  s::vector<s::string_view> procedures;

  External(s::vector<s::string_view> procedures = {}, int label = kInvalidLabel)
    : ASTNode(label), procedures(s::move(procedures)) {}
  ~External() override = default;
};

struct Dimension : ASTNode {
  s::string_view variable;
  s::vector<ASTNode*> dimensions;

  Dimension(s::string_view variable = {}, s::vector<ASTNode*> dimensions = {}, int label = kInvalidLabel)
    : ASTNode(label), variable(variable), dimensions(s::move(dimensions)) {}
  ~Dimension() override {
    for (ASTNode* node : dimensions) {
      DELETE_IF(node);
    }
  }
};

struct Stop : ASTNode {
  ASTNode* message;

  Stop(ASTNode* message = nullptr, int label = kInvalidLabel)
    : ASTNode(label), message(message) {}
  ~Stop() override {
    DELETE_IF(message);
  }
};

struct Pause : ASTNode {
  ASTNode* message;

  Pause(ASTNode* message = nullptr, int label = kInvalidLabel)
    : ASTNode(label), message(message) {}
  ~Pause() override {
    DELETE_IF(message);
  }
};

struct Entry : ASTNode {
  s::string_view name;
  s::vector<s::string_view> parameters;

  Entry(s::string_view name, s::vector<s::string_view> parameters, int label = kInvalidLabel)
    : ASTNode(label), name(name), parameters(s::move(parameters)) {}
  ~Entry() override = default;
};

struct Equivalence : ASTNode {
  s::vector<s::string_view> group;

  Equivalence(s::vector<s::string_view> group = {}, int label = kInvalidLabel)
    : ASTNode(label), group(s::move(group)) {}
  ~Equivalence() override = default;
};

struct Intrinsic : ASTNode {
  s::vector<s::string_view> functions;

  Intrinsic(s::vector<s::string_view> functions = {}, int label = kInvalidLabel)
    : ASTNode(label), functions(s::move(functions)) {}
  ~Intrinsic() override = default;
};

struct Param : ASTNode {
  s::string_view name;
  ASTNode* value;

  Param(s::string_view name, ASTNode* value, int label = kInvalidLabel)
    : ASTNode(label), name(name), value(value) {}
  ~Param() override {
    DELETE_IF(value);
  }
};

struct Save : ASTNode {
  s::vector<s::string_view> variables;

  Save(s::vector<s::string_view> variables = {}, int label = kInvalidLabel)
    : ASTNode(label), variables(s::move(variables)) {}
  ~Save() override = default;
};

struct Close : ASTNode {
  ASTNode* unit;
  ASTNode* status;

  Close(ASTNode* unit = nullptr, ASTNode* status = nullptr, int label = kInvalidLabel)
    : ASTNode(label), unit(unit), status(status) {}
  ~Close() override {
    DELETE_IF(unit);
    DELETE_IF(status);
  }
};

struct Open : ASTNode {
  ASTNode* unit;
  ASTNode* file;
  ASTNode* status;
  ASTNode* access;
  ASTNode* form;
  ASTNode* recl;
  ASTNode* blank;

  Open(ASTNode* unit = nullptr, ASTNode* file = nullptr, ASTNode* status = nullptr,
       ASTNode* access = nullptr, ASTNode* form = nullptr, ASTNode* recl = nullptr,
       ASTNode* blank = nullptr, int label = kInvalidLabel)
    : ASTNode(label), unit(unit), file(file), status(status),
      access(access), form(form), recl(recl), blank(blank) {}
  ~Open() override {
    DELETE_IF(unit);
    DELETE_IF(file);
    DELETE_IF(status);
    DELETE_IF(access);
    DELETE_IF(form);
    DELETE_IF(recl);
    DELETE_IF(blank);
  }
};

struct Read : ASTNode {
  ASTNode* unit;
  ASTNode* format;
  s::vector<ASTNode*> variables;

  Read(ASTNode* unit = nullptr, ASTNode* format = nullptr, s::vector<ASTNode*> variables = {}, int label = kInvalidLabel)
    : ASTNode(label), unit(unit), format(format), variables(s::move(variables)) {}
  ~Read() override {
    DELETE_IF(unit);
    DELETE_IF(format);
    for (ASTNode* node : variables) {
      DELETE_IF(node);
    }
  }
};

struct Write : ASTNode {
  ASTNode* unit;
  ASTNode* format;
  s::vector<ASTNode*> variables;

  Write(ASTNode* unit = nullptr, ASTNode* format = nullptr, s::vector<ASTNode*> variables = {}, int label = kInvalidLabel)
    : ASTNode(label), unit(unit), format(format), variables(s::move(variables)) {}
  ~Write() override {
    DELETE_IF(unit);
    DELETE_IF(format);
    for (ASTNode* node : variables) {
      DELETE_IF(node);
    }
  }
};

struct Print : ASTNode {
  ASTNode* format;
  s::vector<ASTNode*> variables;

  Print(ASTNode* format = nullptr, s::vector<ASTNode*> variables = {}, int label = kInvalidLabel)
    : ASTNode(label), format(format), variables(s::move(variables)) {}
  ~Print() override {
    DELETE_IF(format);
    for (ASTNode* node : variables) {
      DELETE_IF(node);
    }
  }
};

struct Continue : ASTNode {
  Continue(int label = kInvalidLabel) : ASTNode(label) {}
  ~Continue() override = default;
};

struct Return : ASTNode {
  ASTNode* expr;
  FunType rtype;

  Return(ASTNode* expr, int label = kInvalidLabel)
    : ASTNode(label), expr(expr), rtype(TY::UNKNOWN) {}
  ~Return() override {
    DELETE_IF(expr);
  }
};

struct Call : ASTNode {
  s::string_view name;
  s::vector<ASTNode*> arguments;
  FunType rtype;

  Call(s::string_view name, s::vector<ASTNode*>&& arguments, FunType rtype, int label = kInvalidLabel)
    : ASTNode(label), name(name), arguments(s::move(arguments)), rtype(rtype) {}
  ~Call() override {
    for (ASTNode* node : arguments) {
      DELETE_IF(node);
    }
  }
};

struct Lit : ASTNode {
  FunType type;
  s::string_view value;

  Lit(FunType type, s::string_view value, int label = kInvalidLabel)
    : ASTNode(label), type(type), value(value) {}
  ~Lit() override = default;
};

struct Indexing : ASTNode {
  Var* variable;
  IndexList* indexes;
  FunType rtype;

  Indexing(Var* variable, IndexList* indexes, FunType rtype, int label = kInvalidLabel)
    : ASTNode(label), variable(variable), indexes(indexes), rtype(rtype) {}
  ~Indexing() override {
    DELETE_IF(variable);
    DELETE_IF(indexes);
  }
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
