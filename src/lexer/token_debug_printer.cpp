#include <token_debug_printer.h>

#include <iostream>

namespace f77i {

namespace {

const char* tt_name(TT type) {
  switch (type) {
    case TT::INTEGER:    return "INTEGER";
    case TT::REAL:       return "REAL";
    case TT::DOUBLE:     return "DOUBLE";
    case TT::COMPLEX:    return "COMPLEX";
    case TT::LOGICAL:    return "LOGICAL";
    case TT::CHARACTER:  return "CHARACTER";
    case TT::IF:         return "IF";
    case TT::THEN:       return "THEN";
    case TT::ELSE:       return "ELSE";
    case TT::ELSEIF:     return "ELSEIF";
    case TT::ENDIF:      return "ENDIF";
    case TT::DO:         return "DO";
    case TT::ENDDO:      return "ENDDO";
    case TT::CONTINUE:   return "CONTINUE";
    case TT::GOTO:       return "GOTO";
    case TT::STOP:       return "STOP";
    case TT::PAUSE:      return "PAUSE";
    case TT::END:        return "END";
    case TT::PROGRAM:    return "PROGRAM";
    case TT::SUBROUTINE: return "SUBROUTINE";
    case TT::FUNCTION:   return "FUNCTION";
    case TT::BLOCK:      return "BLOCK";
    case TT::DATA:       return "DATA";
    case TT::READ:       return "READ";
    case TT::WRITE:      return "WRITE";
    case TT::PRINT:      return "PRINT";
    case TT::TYPE:       return "TYPE";
    case TT::OPEN:       return "OPEN";
    case TT::CLOSE:      return "CLOSE";
    case TT::REWIND:     return "REWIND";
    case TT::BACKSPACE:  return "BACKSPACE";
    case TT::ENDFILE:    return "ENDFILE";
    case TT::FORMAT:     return "FORMAT";
    case TT::COMMON:     return "COMMON";
    case TT::EQUIVALENCE: return "EQUIVALENCE";
    case TT::EXTERNAL:   return "EXTERNAL";
    case TT::INTRINSIC:  return "INTRINSIC";
    case TT::SAVE:       return "SAVE";
    case TT::PARAMETER:  return "PARAMETER";
    case TT::IMPLICIT:   return "IMPLICIT";
    case TT::DIMENSION:  return "DIMENSION";
    case TT::NONE:       return "NONE";
    case TT::ENTRY:      return "ENTRY";
    case TT::INCLUDE:    return "INCLUDE";
    case TT::CALL:       return "CALL";
    case TT::RETURN:     return "RETURN";
    case TT::PLUS:       return "PLUS";
    case TT::MINUS:      return "MINUS";
    case TT::STAR:       return "STAR";
    case TT::SLASH:      return "SLASH";
    case TT::DSTAR:      return "DSTAR";
    case TT::DSLASH:     return "DSLASH";
    case TT::ASSIGN:     return "ASSIGN";
    case TT::EQ:         return "EQ";
    case TT::NE:         return "NE";
    case TT::LT:         return "LT";
    case TT::LE:         return "LE";
    case TT::GT:         return "GT";
    case TT::GE:         return "GE";
    case TT::AND:        return "AND";
    case TT::OR:         return "OR";
    case TT::NOT:        return "NOT";
    case TT::EQV:        return "EQV";
    case TT::NEQV:       return "NEQV";
    case TT::TRUE:       return "TRUE";
    case TT::FALSE:      return "FALSE";
    case TT::LPAREN:     return "LPAREN";
    case TT::RPAREN:     return "RPAREN";
    case TT::COMMA:      return "COMMA";
    case TT::COLON:      return "COLON";
    case TT::SEMICOLON:  return "SEMICOLON";
    case TT::SQUOTE:     return "SQUOTE";
    case TT::DQUOTE:     return "DQUOTE";
    case TT::INT_LITERAL:  return "INT_LITERAL";
    case TT::DBL_LITERAL:  return "DBL_LITERAL";
    case TT::RL_LITERAL:   return "RL_LITERAL";
    case TT::CMPX_LITERAL: return "CMPX_LITERAL";
    case TT::CHR_LITERAL:  return "CHR_LITERAL";
    case TT::LABEL:        return "LABEL";
    case TT::IDENTIFIER:   return "IDENTIFIER";
    case TT::NEWLINE:      return "NEWLINE";
    case TT::END_OF_FILE:  return "END_OF_FILE";
    case TT::UNKNOWN:      return "UNKNOWN";
  }
  return "UNKNOWN";
}

} // namespace

void TokenDebugPrinter::print(const Token& token) const {
  s::cout << "Token { type=" << tt_name(token.type)
          << ", lno=" << token.lno
          << ", llno=" << token.llno
          << ", value=\"" << token.value << "\" }\n";
}

} // namespace f77i
