#include <gtest/gtest.h>
#include <lexer.h>
#include <exception.h>
#include <string>
#include <string_view>
#include <vector>

using namespace f77i;

// 6-space prefix + code + newline: standard fixed-format F77 line with no label/continuation
static std::string fmtLine(const std::string& code) {
  return "      " + code + "\n";
}

// label(up to 5 chars, right-padded) + space(col 6) + code + newline
static std::string fmtLabel(const std::string& label, const std::string& code) {
  std::string line = label;
  while (line.size() < 5) line += ' ';
  line += ' '; // col 5: no continuation
  line += code + "\n";
  return line;
}

class LexerTest : public ::testing::Test {
protected:
  Config cfg; // defaults: SCSrcCol=6, SCMaxCol=71, codepoint=ASCII
};

// ============================================================
// Keywords
// ============================================================

TEST_F(LexerTest, TokenINTEGER) {
  Lexer lex(cfg);
  std::string src = fmtLine("INTEGER");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 2u);
  EXPECT_EQ(toks[0].type, TT::INTEGER);
  EXPECT_EQ(toks[0].value, "INTEGER");
}

TEST_F(LexerTest, TokenREAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("REAL");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::REAL);
}

TEST_F(LexerTest, TokenDOUBLE) {
  Lexer lex(cfg);
  std::string src = fmtLine("DOUBLE");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::DOUBLE);
}

TEST_F(LexerTest, TokenCOMPLEX) {
  Lexer lex(cfg);
  std::string src = fmtLine("COMPLEX");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::COMPLEX);
}

TEST_F(LexerTest, TokenLOGICAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("LOGICAL");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::LOGICAL);
}

TEST_F(LexerTest, TokenCHARACTER) {
  Lexer lex(cfg);
  std::string src = fmtLine("CHARACTER");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::CHARACTER);
}

TEST_F(LexerTest, TokenIF) {
  Lexer lex(cfg);
  std::string src = fmtLine("IF (X)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::IF);
}

TEST_F(LexerTest, TokenTHEN) {
  Lexer lex(cfg);
  std::string src = fmtLine("IF (X) THEN");
  auto toks = lex.tokenize(src);
  // IF LPAREN X RPAREN THEN NEWLINE EOF
  ASSERT_GE(toks.size(), 5u);
  EXPECT_EQ(toks[4].type, TT::THEN);
}

TEST_F(LexerTest, TokenELSE) {
  Lexer lex(cfg);
  std::string src = fmtLine("ELSE");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::ELSE);
}

TEST_F(LexerTest, TokenELSEIF) {
  Lexer lex(cfg);
  std::string src = fmtLine("ELSEIF");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::ELSEIF);
}

TEST_F(LexerTest, TokenENDIF) {
  Lexer lex(cfg);
  std::string src = fmtLine("ENDIF");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::ENDIF);
}

TEST_F(LexerTest, TokenDO) {
  Lexer lex(cfg);
  std::string src = fmtLine("DO");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::DO);
}

TEST_F(LexerTest, TokenENDDO) {
  Lexer lex(cfg);
  std::string src = fmtLine("ENDDO");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::ENDDO);
}

TEST_F(LexerTest, TokenCONTINUE) {
  Lexer lex(cfg);
  std::string src = fmtLine("CONTINUE");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::CONTINUE);
}

TEST_F(LexerTest, TokenGOTO) {
  Lexer lex(cfg);
  std::string src = fmtLine("GOTO 100");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::GOTO);
}

TEST_F(LexerTest, TokenSTOP) {
  Lexer lex(cfg);
  std::string src = fmtLine("STOP");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::STOP);
}

TEST_F(LexerTest, TokenPAUSE) {
  Lexer lex(cfg);
  std::string src = fmtLine("PAUSE");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::PAUSE);
}

TEST_F(LexerTest, TokenEND) {
  Lexer lex(cfg);
  std::string src = fmtLine("END");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::END);
}

TEST_F(LexerTest, TokenPROGRAM) {
  Lexer lex(cfg);
  std::string src = fmtLine("PROGRAM MAIN");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::PROGRAM);
}

TEST_F(LexerTest, TokenSUBROUTINE) {
  Lexer lex(cfg);
  std::string src = fmtLine("SUBROUTINE FOO");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::SUBROUTINE);
}

TEST_F(LexerTest, TokenFUNCTION) {
  Lexer lex(cfg);
  std::string src = fmtLine("FUNCTION BAR");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::FUNCTION);
}

TEST_F(LexerTest, TokenBLOCK) {
  Lexer lex(cfg);
  std::string src = fmtLine("BLOCK DATA");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::BLOCK);
}

TEST_F(LexerTest, TokenDATA) {
  Lexer lex(cfg);
  std::string src = fmtLine("DATA X /1/");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::DATA);
}

TEST_F(LexerTest, TokenREAD) {
  Lexer lex(cfg);
  std::string src = fmtLine("READ *,X");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::READ);
}

TEST_F(LexerTest, TokenWRITE) {
  Lexer lex(cfg);
  std::string src = fmtLine("WRITE (*,*)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::WRITE);
}

TEST_F(LexerTest, TokenPRINT) {
  Lexer lex(cfg);
  std::string src = fmtLine("PRINT *,X");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::PRINT);
}

TEST_F(LexerTest, TokenTYPE) {
  Lexer lex(cfg);
  std::string src = fmtLine("TYPE *,X");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::TYPE);
}

TEST_F(LexerTest, TokenOPEN) {
  Lexer lex(cfg);
  std::string src = fmtLine("OPEN (10)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::OPEN);
}

TEST_F(LexerTest, TokenCLOSE) {
  Lexer lex(cfg);
  std::string src = fmtLine("CLOSE (10)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::CLOSE);
}

TEST_F(LexerTest, TokenREWIND) {
  Lexer lex(cfg);
  std::string src = fmtLine("REWIND 10");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::REWIND);
}

TEST_F(LexerTest, TokenBACKSPACE) {
  Lexer lex(cfg);
  std::string src = fmtLine("BACKSPACE 10");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::BACKSPACE);
}

TEST_F(LexerTest, TokenENDFILE) {
  Lexer lex(cfg);
  std::string src = fmtLine("ENDFILE 10");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::ENDFILE);
}

TEST_F(LexerTest, TokenFORMAT) {
  Lexer lex(cfg);
  std::string src = fmtLine("FORMAT (I10)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::FORMAT);
}

TEST_F(LexerTest, TokenCOMMON) {
  Lexer lex(cfg);
  std::string src = fmtLine("COMMON X");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::COMMON);
}

TEST_F(LexerTest, TokenEQUIVALENCE) {
  Lexer lex(cfg);
  std::string src = fmtLine("EQUIVALENCE (X,Y)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::EQUIVALENCE);
}

TEST_F(LexerTest, TokenEXTERNAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("EXTERNAL FOO");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::EXTERNAL);
}

TEST_F(LexerTest, TokenINTRINSIC) {
  Lexer lex(cfg);
  std::string src = fmtLine("INTRINSIC ABS");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::INTRINSIC);
}

TEST_F(LexerTest, TokenSAVE) {
  Lexer lex(cfg);
  std::string src = fmtLine("SAVE X");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::SAVE);
}

TEST_F(LexerTest, TokenPARAMETER) {
  Lexer lex(cfg);
  std::string src = fmtLine("PARAMETER (PI=3)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::PARAMETER);
}

TEST_F(LexerTest, TokenIMPLICIT) {
  Lexer lex(cfg);
  std::string src = fmtLine("IMPLICIT NONE");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::IMPLICIT);
}

TEST_F(LexerTest, TokenDIMENSION) {
  Lexer lex(cfg);
  std::string src = fmtLine("DIMENSION A(10)");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::DIMENSION);
}

TEST_F(LexerTest, TokenNONE) {
  Lexer lex(cfg);
  // IMPLICIT NONE: second token is NONE
  std::string src = fmtLine("IMPLICIT NONE");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::NONE);
}

TEST_F(LexerTest, TokenENTRY) {
  Lexer lex(cfg);
  std::string src = fmtLine("ENTRY FOO");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::ENTRY);
}

TEST_F(LexerTest, TokenINCLUDE) {
  Lexer lex(cfg);
  std::string src = fmtLine("INCLUDE 'FILE.F'");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::INCLUDE);
}

TEST_F(LexerTest, TokenCALL) {
  Lexer lex(cfg);
  std::string src = fmtLine("CALL FOO");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::CALL);
}

TEST_F(LexerTest, TokenRETURN) {
  Lexer lex(cfg);
  std::string src = fmtLine("RETURN");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::RETURN);
}

// ============================================================
// Operators
// ============================================================

TEST_F(LexerTest, TokenPLUS) {
  Lexer lex(cfg);
  // "X+Y": IDENTIFIER PLUS IDENTIFIER NEWLINE EOF
  std::string src = fmtLine("X+Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::PLUS);
  EXPECT_EQ(toks[1].value, "+");
}

TEST_F(LexerTest, TokenMINUS) {
  Lexer lex(cfg);
  std::string src = fmtLine("X-Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::MINUS);
  EXPECT_EQ(toks[1].value, "-");
}

TEST_F(LexerTest, TokenSTAR) {
  Lexer lex(cfg);
  std::string src = fmtLine("X*Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::STAR);
  EXPECT_EQ(toks[1].value, "*");
}

TEST_F(LexerTest, TokenSLASH) {
  Lexer lex(cfg);
  std::string src = fmtLine("X/Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::SLASH);
  EXPECT_EQ(toks[1].value, "/");
}

TEST_F(LexerTest, TokenDSTAR) {
  Lexer lex(cfg);
  std::string src = fmtLine("X**Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::DSTAR);
  EXPECT_EQ(toks[1].value, "**");
}

TEST_F(LexerTest, TokenDSLASH) {
  Lexer lex(cfg);
  std::string src = fmtLine("X//Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::DSLASH);
  EXPECT_EQ(toks[1].value, "//");
}

// ============================================================
// Assignment
// ============================================================

TEST_F(LexerTest, TokenASSIGN) {
  Lexer lex(cfg);
  std::string src = fmtLine("X=1");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::ASSIGN);
  EXPECT_EQ(toks[1].value, "=");
}

// ============================================================
// Relational operators
// ============================================================

TEST_F(LexerTest, TokenEQ) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .EQ. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::EQ);
  EXPECT_EQ(toks[1].value, ".EQ.");
}

TEST_F(LexerTest, TokenNE) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .NE. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::NE);
  EXPECT_EQ(toks[1].value, ".NE.");
}

TEST_F(LexerTest, TokenLT) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .LT. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::LT);
  EXPECT_EQ(toks[1].value, ".LT.");
}

TEST_F(LexerTest, TokenLE) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .LE. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::LE);
  EXPECT_EQ(toks[1].value, ".LE.");
}

TEST_F(LexerTest, TokenGT) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .GT. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::GT);
  EXPECT_EQ(toks[1].value, ".GT.");
}

TEST_F(LexerTest, TokenGE) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .GE. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::GE);
  EXPECT_EQ(toks[1].value, ".GE.");
}

// ============================================================
// Logical operators
// ============================================================

TEST_F(LexerTest, TokenAND) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .AND. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::AND);
  EXPECT_EQ(toks[1].value, ".AND.");
}

TEST_F(LexerTest, TokenOR) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .OR. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::OR);
  EXPECT_EQ(toks[1].value, ".OR.");
}

TEST_F(LexerTest, TokenNOT) {
  Lexer lex(cfg);
  // ".NOT. X": NOT IDENTIFIER NEWLINE EOF
  std::string src = fmtLine(".NOT. X");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 2u);
  EXPECT_EQ(toks[0].type, TT::NOT);
  EXPECT_EQ(toks[0].value, ".NOT.");
}

TEST_F(LexerTest, TokenEQV) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .EQV. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::EQV);
  EXPECT_EQ(toks[1].value, ".EQV.");
}

TEST_F(LexerTest, TokenNEQV) {
  Lexer lex(cfg);
  std::string src = fmtLine("X .NEQV. Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::NEQV);
  EXPECT_EQ(toks[1].value, ".NEQV.");
}

// ============================================================
// Logical literals
// ============================================================

TEST_F(LexerTest, TokenTRUE) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = .TRUE.");
  auto toks = lex.tokenize(src);
  // IDENTIFIER ASSIGN TRUE NEWLINE EOF
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::TRUE);
  EXPECT_EQ(toks[2].value, ".TRUE.");
}

TEST_F(LexerTest, TokenFALSE) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = .FALSE.");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::FALSE);
  EXPECT_EQ(toks[2].value, ".FALSE.");
}

// ============================================================
// Punctuation
// ============================================================

TEST_F(LexerTest, TokenLPAREN) {
  Lexer lex(cfg);
  // "(X)": LPAREN IDENTIFIER RPAREN (not a complex literal; X is not a number)
  std::string src = fmtLine("(X)");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[0].type, TT::LPAREN);
  EXPECT_EQ(toks[0].value, "(");
}

TEST_F(LexerTest, TokenRPAREN) {
  Lexer lex(cfg);
  std::string src = fmtLine("(X)");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::RPAREN);
  EXPECT_EQ(toks[2].value, ")");
}

TEST_F(LexerTest, TokenCOMMA) {
  Lexer lex(cfg);
  // "X,Y": IDENTIFIER COMMA IDENTIFIER NEWLINE EOF
  std::string src = fmtLine("X,Y");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::COMMA);
  EXPECT_EQ(toks[1].value, ",");
}

TEST_F(LexerTest, TokenCOLON) {
  Lexer lex(cfg);
  // "X(1:5)": IDENTIFIER LPAREN INT_LITERAL COLON INT_LITERAL RPAREN NEWLINE EOF
  std::string src = fmtLine("X(1:5)");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 5u);
  EXPECT_EQ(toks[3].type, TT::COLON);
  EXPECT_EQ(toks[3].value, ":");
}

TEST_F(LexerTest, TokenSEMICOLON) {
  Lexer lex(cfg);
  // "X=1;Y=2": IDENTIFIER ASSIGN INT_LITERAL SEMICOLON ...
  std::string src = fmtLine("X=1;Y=2");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 4u);
  EXPECT_EQ(toks[3].type, TT::SEMICOLON);
  EXPECT_EQ(toks[3].value, ";");
}

// Note: SQUOTE and DQUOTE are defined in TT but are not produced by the lexer;
// quoted strings are always emitted as CHR_LITERAL regardless of delimiter.

// ============================================================
// Literals
// ============================================================

TEST_F(LexerTest, TokenINT_LITERAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = 42");
  auto toks = lex.tokenize(src);
  // IDENTIFIER ASSIGN INT_LITERAL NEWLINE EOF
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::INT_LITERAL);
  EXPECT_EQ(toks[2].value, "42");
}

TEST_F(LexerTest, TokenDBL_LITERAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = 1.5D10");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::DBL_LITERAL);
  EXPECT_EQ(toks[2].value, "1.5D10");
}

TEST_F(LexerTest, TokenRL_LITERAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = 3.14");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::RL_LITERAL);
  EXPECT_EQ(toks[2].value, "3.14");
}

TEST_F(LexerTest, TokenCMPX_LITERAL) {
  Lexer lex(cfg);
  // "(1.5, -2.0)" is a single CMPX_LITERAL token
  std::string src = fmtLine("X = (1.5, -2.0)");
  auto toks = lex.tokenize(src);
  // IDENTIFIER ASSIGN CMPX_LITERAL NEWLINE EOF
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::CMPX_LITERAL);
  EXPECT_EQ(toks[2].value, "(1.5, -2.0)");
}

TEST_F(LexerTest, TokenCHR_LITERAL) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = 'HELLO'");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::CHR_LITERAL);
  EXPECT_EQ(toks[2].value, "'HELLO'");
}

// ============================================================
// Structural
// ============================================================

TEST_F(LexerTest, TokenLABEL) {
  Lexer lex(cfg);
  // "100   STOP": cols 0-2 = "100", cols 3-5 = spaces, cols 6+ = "STOP"
  std::string src = fmtLabel("100", "STOP");
  auto toks = lex.tokenize(src);
  // LABEL STOP NEWLINE EOF
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[0].type, TT::LABEL);
  EXPECT_EQ(toks[0].value, "100");
  EXPECT_EQ(toks[1].type, TT::STOP);
}

TEST_F(LexerTest, TokenIDENTIFIER) {
  Lexer lex(cfg);
  std::string src = fmtLine("MYVAR");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 2u);
  EXPECT_EQ(toks[0].type, TT::IDENTIFIER);
  EXPECT_EQ(toks[0].value, "MYVAR");
}

TEST_F(LexerTest, TokenNEWLINE) {
  Lexer lex(cfg);
  // Any statement ends with a NEWLINE before EOF
  std::string src = fmtLine("X");
  auto toks = lex.tokenize(src);
  // IDENTIFIER NEWLINE EOF
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[1].type, TT::NEWLINE);
}

TEST_F(LexerTest, TokenEND_OF_FILE) {
  Lexer lex(cfg);
  std::string src = fmtLine("X");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 1u);
  EXPECT_EQ(toks.back().type, TT::END_OF_FILE);
}

TEST_F(LexerTest, TokenUNKNOWN) {
  Lexer lex(cfg);
  // "@#" are both unrecognized; greedy merging produces one UNKNOWN token
  std::string src = fmtLine("@#");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 2u);
  EXPECT_EQ(toks[0].type, TT::UNKNOWN);
  EXPECT_EQ(toks[0].value, "@#");
}

// ============================================================
// Additional behavior tests
// ============================================================

TEST_F(LexerTest, UnknownGreedyMerge) {
  Lexer lex(cfg);
  // Multiple consecutive unknown characters → single UNKNOWN token
  // '@' is unknown; '!' ends the comment scan; '@%' should merge into one token
  std::string src = fmtLine("@%");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 2u);
  EXPECT_EQ(toks[0].type, TT::UNKNOWN);
  EXPECT_EQ(toks[0].value, "@%");
}

TEST_F(LexerTest, OobErrorThrows) {
  Lexer lex(cfg);
  // 6 spaces + 66 'X's + 'Y' = 73 chars; index 72 ('Y') > SCMaxCol(71) → oob_error
  std::string src = "      " + std::string(66, 'X') + "Y\n";
  EXPECT_THROW(lex.tokenize(src), oob_error);
}

TEST_F(LexerTest, OobNoThrowTrailingSpace) {
  Lexer lex(cfg);
  // Beyond SCMaxCol but only spaces → no exception
  std::string src = "      " + std::string(66, 'X') + " \n";
  EXPECT_NO_THROW(lex.tokenize(src));
}

TEST_F(LexerTest, CommentLineCSkipped) {
  Lexer lex(cfg);
  // Comment line (starts with 'C') produces no tokens except from other lines
  std::string src = "C THIS IS A COMMENT\n      X\n";
  auto toks = lex.tokenize(src);
  // Only X line tokens: IDENTIFIER NEWLINE EOF
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[0].type, TT::IDENTIFIER);
  EXPECT_EQ(toks[0].value, "X");
}

TEST_F(LexerTest, CommentLineLowercaseCSkipped) {
  Lexer lex(cfg);
  std::string src = "c another comment\n      Y\n";
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::IDENTIFIER);
  EXPECT_EQ(toks[0].value, "Y");
}

TEST_F(LexerTest, CommentLineStarSkipped) {
  Lexer lex(cfg);
  std::string src = "* star comment\n      Z\n";
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::IDENTIFIER);
  EXPECT_EQ(toks[0].value, "Z");
}

TEST_F(LexerTest, InlineCommentStripped) {
  Lexer lex(cfg);
  // "X = 1 ! this is inline comment" → only X, =, 1 are tokenized
  std::string src = fmtLine("X = 1 ! ignored");
  auto toks = lex.tokenize(src);
  // IDENTIFIER ASSIGN INT_LITERAL NEWLINE EOF
  ASSERT_GE(toks.size(), 4u);
  EXPECT_EQ(toks[0].type, TT::IDENTIFIER);
  EXPECT_EQ(toks[1].type, TT::ASSIGN);
  EXPECT_EQ(toks[2].type, TT::INT_LITERAL);
  EXPECT_EQ(toks[3].type, TT::NEWLINE);
}

TEST_F(LexerTest, CaseInsensitiveKeyword) {
  Lexer lex(cfg);
  // "integer" (lowercase) → TT::INTEGER, value preserves original case
  std::string src = fmtLine("integer");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 2u);
  EXPECT_EQ(toks[0].type, TT::INTEGER);
  EXPECT_EQ(toks[0].value, "integer");
}

TEST_F(LexerTest, CaseInsensitiveMixed) {
  Lexer lex(cfg);
  std::string src = fmtLine("InTeGeR");
  auto toks = lex.tokenize(src);
  EXPECT_EQ(toks[0].type, TT::INTEGER);
  EXPECT_EQ(toks[0].value, "InTeGeR");
}

TEST_F(LexerTest, ContinuationLineSharedLlno) {
  Lexer lex(cfg);
  // Line 1: code, no continuation
  // Line 2: '+' in col 5 → continuation of line 1
  std::string src = fmtLine("X = 1 +") + "     +2\n";
  auto toks = lex.tokenize(src);
  // All tokens from both physical lines share llno=0
  ASSERT_GE(toks.size(), 6u);
  for (std::size_t i = 0; i < toks.size() - 1; i++) {
    if (toks[i].type == TT::NEWLINE) continue;
    EXPECT_EQ(toks[i].llno, 0) << "token " << i << " has wrong llno";
  }
  // Physical line numbers differ: X is on lno=0, INT_LITERAL("2") on lno=1
  EXPECT_EQ(toks[0].lno, 0);
  bool found_2 = false;
  for (auto& t : toks) {
    if (t.type == TT::INT_LITERAL && t.value == "2") {
      EXPECT_EQ(t.lno, 1);
      EXPECT_EQ(t.llno, 0);
      found_2 = true;
    }
  }
  EXPECT_TRUE(found_2);
}

TEST_F(LexerTest, MultipleStatementsLlnoIncrements) {
  Lexer lex(cfg);
  // Two separate lines → llno increments after each statement
  std::string src = fmtLine("X = 1") + fmtLine("Y = 2");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 8u);
  EXPECT_EQ(toks[0].llno, 0); // X
  bool found_y = false;
  for (auto& t : toks) {
    if (t.type == TT::IDENTIFIER && t.value == "Y") {
      EXPECT_EQ(t.llno, 1);
      found_y = true;
    }
  }
  EXPECT_TRUE(found_y);
}

TEST_F(LexerTest, LabelOnNonContinuationOnly) {
  Lexer lex(cfg);
  // Continuation line has digits in cols 0-4 but must NOT emit LABEL
  std::string src = fmtLabel("10", "X =") + "99999+2\n";
  auto toks = lex.tokenize(src);
  int label_count = 0;
  for (auto& t : toks)
    if (t.type == TT::LABEL) label_count++;
  EXPECT_EQ(label_count, 1);
  EXPECT_EQ(toks[0].type, TT::LABEL);
  EXPECT_EQ(toks[0].value, "10");
}

TEST_F(LexerTest, IntLiteralDisambiguationFromDotOp) {
  Lexer lex(cfg);
  // "3.GT.0" → INT_LITERAL("3") GT INT_LITERAL("0"), not RL_LITERAL
  std::string src = fmtLine("3.GT.0");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[0].type, TT::INT_LITERAL);
  EXPECT_EQ(toks[0].value, "3");
  EXPECT_EQ(toks[1].type, TT::GT);
  EXPECT_EQ(toks[2].type, TT::INT_LITERAL);
  EXPECT_EQ(toks[2].value, "0");
}

TEST_F(LexerTest, RealLiteralLeadingDot) {
  Lexer lex(cfg);
  // ".5" → RL_LITERAL
  std::string src = fmtLine("X = .5");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::RL_LITERAL);
  EXPECT_EQ(toks[2].value, ".5");
}

TEST_F(LexerTest, DoubleLiteralLowercaseD) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = 1.5d-3");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::DBL_LITERAL);
  EXPECT_EQ(toks[2].value, "1.5d-3");
}

TEST_F(LexerTest, RealLiteralEExponent) {
  Lexer lex(cfg);
  std::string src = fmtLine("X = 2.5E+4");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::RL_LITERAL);
  EXPECT_EQ(toks[2].value, "2.5E+4");
}

TEST_F(LexerTest, ComplexLiteralSignedComponents) {
  Lexer lex(cfg);
  // "(-1.0, +2.5D3)" → single CMPX_LITERAL
  std::string src = fmtLine("X = (-1.0, +2.5D3)");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::CMPX_LITERAL);
  EXPECT_EQ(toks[2].value, "(-1.0, +2.5D3)");
}

TEST_F(LexerTest, ParenExprNotComplexLiteral) {
  Lexer lex(cfg);
  // "(X + Y)" → LPAREN, not CMPX_LITERAL, because X is not a number
  std::string src = fmtLine("(X + Y)");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 1u);
  EXPECT_EQ(toks[0].type, TT::LPAREN);
}

TEST_F(LexerTest, ChrLiteralDoubleQuote) {
  Lexer lex(cfg);
  // Double-quoted string
  std::string src = fmtLine("X = \"WORLD\"");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::CHR_LITERAL);
  EXPECT_EQ(toks[2].value, "\"WORLD\"");
}

TEST_F(LexerTest, ChrLiteralEscapedQuote) {
  Lexer lex(cfg);
  // Doubled single quote inside string: 'IT''S' → CHR_LITERAL with escaped quote
  std::string src = fmtLine("X = 'IT''S'");
  auto toks = lex.tokenize(src);
  ASSERT_GE(toks.size(), 3u);
  EXPECT_EQ(toks[2].type, TT::CHR_LITERAL);
  EXPECT_EQ(toks[2].value, "'IT''S'");
}

TEST_F(LexerTest, EmptySourceEOF) {
  Lexer lex(cfg);
  std::string src;
  auto toks = lex.tokenize(src);
  ASSERT_EQ(toks.size(), 1u);
  EXPECT_EQ(toks[0].type, TT::END_OF_FILE);
}

TEST_F(LexerTest, StartLnoOffsets) {
  Lexer lex(cfg);
  std::string src = fmtLine("X");
  auto toks = lex.tokenize(src, 5);
  // lno and llno of first token should be 5
  ASSERT_GE(toks.size(), 1u);
  EXPECT_EQ(toks[0].lno, 5);
  EXPECT_EQ(toks[0].llno, 5);
}
