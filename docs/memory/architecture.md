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
    Decl --|> ASTNode : extends
    Decl --> FunType : type
    Decl --> IndexList : sizes
    FunType --> TY : uses
    FunType --> VT : uses
```
