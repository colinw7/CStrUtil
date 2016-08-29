#include <CStrParse.h>
#include <CStrUtil.h>
#include <cassert>

CStrParse::
CStrParse(const std::string &str)
{
  setString(str);
}

void
CStrParse::
setString(const std::string &str)
{
  str_ = str;
  pos_ = 0;
  len_ = str_.size();
}

void
CStrParse::
addString(const std::string &str)
{
  str_ += str;

  len_ = str_.size();
}

void
CStrParse::
setPos(int pos)
{
  if (pos >= 0 || pos <= (int) len_)
    pos_ = pos;
}

std::string
CStrParse::
getAt() const
{
  return getAt(pos_);
}

std::string
CStrParse::
getAt(uint pos) const
{
  if (pos < len_)
    return str_.substr(pos);
  else
    return "";
}

std::string
CStrParse::
getAt(uint pos, uint len) const
{
  if (pos < len_ && pos + len <= len_)
    return str_.substr(pos, len);
  else
    return getAt(pos);
}

std::string
CStrParse::
getBefore(uint pos) const
{
  if      (pos < len_ && pos < pos_)
    return str_.substr(pos, pos_ - pos);
  else if (pos < len_)
    return str_.substr(pos);
  else
    return "";
}

std::string
CStrParse::
getAfter() const
{
  if (pos_ + 1 < len_)
    return str_.substr(pos_ + 1);
  else
    return "";
}

char
CStrParse::
getCharBefore(uint offset) const
{
  if (pos_ >= offset)
    return str_[pos_ - offset];
  else
    return '\0';
}

char
CStrParse::
getCharAt() const
{
  if (eof()) return '\0';

  return str_[pos_];
}

char
CStrParse::
getCharAfter(uint offset) const
{
  if (pos_ + offset < len_)
    return str_[pos_ + offset];
  else
    return '\0';
}

void
CStrParse::
skipSpace()
{
  if (eof()) return;

  int pos1 = pos_;

  CStrUtil::skipSpace(str_, &pos_);

  updateNL(pos1, pos_);
}

void
CStrParse::
skipNonSpace()
{
  if (eof()) return;

  CStrUtil::skipNonSpace(str_, &pos_);
}

void
CStrParse::
autoSkipSpace() const
{
  if (eof()) return;

  if (auto_skip_space_) {
    CStrParse *th = const_cast<CStrParse *>(this);

    th->skipSpace();
  }
}

bool
CStrParse::
skipChar()
{
  autoSkipSpace();

  if (eof())
    return false;

  if (str_[pos_] == '\n')
    ++lineNum_;

  ++pos_;

  return true;
}

bool
CStrParse::
skipChar(char c)
{
  autoSkipSpace();

  if (eof())
    return false;

  if (str_[pos_] != c)
    return false;

  if (str_[pos_] == '\n')
    ++lineNum_;

  ++pos_;

  return true;
}

bool
CStrParse::
skipChars(const std::string &s)
{
  return skipChars(s.size());
}

bool
CStrParse::
skipChars(uint n)
{
  if (pos_ + n > len_) {
    pos_ = len_;
    return false;
  }

  int pos1 = pos_;

  pos_ += n;

  updateNL(pos1, pos_);

  return true;
}

bool
CStrParse::
skipString()
{
  autoSkipSpace();

  if (eof())
    return false;

  int pos1 = pos_;

  if      (str_[pos_] == '\"') {
    if (! CStrUtil::skipDoubleQuotedString(str_, &pos_))
      return false;
  }
  else if (str_[pos_] == '\'') {
    if (! CStrUtil::skipSingleQuotedString(str_, &pos_))
      return false;
  }
  else
    return false;

  updateNL(pos1, pos_);

  return true;
}

void
CStrParse::
skipToEnd()
{
  int pos1 = pos_;

  pos_ = len_;

  updateNL(pos1, pos_);
}

bool
CStrParse::
readNonSpace(std::string &text)
{
  if (eof() || isspace(str_[pos_]))
    return false;

  int pos = pos_;

  skipNonSpace();

  text = str_.substr(pos, pos_ - pos);

  return true;
}

char
CStrParse::
readChar()
{
  autoSkipSpace();

  assert(! eof());

  if (str_[pos_] == '\n')
    ++lineNum_;

  return str_[pos_++];
}

bool
CStrParse::
readChar(char *c)
{
  autoSkipSpace();

  if (eof())
    return false;

  if (str_[pos_] == '\n')
    ++lineNum_;

  *c = str_[pos_++];

  return true;
}

bool
CStrParse::
unreadChar()
{
  if (pos_ == 0)
    return false;

  --pos_;

  if (str_[pos_] == '\n')
    --lineNum_;

  return true;
}

bool
CStrParse::
unreadString(const std::string &str)
{
  uint len = str.size();

  int pos1 = pos_;

  if (pos_ < len) {
    pos_ = 0;

    updateNL(pos_, pos1, -1);

    return false;
  }
  else {
    pos_ -= len;

    updateNL(pos_, pos1, -1);

    return true;
  }
}

bool
CStrParse::
readNumber(double &real, int &integer, bool &is_real)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readNumber(str_, &pos_, real, integer, is_real);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
readInteger(int *integer)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readInteger(str_, &pos_, integer);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
readBaseInteger(int base, int *integer)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readBaseInteger(str_, base, &pos_, integer);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
readReal(double *real)
{
  autoSkipSpace();

  uint pos1 = pos_;

  if (! CStrUtil::readReal(str_, &pos1, real))
    return false;

  pos_ = pos1;

  updateNL(pos1, pos_);

  return true;

}

bool
CStrParse::
readString(std::string &str, bool strip_quotes)
{
  autoSkipSpace();

  int pos = pos_;

  if (! skipString())
    return false;

  if (! strip_quotes)
    str = str_.substr(pos, pos_ - pos);
  else
    str = str_.substr(pos + 1, pos_ - pos - 2);

  return true;
}

bool
CStrParse::
skipToChar(char c)
{
  while (pos_ < len_ && str_[pos_] != c) {
    if (str_[pos_] == '\n')
      ++lineNum_;

    ++pos_;
  }

  if (eof())
    return false;

  return true;
}

bool
CStrParse::
readToChar(char c, std::string &text)
{
  text = "";

  while (pos_ < len_ && str_[pos_] != c) {
    if (str_[pos_] == '\n')
      ++lineNum_;

    text += str_[pos_++];
  }

  if (eof())
    return false;

  return true;
}

bool
CStrParse::
isIdentifier(int offset)
{
  return CStrUtil::isIdentifier(str_, pos_ + offset);
}

bool
CStrParse::
readIdentifier(std::string &identifier)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readIdentifier(str_, &pos_, identifier);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
readRealFormat(std::string &real_format)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readRealFormat(str_, &pos_, real_format);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
readIntegerFormat(std::string &integer_format)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readIntegerFormat(str_, &pos_, integer_format);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
readStringFormat(std::string &string_format)
{
  autoSkipSpace();

  int pos1 = pos_;

  bool b = CStrUtil::readStringFormat(str_, &pos_, string_format);

  updateNL(pos1, pos_);

  return b;
}

bool
CStrParse::
isSpace() const
{
  if (eof()) return false;

  return (pos_ < len_ && isspace(str_[pos_]));
}


bool
CStrParse::
isLower() const
{
  autoSkipSpace();

  return (pos_ < len_ && isalpha(str_[pos_]) && isupper(str_[pos_]));
}

bool
CStrParse::
isUpper() const
{
  autoSkipSpace();

  return (pos_ < len_ && isalpha(str_[pos_]) && isupper(str_[pos_]));
}

bool
CStrParse::
isAlpha() const
{
  autoSkipSpace();

  return (pos_ < len_ && isalpha(str_[pos_]));
}

bool
CStrParse::
isAlnum() const
{
  autoSkipSpace();

  return (pos_ < len_ && isalnum(str_[pos_]));
}

bool
CStrParse::
isDigit() const
{
  autoSkipSpace();

  return (pos_ < len_ && isdigit(str_[pos_]));
}

bool
CStrParse::
isDigitAt(int offset) const
{
  return (pos_ + offset < len_ && isdigit(str_[pos_ + offset]));
}

bool
CStrParse::
isODigit() const
{
  autoSkipSpace();

  return (pos_ < len_ && CStrUtil::isodigit(str_[pos_]));
}

bool
CStrParse::
isXDigit() const
{
  autoSkipSpace();

  return (pos_ < len_ && CStrUtil::isxdigit(str_[pos_]));
}

bool
CStrParse::
isCharBefore(char c, uint offset) const
{
  return (getCharBefore(offset) == c);
}

bool
CStrParse::
isChar(char c) const
{
  return (getCharAt() == c);
}

bool
CStrParse::
isCharAfter(char c, uint offset) const
{
  return (getCharAfter(offset) == c);
}

bool
CStrParse::
isNextChar(char c) const
{
  return (getCharAfter(1) == c);
}

bool
CStrParse::
isString(const std::string &str) const
{
  int len = str.size();

  if (pos_ + len > len_)
    return false;

  for (int i = 0; i < len; ++i)
    if (str_[pos_ + i] != str[i])
      return false;

  return true;
}

bool
CStrParse::
isOneOf(const std::string &str) const
{
  autoSkipSpace();

  return (pos_ < len_ && str.find(str_[pos_]) != std::string::npos);
}

bool
CStrParse::
isWord(const std::string &str) const
{
  int len = str.size();

  if (pos_ + len > len_)
    return false;

  for (int i = 0; i < len; ++i)
    if (str_[pos_ + i] != str[i])
      return false;

  if (pos_ + len <= len_ && isalnum(str_[pos_ + len]))
    return false;

  return true;
}

bool
CStrParse::
eof() const
{
  return (pos_ >= len_);
}

void
CStrParse::
updateNL(int pos1, int pos2, int d)
{
  assert(pos1 <= pos2);

  int n = 0;

  for (int pos = pos1; pos < pos2; ++pos) {
    if (str_[pos] == '\n')
      ++n;
  }

  if (n) {
    n *= d;

    lineNum_ += d;
  }
}
