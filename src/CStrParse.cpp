#include <CStrParse.h>
#include <CStrUtil.h>

using std::string;

CStrParse::
CStrParse(const string &str) :
 auto_skip_space_(false)
{
  setString(str);
}

void
CStrParse::
setString(const string &str)
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

string
CStrParse::
getBefore() const
{
  return str_.substr(0, pos_);
}

string
CStrParse::
getAt() const
{
  return getAt(pos_);
}

string
CStrParse::
getAt(uint pos) const
{
  if (pos < len_)
    return str_.substr(pos);
  else
    return "";
}

string
CStrParse::
getAt(uint pos, uint len) const
{
  if (pos < len_ && pos + len <= len_)
    return str_.substr(pos, len);
  else
    return getAt(pos);
}

string
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

  CStrUtil::skipSpace(str_, &pos_);
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

  ++pos_;

  return true;
}

bool
CStrParse::
skipChars(uint n)
{
  if (pos_ + n > len_) {
    pos_ = len_;
    return false;
  }

  pos_ += n;

  return true;
}

bool
CStrParse::
skipString()
{
  autoSkipSpace();

  if (eof())
    return false;

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

  return true;
}

void
CStrParse::
skipToEnd()
{
  pos_ = len_;
}

bool
CStrParse::
readNonSpace(string &text)
{
  int pos = pos_;

  if (eof() || isspace(str_[pos_]))
    return false;

  CStrUtil::skipNonSpace(str_, &pos_);

  text = str_.substr(pos, pos_ - pos);

  return true;
}

bool
CStrParse::
readChar(char *c)
{
  autoSkipSpace();

  if (eof())
    return false;

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

  return true;
}

bool
CStrParse::
unreadString(const string &str)
{
  uint len = str.size();

  if (pos_ < len) {
    pos_ = 0;

    return false;
  }

  pos_ -= len;

  return true;
}

bool
CStrParse::
readNumber(double &real, int &integer, bool &is_real)
{
  autoSkipSpace();

  return CStrUtil::readNumber(str_, &pos_, real, integer, is_real);
}

bool
CStrParse::
readInteger(int *integer)
{
  autoSkipSpace();

  return CStrUtil::readInteger(str_, &pos_, integer);
}

bool
CStrParse::
readBaseInteger(int base, int *integer)
{
  autoSkipSpace();

  return CStrUtil::readBaseInteger(str_, base, &pos_, integer);
}

bool
CStrParse::
readReal(double *real)
{
  autoSkipSpace();

  uint pos1 = pos_;

  if (CStrUtil::readReal(str_, &pos1, real)) {
    pos_ = pos1;
    return true;
  }

  return false;
}

bool
CStrParse::
readString(string &str, bool strip_quotes)
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
  while (pos_ < len_ && str_[pos_] != c)
    ++pos_;

  if (eof())
    return false;

  return true;
}

bool
CStrParse::
readToChar(char c, string &text)
{
  text = "";

  while (pos_ < len_ && str_[pos_] != c)
    text += str_[pos_++];

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
readIdentifier(string &identifier)
{
  autoSkipSpace();

  return CStrUtil::readIdentifier(str_, &pos_, identifier);
}

bool
CStrParse::
readRealFormat(string &real_format)
{
  autoSkipSpace();

  return CStrUtil::readRealFormat(str_, &pos_, real_format);
}

bool
CStrParse::
readIntegerFormat(string &integer_format)
{
  autoSkipSpace();

  return CStrUtil::readIntegerFormat(str_, &pos_, integer_format);
}

bool
CStrParse::
readStringFormat(string &string_format)
{
  autoSkipSpace();

  return CStrUtil::readStringFormat(str_, &pos_, string_format);
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
isString(const string &str) const
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
isOneOf(const string &str) const
{
  autoSkipSpace();

  return (pos_ < len_ && str.find(str_[pos_]) != string::npos);
}

bool
CStrParse::
isWord(const string &str) const
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
