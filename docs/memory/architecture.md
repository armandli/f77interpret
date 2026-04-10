# Architecture

This file contains the mermaid class diagram describing the architecture of this project.
Update this diagram when making structural changes (new classes, interfaces, relationships, method signatures).
Changes here drive source code updates via the agent-memory-v2 skill (Path C).

```mermaid
classDiagram
    namespace f77i {
        class Config {
            +string source_file
            +vector~string~ include_paths
            +Codepoint codepoint
            +int SCSrcCol
            +int SCMaxCol
        }
        class TT {
            <<enumeration>>
            INTEGER REAL DOUBLE COMPLEX LOGICAL CHARACTER
            IF THEN ELSE ELSEIF ENDIF
            DO ENDDO CONTINUE GOTO STOP PAUSE END
            PROGRAM SUBROUTINE FUNCTION BLOCK DATA
            READ WRITE PRINT TYPE
            OPEN CLOSE REWIND BACKSPACE ENDFILE
            FORMAT COMMON EQUIVALENCE EXTERNAL INTRINSIC
            SAVE PARAMETER IMPLICIT DIMENSION NONE
            ENTRY INCLUDE CALL RETURN
            PLUS MINUS STAR SLASH DSTAR DSLASH
            ASSIGN
            EQ NE LT LE GT GE
            AND OR NOT EQV NEQV
            TRUE FALSE
            LPAREN RPAREN COMMA COLON SEMICOLON SQUOTE DQUOTE
            INT_LITERAL DBL_LITERAL RL_LITERAL CMPX_LITERAL CHR_LITERAL
            LABEL IDENTIFIER NEWLINE END_OF_FILE UNKNOWN
        }
        class Token {
            +TT type
            +int lno
            +int llno
            +string_view value
        }
        class Lexer {
            -Config mConf
            +Lexer(conf Config)
            +tokenize(source_code string, start_lno int) vector~Token~
            +count_lines(source_code string) int
            +count_logical_lines(source_code string) int
            #isCommentLine(line string) bool
            #isContinuationLine(line string) bool
        }
        class SourceBase {
            <<abstract>>
            #Config mConf
            #vector~Token~ mTokens
            +SourceBase(conf Config)
            +tokens() vector~Token~
            +content() string*
        }
        class SourceFile {
            -path mPath
            -string mContent
            +SourceFile(path path, conf Config)
            +SourceFile(other SourceFile)
            +filename() path
            +content() string
            -tokenize()
        }
        class TY {
            <<enumeration>>
            INTEGER REAL DOUBLE COMPLEX CHARACTER LOGICAL VOID UNKNOWN
        }
        class VT {
            <<enumeration>>
            VARLEN UNKNOWN
        }
        class BOP {
            <<enumeration>>
            ADD SUB MUL DIV POW EQ NE GT GE LT LE AND OR NOT EQV NEQV CAT UNKNOWN
        }
        class UOP {
            <<enumeration>>
            NEG POS NOT UNKNOWN
        }
        class ASTNode {
            <<abstract>>
            +int label
            +ASTNode()
            +ASTNode(label int)
            +~ASTNode() virtual
        }
        class CmpStmt {
            +vector~ASTNode*~ stmts
            +CmpStmt()
            +CmpStmt(other CmpStmt)
            +~CmpStmt() override
        }
        class Sub {
            +string_view name
            +vector~string_view~ parameters
            +CmpStmt body
            +Sub(name string_view, parameters vector, body CmpStmt&&)
            +~Sub() override
        }
        class Prog {
            +string_view name
            +CmpStmt body
            +Prog(name string_view, body CmpStmt&&)
            +~Prog() override
        }
        class If {
            +ASTNode* condition
            +ASTNode* else_block
            +CmpStmt body
            +If()
            +If(condition ASTNode*, else_block ASTNode*, body CmpStmt&&)
            +~If() override
        }
        class Do {
            +ASTNode* init
            +ASTNode* fin
            +ASTNode* step
            +CmpStmt body
            +Do()
            +Do(init ASTNode*, fin ASTNode*, body CmpStmt&&, label int)
            +Do(init ASTNode*, fin ASTNode*, step ASTNode*, body CmpStmt&&, label int)
            +~Do() override
        }
        class FunType {
            +TY type
            +size_t size
            +bool varlen
            +FunType(type TY, vt VT)
            +FunType(type TY, size size_t)
        }
        class Function {
            +string_view name
            +vector~string_view~ parameters
            +FunType return_type
            +CmpStmt body
            +Function(name string_view, parameters vector, return_type FunType, body CmpStmt&&, label int)
            +~Function() override
        }
        class BinaryOp {
            +BOP op
            +ASTNode* left
            +ASTNode* right
            +FunType rtype
            +BinaryOp(op BOP, left ASTNode*, right ASTNode*, rtype FunType, label int)
            +~BinaryOp() override
        }
        class Var {
            +string_view name
            +FunType rtype
            +Var(name string_view, rtype FunType, label int)
            +~Var() override
        }
        class UnaryOp {
            +UOP op
            +ASTNode* expr
            +FunType rtype
            +UnaryOp(op UOP, expr ASTNode*, rtype FunType, label int)
            +~UnaryOp() override
        }
        class Call {
            +string_view name
            +vector~ASTNode*~ arguments
            +FunType rtype
            +Call(name string_view, arguments vector~ASTNode*~, rtype FunType, label int)
            +~Call() override
        }
        class Star {
            +Star(label int)
            +~Star() override
        }
        class Implicit {
            +Implicit(label int)
            +~Implicit() override
        }
        class Goto {
            +ASTNode* var
            +vector~int~ labels
            +Goto(var ASTNode*, labels vector~int~, label int)
            +~Goto() override
        }
        class Format {
            +vector~string_view~ specification
            +Format(specification vector~string_view~, label int)
            +~Format() override
        }
        class Data {
            +vector~string_view~ variables
            +vector~ASTNode*~ values
            +Data(variables vector~string_view~, values vector~ASTNode*~, label int)
            +~Data() override
        }
        class Common {
            +string_view name
            +vector~string_view~ variables
            +Common(name string_view, variables vector~string_view~, label int)
            +~Common() override
        }
        class External {
            +vector~string_view~ procedures
            +External(procedures vector~string_view~, label int)
            +~External() override
        }
        class Dimension {
            +string_view variable
            +vector~ASTNode*~ dimensions
            +Dimension(variable string_view, dimensions vector~ASTNode*~, label int)
            +~Dimension() override
        }
        class Stop {
            +ASTNode* message
            +Stop(message ASTNode*, label int)
            +~Stop() override
        }
        class Pause {
            +ASTNode* message
            +Pause(message ASTNode*, label int)
            +~Pause() override
        }
        class Entry {
            +string_view name
            +vector~string_view~ parameters
            +Entry(name string_view, parameters vector~string_view~, label int)
            +~Entry() override
        }
        class Interface {
            +string_view name
            +vector~string_view~ parameters
            +Interface(name string_view, parameters vector~string_view~, label int)
            +~Interface() override
        }
        class Equivalence {
            +vector~string_view~ group
            +Equivalence(group vector~string_view~, label int)
            +~Equivalence() override
        }
        class Intrinsic {
            +vector~string_view~ functions
            +Intrinsic(functions vector~string_view~, label int)
            +~Intrinsic() override
        }
        class Param {
            +string_view name
            +ASTNode* value
            +Param(name string_view, value ASTNode*, label int)
            +~Param() override
        }
        class Save {
            +vector~string_view~ variables
            +Save(variables vector~string_view~, label int)
            +~Save() override
        }
        class Close {
            +ASTNode* unit
            +ASTNode* status
            +Close(unit ASTNode*, status ASTNode*, label int)
            +~Close() override
        }
        class Open {
            +ASTNode* unit
            +ASTNode* file
            +ASTNode* status
            +ASTNode* access
            +ASTNode* form
            +ASTNode* recl
            +ASTNode* blank
            +Open(unit ASTNode*, file ASTNode*, status ASTNode*, access ASTNode*, form ASTNode*, recl ASTNode*, blank ASTNode*, label int)
            +~Open() override
        }
        class Read {
            +ASTNode* unit
            +ASTNode* format
            +vector~ASTNode*~ variables
            +Read(unit ASTNode*, format ASTNode*, variables vector~ASTNode*~, label int)
            +~Read() override
        }
        class Write {
            +ASTNode* unit
            +ASTNode* format
            +vector~ASTNode*~ variables
            +Write(unit ASTNode*, format ASTNode*, variables vector~ASTNode*~, label int)
            +~Write() override
        }
        class Print {
            +ASTNode* format
            +vector~ASTNode*~ variables
            +Print(format ASTNode*, variables vector~ASTNode*~, label int)
            +~Print() override
        }
        class Continue {
            +Continue(label int)
            +~Continue() override
        }
        class Return {
            +ASTNode* expr
            +FunType rtype
            +Return(expr ASTNode*, label int)
            +~Return() override
        }
        class Lit {
            +FunType type
            +string_view value
            +Lit(type FunType, value string_view, label int)
            +~Lit() override
        }
        class Indexing {
            +Var* variable
            +IndexList* indexes
            +FunType rtype
            +Indexing(variable Var*, indexes IndexList*, rtype FunType, label int)
            +~Indexing() override
        }
        class Decl {
            +FunType type
            +string_view name
            +IndexList* sizes
            +Decl(type FunType, name string_view, sizes IndexList*, label int)
            +~Decl() override
        }
        class IndexList {
            +vector~ASTNode*~ indexes
            +IndexList(indexes vector~ASTNode*~, label int)
            +~IndexList() override
        }
        class TokenDebugPrinter {
            +print(token Token) void
        }
        class InteractiveSourceFile {
            -deque~string~ mLines
            -int mNextLno
            +InteractiveSourceFile(conf Config)
            +append(text string)
            +lineCount() int
            +content() string
        }
    }
    class main {
        <<entrypoint>>
        +main(argc int, argv string[]) int
    }
    main --> Config : creates
    TokenDebugPrinter --> Token : inspects
    Token --> TT : uses
    Lexer --> Config : owns
    Lexer --> Token : produces
    SourceBase --> Config : references
    SourceBase --> Token : owns
    SourceFile --|> SourceBase : extends
    SourceFile --> Lexer : uses
    InteractiveSourceFile --|> SourceBase : extends
    InteractiveSourceFile --> Lexer : uses
    CmpStmt --|> ASTNode : extends
    Sub --|> ASTNode : extends
    Prog --|> ASTNode : extends
    If --|> ASTNode : extends
    Sub --> CmpStmt : owns
    Prog --> CmpStmt : owns
    If --> CmpStmt : owns
    If --> ASTNode : condition
    If --> ASTNode : else_block
    Do --|> ASTNode : extends
    Do --> CmpStmt : owns
    Do --> ASTNode : init
    Do --> ASTNode : fin
    Do --> ASTNode : step
    IndexList --|> ASTNode : extends
    Function --|> ASTNode : extends
    Function --> CmpStmt : owns
    Function --> FunType : return_type
    Var --|> ASTNode : extends
    Var --> FunType : rtype
    UnaryOp --|> ASTNode : extends
    UnaryOp --> UOP : op
    UnaryOp --> FunType : rtype
    BinaryOp --|> ASTNode : extends
    BinaryOp --> BOP : op
    BinaryOp --> FunType : rtype
    Call --|> ASTNode : extends
    Call --> FunType : rtype
    Star --|> ASTNode : extends
    Implicit --|> ASTNode : extends
    Format --|> ASTNode : extends
    Data --|> ASTNode : extends
    Common --|> ASTNode : extends
    External --|> ASTNode : extends
    Dimension --|> ASTNode : extends
    Stop --|> ASTNode : extends
    Pause --|> ASTNode : extends
    Entry --|> ASTNode : extends
    Interface --|> ASTNode : extends
    Equivalence --|> ASTNode : extends
    Intrinsic --|> ASTNode : extends
    Param --|> ASTNode : extends
    Save --|> ASTNode : extends
    Close --|> ASTNode : extends
    Open --|> ASTNode : extends
    Read --|> ASTNode : extends
    Write --|> ASTNode : extends
    Print --|> ASTNode : extends
    Goto --|> ASTNode : extends
    Continue --|> ASTNode : extends
    Return --|> ASTNode : extends
    Return --> FunType : rtype
    Lit --|> ASTNode : extends
    Lit --> FunType : type
    Indexing --|> ASTNode : extends
    Indexing --> Var : variable
    Indexing --> IndexList : indexes
    Indexing --> FunType : rtype
    Decl --|> ASTNode : extends
    Decl --> FunType : type
    Decl --> IndexList : sizes
    FunType --> TY : uses
    FunType --> VT : uses
```
