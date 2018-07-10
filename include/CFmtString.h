#ifndef CFMT_STRING_H
#define CFMT_STRING_H

#include <CStrFmt.h>

class CFmtString {
 public:
  CFmtString(const std::string &str, int width=80,
             CStrFmt::AlignType align=CStrFmt::AlignType::LEFT);
 ~CFmtString();

  void setString(const std::string &str);
  void setWidth(int width);
  void setAlign(CStrFmt::AlignType align);

  int getNumLines() const { return lines_.size(); }

  const std::string &getLine(int i) const { return lines_[i]; }

 private:
  void format();

 private:
  std::string              str_;
  int                      width_;
  CStrFmt::AlignType       align_;
  std::vector<std::string> lines_;
};

#endif
