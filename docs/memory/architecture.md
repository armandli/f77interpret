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
    }
    class main {
        <<entrypoint>>
        +main(argc int, argv string[]) int
    }
    main --> Config : creates
    Token --> TT : uses
    Lexer --> Config : owns
    Lexer --> Token : produces
```
