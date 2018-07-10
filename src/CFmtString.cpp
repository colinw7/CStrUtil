#include <CFmtString.h>

CFmtString::
CFmtString(const std::string &str, int width, CStrFmt::AlignType align) :
 str_(str), width_(width), align_(align)
{
  format();
}

CFmtString::
~CFmtString()
{
}

void
CFmtString::
setString(const std::string &str)
{
  if (str == str_)
    return;

  str_ = str;

  format();
}

void
CFmtString::
setWidth(int width)
{
  if (width == width_)
    return;

  width_ = width;

  format();
}

void
CFmtString::
setAlign(CStrFmt::AlignType align)
{
  if (align == align_)
    return;

  align_ = align;

  format();
}

void
CFmtString::
format()
{
  lines_ = CStrFmt::format(str_, width_, align_);
}
