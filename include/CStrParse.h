#ifndef CStrParse_H
#define CStrParse_H

#include <sys/types.h>

#include <string>

class CStrParse {
 public:
  CStrParse(const std::string &str="");

  virtual ~CStrParse() { }

  void setAutoSkipSpace(bool auto_skip_space=true) {
    auto_skip_space_ = auto_skip_space;
  }

  virtual void setString(const std::string &str);

  virtual const std::string &getString() const { return str_; }

  virtual void addString(const std::string &str);

  virtual int getPos() const { return pos_; }

  virtual std::string::size_type getLen() const { return len_; }

  virtual void setPos(int pos);

  virtual std::string getBefore() const;
  virtual std::string getAt    () const;
  virtual std::string getAt    (uint pos) const;
  virtual std::string getAt    (uint pos, uint len) const;
  virtual std::string getAfter () const;

  virtual char getCharBefore(uint offset = 1) const;
  virtual char getCharAt    () const;
  virtual char getCharAfter (uint offset = 1) const;

  virtual void skipSpace();
  virtual void skipNonSpace();
  virtual bool skipChar();
  virtual bool skipChar(char c);
  virtual bool skipChars(uint n);
  virtual bool skipString();
  virtual void skipToEnd();

  virtual void autoSkipSpace() const;

  virtual bool readNonSpace(std::string &text);

  virtual bool readChar(char *c);
  virtual bool unreadChar();

  virtual bool unreadString(const std::string &str);

  virtual bool readNumber(double &real, int &integer, bool &is_real);

  virtual bool readInteger(int *integer);
  virtual bool readBaseInteger(int base, int *integer);
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

  virtual bool isAlpha() const;
  virtual bool isLower() const;
  virtual bool isUpper() const;
  virtual bool isAlnum() const;
  virtual bool isDigit() const;
  virtual bool isODigit() const;
  virtual bool isXDigit() const;

  virtual bool isCharBefore(char c, uint offset = 1) const;
  virtual bool isChar(char c) const;
  virtual bool isCharAfter(char c, uint offset = 1) const;

  virtual bool isNextChar(char c) const;

  virtual bool isString(const std::string &str) const;
  virtual bool isOneOf(const std::string &str) const;
  virtual bool isWord(const std::string &str) const;

  virtual bool eof() const;

 private:
  std::string            str_;
  uint                   pos_;
  std::string::size_type len_;
  bool                   auto_skip_space_;
};

#endif
