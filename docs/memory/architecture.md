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
        class ASTNode {
            <<abstract>>
            +int label
            +ASTNode()
            +ASTNode(label int)
            +~ASTNode() virtual
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
```
