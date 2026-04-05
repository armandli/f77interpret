#include <lexer.h>

#include <algorithm>
#include <cctype>
#include <unordered_map>

#include <exception.h>

namespace f77i {

namespace {

const s::unordered_map<s::string, TT> kKeywords = {
  {"integer",     TT::INTEGER},
  {"real",        TT::REAL},
  {"double",      TT::DOUBLE},
  {"complex",     TT::COMPLEX},
  {"logical",     TT::LOGICAL},
  {"character",   TT::CHARACTER},
  {"if",          TT::IF},
  {"then",        TT::THEN},
  {"else",        TT::ELSE},
  {"elseif",      TT::ELSEIF},
  {"endif",       TT::ENDIF},
  {"do",          TT::DO},
  {"enddo",       TT::ENDDO},
  {"continue",    TT::CONTINUE},
  {"goto",        TT::GOTO},
  {"stop",        TT::STOP},
  {"pause",       TT::PAUSE},
  {"end",         TT::END},
  {"program",     TT::PROGRAM},
  {"subroutine",  TT::SUBROUTINE},
  {"function",    TT::FUNCTION},
  {"block",       TT::BLOCK},
  {"data",        TT::DATA},
  {"read",        TT::READ},
  {"write",       TT::WRITE},
  {"print",       TT::PRINT},
  {"type",        TT::TYPE},
  {"open",        TT::OPEN},
  {"close",       TT::CLOSE},
  {"rewind",      TT::REWIND},
  {"backspace",   TT::BACKSPACE},
  {"endfile",     TT::ENDFILE},
  {"format",      TT::FORMAT},
  {"common",      TT::COMMON},
  {"equivalence", TT::EQUIVALENCE},
  {"external",    TT::EXTERNAL},
  {"intrinsic",   TT::INTRINSIC},
  {"save",        TT::SAVE},
  {"parameter",   TT::PARAMETER},
  {"implicit",    TT::IMPLICIT},
  {"dimension",   TT::DIMENSION},
  {"none",        TT::NONE},
  {"entry",       TT::ENTRY},
  {"include",     TT::INCLUDE},
  {"call",        TT::CALL},
  {"return",      TT::RETURN},
};

const s::unordered_map<s::string, TT> kDotOps = {
  {"EQ",   TT::EQ},
  {"NE",   TT::NE},
  {"LT",   TT::LT},
  {"LE",   TT::LE},
  {"GT",   TT::GT},
  {"GE",   TT::GE},
  {"AND",  TT::AND},
  {"OR",   TT::OR},
  {"NOT",  TT::NOT},
  {"EQV",  TT::EQV},
  {"NEQV", TT::NEQV},
  {"TRUE", TT::TRUE},
  {"FALSE",TT::FALSE},
};

s::size_t codepointSize(unsigned char c, Codepoint cp) {
  if (cp != Codepoint::UTF8) return 1;
  if ((c & 0x80) == 0x00) return 1;
  if ((c & 0xE0) == 0xC0) return 2;
  if ((c & 0xF0) == 0xE0) return 3;
  if ((c & 0xF8) == 0xF0) return 4;
  return 1;
}

// Counts codepoints in a string_view according to the given encoding.
// Returns the byte position of codepoint index `col`, or sv.size() if out of range.
s::size_t byteOffsetOfCol(s::string_view sv, int col, Codepoint cp) {
  int c = 0;
  s::size_t b = 0;
  while (b < sv.size() and c < col) {
    b += codepointSize(static_cast<unsigned char>(sv[b]), cp);
    c++;
  }
  return b;
}

// Returns the number of codepoints in sv.
int codepointCount(s::string_view sv, Codepoint cp) {
  int c = 0;
  s::size_t b = 0;
  while (b < sv.size()) {
    b += codepointSize(static_cast<unsigned char>(sv[b]), cp);
    c++;
  }
  return c;
}

bool isUnknownStart(char c) {
  if (c == ' ' or c == '\t' or c == '!' or
      c == '\'' or c == '"' or c == '(' or c == ')' or
      c == ',' or c == ':' or c == ';' or c == '=' or
      c == '+' or c == '-' or c == '*' or c == '/')
    return false;
  if (static_cast<unsigned char>(c) >= 0x80) return true; // non-ASCII: unknown
  if (std::isdigit(static_cast<unsigned char>(c))) return false;
  if (std::isalpha(static_cast<unsigned char>(c))) return false;
  if (c == '_') return false;
  return true;
}

} // anonymous namespace

s::size_t Lexer::tryParseSignedNumber(s::string_view code, s::size_t i) const {
  s::size_t j = i;
  s::size_t n = code.size();

  if (j < n and (code[j] == '+' or code[j] == '-'))
    j++;

  if (j >= n)
    return i;

  bool has_digits = false;

  if (code[j] == '.'
      and j + 1 < n
      and std::isdigit(static_cast<unsigned char>(code[j + 1]))) {
    // starts with decimal point
    j++;
    while (j < n and std::isdigit(static_cast<unsigned char>(code[j]))) {
      j++;
      has_digits = true;
    }
  } else if (std::isdigit(static_cast<unsigned char>(code[j]))) {
    while (j < n and std::isdigit(static_cast<unsigned char>(code[j]))) {
      j++;
      has_digits = true;
    }
    // optional decimal point
    if (j < n and code[j] == '.') {
      char next = (j + 1 < n) ? code[j + 1] : '\0';
      if (not std::isalpha(static_cast<unsigned char>(next)) and next != '_') {
        j++; // consume '.'
        while (j < n and std::isdigit(static_cast<unsigned char>(code[j])))
          j++;
      }
    }
  } else {
    return i; // no number
  }

  if (not has_digits)
    return i;

  // optional exponent
  if (j < n and (std::tolower(static_cast<unsigned char>(code[j])) == 'e' or
                 std::tolower(static_cast<unsigned char>(code[j])) == 'd')) {
    s::size_t k = j + 1;
    if (k < n and (code[k] == '+' or code[k] == '-'))
      k++;
    if (k < n and std::isdigit(static_cast<unsigned char>(code[k]))) {
      j = k;
      while (j < n and std::isdigit(static_cast<unsigned char>(code[j])))
        j++;
    }
  }

  return j;
}

s::size_t Lexer::tryParseComplexLiteral(s::string_view code, s::size_t i) const {
  s::size_t n = code.size();
  s::size_t j = i + 1; // skip '('

  while (j < n and code[j] == ' ') j++;

  s::size_t end1 = tryParseSignedNumber(code, j);
  if (end1 == j) return i;
  j = end1;

  while (j < n and code[j] == ' ') j++;
  if (j >= n or code[j] != ',') return i;
  j++;

  while (j < n and code[j] == ' ') j++;

  s::size_t end2 = tryParseSignedNumber(code, j);
  if (end2 == j) return i;
  j = end2;

  while (j < n and code[j] == ' ') j++;
  if (j >= n or code[j] != ')') return i;
  j++;

  return j;
}

TT Lexer::parseDotToken(
    s::string_view code, s::size_t i, s::size_t& out_end) const
{
  s::size_t n = code.size();
  s::size_t j = i + 1; // skip leading '.'

  s::size_t name_start = j;
  while (j < n and std::isalpha(static_cast<unsigned char>(code[j])))
    j++;

  if (j >= n or code[j] != '.') {
    out_end = i + 1;
    return TT::UNKNOWN;
  }

  s::string name(code.substr(name_start, j - name_start));
  s::transform(name.begin(), name.end(), name.begin(),
               [](unsigned char c) { return std::toupper(c); });

  auto it = kDotOps.find(name);
  if (it != kDotOps.end()) {
    out_end = j + 1; // include closing '.'
    return it->second;
  }

  out_end = i + 1;
  return TT::UNKNOWN;
}

void Lexer::tokenizeCode(
    s::string_view code,
    int lno,
    int llno,
    s::vector<Token>& tokens) const
{
  s::size_t n = code.size();
  s::size_t i = 0;
  Codepoint cp = mConf.codepoint;

  while (i < n) {
    unsigned char c = static_cast<unsigned char>(code[i]);

    // skip whitespace
    if (c == ' ' or c == '\t') {
      i++;
      continue;
    }

    // inline comment
    if (c == '!') break;

    // opening paren: try complex literal first
    if (c == '(') {
      s::size_t end = tryParseComplexLiteral(code, i);
      if (end > i) {
        tokens.push_back(Token{
            TT::CMPX_LITERAL, lno, llno, code.substr(i, end - i)});
        i = end;
      } else {
        tokens.push_back(Token{TT::LPAREN, lno, llno, code.substr(i, 1)});
        i++;
      }
      continue;
    }

    if (c == ')') {
      tokens.push_back(Token{TT::RPAREN, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }
    if (c == ',') {
      tokens.push_back(Token{TT::COMMA, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }
    if (c == ':') {
      tokens.push_back(Token{TT::COLON, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }
    if (c == ';') {
      tokens.push_back(Token{TT::SEMICOLON, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }
    if (c == '=') {
      tokens.push_back(Token{TT::ASSIGN, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }
    if (c == '+') {
      tokens.push_back(Token{TT::PLUS, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }
    if (c == '-') {
      tokens.push_back(Token{TT::MINUS, lno, llno, code.substr(i, 1)});
      i++;
      continue;
    }

    if (c == '*') {
      if (i + 1 < n and code[i + 1] == '*') {
        tokens.push_back(Token{TT::DSTAR, lno, llno, code.substr(i, 2)});
        i += 2;
      } else {
        tokens.push_back(Token{TT::STAR, lno, llno, code.substr(i, 1)});
        i++;
      }
      continue;
    }

    if (c == '/') {
      if (i + 1 < n and code[i + 1] == '/') {
        tokens.push_back(Token{TT::DSLASH, lno, llno, code.substr(i, 2)});
        i += 2;
      } else {
        tokens.push_back(Token{TT::SLASH, lno, llno, code.substr(i, 1)});
        i++;
      }
      continue;
    }

    // dot: dotted operator or real literal starting with '.'
    if (c == '.') {
      if (i + 1 < n and std::isdigit(static_cast<unsigned char>(code[i + 1]))) {
        // real/double literal starting with '.'
        s::size_t j = i + 1;
        while (j < n and std::isdigit(static_cast<unsigned char>(code[j]))) j++;
        TT tt = TT::RL_LITERAL;
        if (j < n) {
          char ec = static_cast<char>(std::tolower(static_cast<unsigned char>(code[j])));
          if (ec == 'e' or ec == 'd') {
            s::size_t k = j + 1;
            if (k < n and (code[k] == '+' or code[k] == '-')) k++;
            if (k < n and std::isdigit(static_cast<unsigned char>(code[k]))) {
              tt = (ec == 'd') ? TT::DBL_LITERAL : TT::RL_LITERAL;
              j = k;
              while (j < n and std::isdigit(static_cast<unsigned char>(code[j]))) j++;
            }
          }
        }
        tokens.push_back(Token{tt, lno, llno, code.substr(i, j - i)});
        i = j;
      } else {
        s::size_t out_end = i + 1;
        TT tt = parseDotToken(code, i, out_end);
        tokens.push_back(Token{tt, lno, llno, code.substr(i, out_end - i)});
        i = out_end;
      }
      continue;
    }

    // string literals
    if (c == '\'' or c == '"') {
      char delim = static_cast<char>(c);
      s::size_t j = i + 1;
      while (j < n) {
        if (code[j] == delim) {
          if (j + 1 < n and code[j + 1] == delim) {
            j += 2; // doubled quote = escaped quote
          } else {
            j++;
            break;
          }
        } else {
          j++;
        }
      }
      tokens.push_back(Token{
          TT::CHR_LITERAL, lno, llno, code.substr(i, j - i)});
      i = j;
      continue;
    }

    // number literal starting with digit
    if (std::isdigit(c)) {
      s::size_t j = i;
      while (j < n and std::isdigit(static_cast<unsigned char>(code[j]))) j++;

      TT tt = TT::INT_LITERAL;

      // check for decimal point
      if (j < n and code[j] == '.') {
        char after_dot = (j + 1 < n) ? code[j + 1] : '\0';
        if (not std::isalpha(static_cast<unsigned char>(after_dot))
            and after_dot != '_') {
          // consume decimal point
          j++;
          while (j < n and std::isdigit(static_cast<unsigned char>(code[j]))) j++;
          tt = TT::RL_LITERAL;
        }
        // else: '.' starts a dotted operator, don't consume it
      }

      // check for exponent
      if (j < n) {
        char ec = static_cast<char>(std::tolower(static_cast<unsigned char>(code[j])));
        if (ec == 'e' or ec == 'd') {
          s::size_t k = j + 1;
          if (k < n and (code[k] == '+' or code[k] == '-')) k++;
          if (k < n and std::isdigit(static_cast<unsigned char>(code[k]))) {
            tt = (ec == 'd') ? TT::DBL_LITERAL : TT::RL_LITERAL;
            j = k;
            while (j < n and std::isdigit(static_cast<unsigned char>(code[j])))
              j++;
          }
          // else: no digits after exponent letter, backtrack (don't consume E/D)
        }
      }

      tokens.push_back(Token{tt, lno, llno, code.substr(i, j - i)});
      i = j;
      continue;
    }

    // identifier or keyword (ASCII only: alpha or '_')
    if (std::isalpha(c) or c == '_') {
      s::size_t j = i;
      while (j < n) {
        unsigned char ch = static_cast<unsigned char>(code[j]);
        if (std::isalnum(ch) or ch == '_' or ch == '$')
          j++;
        else
          break;
      }
      s::string_view raw = code.substr(i, j - i);
      s::string lower(raw);
      s::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char ch) { return std::tolower(ch); });
      auto it = kKeywords.find(lower);
      TT tt = (it != kKeywords.end()) ? it->second : TT::IDENTIFIER;
      tokens.push_back(Token{tt, lno, llno, raw});
      i = j;
      continue;
    }

    // unknown: greedily consume consecutive unknown codepoints
    {
      s::size_t j = i + codepointSize(c, cp);
      while (j < n and isUnknownStart(code[j]))
        j += codepointSize(static_cast<unsigned char>(code[j]), cp);
      tokens.push_back(Token{TT::UNKNOWN, lno, llno, code.substr(i, j - i)});
      i = j;
    }
  }
}

bool Lexer::isCommentLine(s::string_view line) const {
  if (line.empty()) return false;
  char c = line[0];
  return c == 'C' or c == 'c' or c == '*';
}

bool Lexer::isContinuationLine(s::string_view line) const {
  int cont_col = mConf.SCSrcCol - 1;
  Codepoint cp = mConf.codepoint;
  if (static_cast<int>(line.size()) <= cont_col) return false;
  s::size_t byte_cont = byteOffsetOfCol(line, cont_col, cp);
  if (byte_cont >= line.size()) return false;
  char ch = line[byte_cont];
  return ch != ' ' and ch != '0';
}

int Lexer::count_lines(s::string_view source_code) const {
  if (source_code.empty()) return 0;
  int count = 0;
  for (char ch : source_code) {
    if (ch == '\n') count++;
  }
  if (source_code.back() != '\n') count++;
  return count;
}

int Lexer::count_logical_lines(s::string_view source_code) const {
  s::vector<s::string_view> lines;
  {
    s::string_view sv(source_code);
    s::size_t pos = 0;
    while (pos <= sv.size()) {
      s::size_t nl = sv.find('\n', pos);
      if (nl == s::string_view::npos) {
        lines.push_back(sv.substr(pos));
        break;
      }
      lines.push_back(sv.substr(pos, nl - pos));
      pos = nl + 1;
    }
  }

  int n = static_cast<int>(lines.size());
  int logical_count = 0;
  for (int idx = 0; idx < n; idx++) {
    s::string_view line = lines[idx];
    if (line.empty()) continue;
    if (isCommentLine(line)) continue;

    bool next_is_cont = false;
    for (int j = idx + 1; j < n; j++) {
      s::string_view next_line = lines[j];
      if (next_line.empty()) continue;
      if (isCommentLine(next_line)) continue;
      next_is_cont = isContinuationLine(next_line);
      break;
    }

    if (not next_is_cont) logical_count++;
  }

  return logical_count;
}

s::vector<Token> Lexer::tokenize(s::string_view source_code, int start_lno) {
  s::vector<Token> tokens;
  Codepoint cp = mConf.codepoint;
  int src_col = mConf.SCSrcCol;
  int max_col = mConf.SCMaxCol;
  int cont_col = src_col - 1;  // continuation column index (0-indexed)
  int label_end = src_col - 1; // label field is [0, cont_col)

  // Split source into lines as string_views into source_code
  s::vector<s::string_view> lines;
  {
    s::string_view sv(source_code);
    s::size_t pos = 0;
    while (pos <= sv.size()) {
      s::size_t nl = sv.find('\n', pos);
      if (nl == s::string_view::npos) {
        lines.push_back(sv.substr(pos));
        break;
      }
      lines.push_back(sv.substr(pos, nl - pos));
      pos = nl + 1;
    }
  }

  int n = static_cast<int>(lines.size());
  int llno = start_lno;

  for (int idx = 0; idx < n; idx++) {
    s::string_view line = lines[idx];
    int lno = start_lno + idx;

    if (line.empty()) continue;

    // Comment line: skip entirely
    if (isCommentLine(line)) continue;

    // OOB check: count codepoints in this line; error if > max_col+1
    {
      int cp_count = codepointCount(line, cp);
      if (cp_count > max_col + 1) {
        // Check if content beyond max_col is non-space
        s::size_t byte_past = byteOffsetOfCol(line, max_col + 1, cp);
        bool has_content = false;
        for (s::size_t b = byte_past; b < line.size(); b++) {
          if (line[b] != ' ' and line[b] != '\t') {
            has_content = true;
            break;
          }
        }
        if (has_content) {
          throw oob_error(
              "line " + s::to_string(lno) +
              " exceeds maximum column " + s::to_string(max_col + 1));
        }
      }
    }

    bool cont = isContinuationLine(line);

    // Label field: only for non-continuation lines
    if (not cont) {
      // Label field byte range: [0, byte_offset_of(cont_col))
      s::size_t label_byte_end = byteOffsetOfCol(line, label_end, cp);
      label_byte_end = s::min(label_byte_end, line.size());
      if (label_byte_end > 0) {
        s::string_view label_field = line.substr(0, label_byte_end);
        // Check for non-space chars in label field
        s::size_t ls = label_field.find_first_not_of(' ');
        if (ls != s::string_view::npos) {
          s::size_t le = label_field.find_last_not_of(' ');
          tokens.push_back(Token{
              TT::LABEL, lno, llno,
              label_field.substr(ls, le - ls + 1)});
        }
      }
    }

    // Code field: [src_col, max_col] in codepoints
    s::size_t byte_src = byteOffsetOfCol(line, src_col, cp);
    s::size_t byte_max = byteOffsetOfCol(line, max_col + 1, cp);
    byte_max = s::min(byte_max, line.size());

    if (byte_src < line.size() and byte_src < byte_max) {
      s::string_view code = line.substr(byte_src, byte_max - byte_src);

      // Strip inline comment: find first '!' outside string literals
      {
        s::size_t ci = 0;
        while (ci < code.size()) {
          char ch = code[ci];
          if (ch == '!') {
            code = code.substr(0, ci);
            break;
          } else if (ch == '\'' or ch == '"') {
            char delim = ch;
            ci++;
            while (ci < code.size()) {
              if (code[ci] == delim) {
                if (ci + 1 < code.size() and code[ci + 1] == delim) {
                  ci += 2;
                } else {
                  ci++;
                  break;
                }
              } else {
                ci++;
              }
            }
          } else {
            ci++;
          }
        }
      }

      tokenizeCode(code, lno, llno, tokens);
    }

    // Peek ahead: is the next effective (non-comment, non-empty) line a continuation?
    bool next_is_cont = false;
    for (int j = idx + 1; j < n; j++) {
      s::string_view next_line = lines[j];
      if (next_line.empty()) continue;
      if (isCommentLine(next_line)) continue;
      next_is_cont = isContinuationLine(next_line);
      break;
    }

    if (not next_is_cont) {
      tokens.push_back(Token{TT::NEWLINE, lno, llno, s::string_view{}});
      llno++;
    }
  }

  tokens.push_back(Token{
      TT::END_OF_FILE, start_lno + n, llno, s::string_view{}});
  return tokens;
}

} // namespace f77i
