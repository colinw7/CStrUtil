#ifndef CStrParse_H
#define CStrParse_H

#include <sys/types.h>

#include <map>
#include <string>

class CStrParse {
 public:
  explicit CStrParse(const std::string &str="");

  virtual ~CStrParse() { }

  bool isAutoSkipSpace() const { return autoSkipSpace_; }
  void setAutoSkipSpace(bool autoSkipSpace=true) { autoSkipSpace_ = autoSkipSpace; }

  virtual void setString(const std::string &str);

  virtual const std::string &getString() const { return str_; }

  virtual void addString(const std::string &str);

  virtual uint getPos() const { return pos_; }

  virtual std::string::size_type getLen() const { return len_; }

  virtual void setPos(int pos);

  int lineNum() const { return lineNum_; }
  int linePos() const { return linePos_; }

  // get string from current position to end
  virtual std::string getAt() const;

  // get string from position to end
  virtual std::string getAt(uint pos) const;

  // get string from position of specified number of characters
  virtual std::string getAt(uint pos, uint len) const;

  // get string from specified pos to current pos
  virtual std::string getBefore(uint pos=0) const;

  // get string after specified pos
  virtual std::string getAfter() const;

  virtual char getCharBefore(uint offset = 1) const;
  virtual char getCharAt    () const;
  virtual char getCharAt    (int pos) const;
  virtual char getCharAfter (uint offset = 1) const;

  virtual int skipSpace();
  virtual int skipNonSpace();

  virtual bool skipChar();
  virtual bool skipChar(char c);
  virtual bool skipChars(const std::string &s);
  virtual bool skipChars(uint n);
  virtual bool skipString();
  virtual void skipToEnd();

  virtual bool skipLastString() { return skipChars(lastStringLen_); }

  virtual void autoSkipSpace() const;

  virtual bool readNonSpace(std::string &text);

  virtual bool skipBracedString();
  virtual bool readBracedString(std::string &text, bool includeBraces=false);

  virtual bool readChar(char *c);
  virtual char readChar();
  virtual bool unreadChar();

  virtual bool readUtf8Char(ulong *c);

  virtual bool unreadString(const std::string &str);

  virtual bool readNumber(double &real, int &integer, bool &is_real);

  virtual bool readInteger(int *integer);
  virtual bool readInteger(long *integer);
  virtual bool readBaseInteger(int base, int *integer);
  virtual bool readBaseInteger(int base, long *integer);
  virtual bool readReal(double *real);
  virtual bool readString(std::string &str, bool strip_quotes=false);

  virtual bool skipToChar(char c);
  virtual bool readToChar(char c, std::string &text);

  virtual bool isIdentifier(int offset=0);
  virtual bool readIdentifier(std::string &identifier);

  virtual bool readRealFormat(std::string &real_format);
  virtual bool readIntegerFormat(std::string &integer_format);
  virtual bool readStringFormat(std::string &string_format);

  virtual bool isSpace() const;
  virtual bool isNewline() const;

  virtual bool isAlpha() const;
  virtual bool isLower() const;
  virtual bool isUpper() const;
  virtual bool isAlnum() const;
  virtual bool isDigit() const;
  virtual bool isDigitAt(int offset) const;
  virtual bool isODigit() const;
  virtual bool isXDigit() const;

  virtual bool isCharBefore(char c, uint offset = 1) const;
  virtual bool isChar(char c) const;
  virtual bool isCharAfter(char c, uint offset = 1) const;

  virtual bool isNextChar(char c) const;

  virtual bool isString(const std::string &str) const;
  virtual bool isStringEnd(const std::string &str) const;
  virtual bool isOneOf(const std::string &str) const;
  virtual bool isWord(const std::string &str) const;

  virtual bool eof() const;

  virtual bool neof(int n) const;

  virtual void setEof();

  virtual std::string stateStr() const {
    return getBefore() + getCharAt() + "\b_" + getAfter();
  }

  void print() const;

 private:
  using LineLen = std::map<int, int>;

  std::string            str_;
  uint                   pos_ { 0 };
  std::string::size_type len_ { 0 };
  bool                   autoSkipSpace_ { false };
  int                    lineNum_ { 1 };
  int                    linePos_ { 0 };
  LineLen                lineLen_;

  mutable uint lastStringLen_ { 0 };
};

#endif
