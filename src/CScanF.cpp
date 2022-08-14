#include <CScanF.h>
#include <CStrUtil.h>

#include <set>
#include <iostream>
#include <cassert>

CScanF::
CScanF(const std::string &format) :
 format_(format)
{
}

bool
CScanF::
scan(const std::string &str)
{
  ns_ = 0;

  size_t fpos = 0;
  auto   flen = format_.size();

  size_t spos = 0;
  auto   slen = str.size();

  while (fpos < flen) {
    // match any amount of white space
    if      (isspace(format_[fpos])) {
      while (fpos < flen && isspace(format_[fpos]))
        ++fpos;

      while (spos < slen && isspace(str[spos]))
        ++spos;
    }
    else if (format_[fpos] == '%') {
      ++fpos;

      if (fpos >= flen) {
        if (spos >= slen || format_[fpos - 1] != str[spos])
          return false;

        break;
      }

      if (format_[fpos] == '%') {
        while (spos < slen && isspace(str[spos]))
          ++spos;

        if (spos >= slen || format_[fpos] != str[spos])
          return false;

        ++fpos;
        ++spos;

        continue;
      }

      bool ignore = false;

      if (format_[fpos] == '*') {
        ++fpos;

        if (fpos >= flen)
          return false;

        ignore = true;
      }

      int field_width = -1;

      if (isdigit(format_[fpos])) {
        field_width = 0;

        while (fpos < flen && isdigit(format_[fpos]))
          field_width = field_width*10 + (format_[fpos] - '0');

        if (fpos >= flen)
          return false;
      }

      std::string type_modifier;

      if (format_[fpos] == 'h' || format_[fpos] == 'j' || format_[fpos] == 'l' ||
          format_[fpos] == 'L' || format_[fpos] == 'q' || format_[fpos] == 't' ||
          format_[fpos] == 'z') {
        type_modifier += format_[fpos++];

        if (fpos >= flen)
          return false;

        if (format_[fpos] == 'h') {
          type_modifier += format_[fpos++];

          if (fpos >= flen)
            return false;
        }
      }

      if      (format_[fpos] == 'd' || format_[fpos] == 'D' || format_[fpos] == 'u') {
        auto c = format_[fpos++];

        while (spos < slen && isspace(str[spos]))
          ++spos;

        uint pos  = uint(spos);
        long ival = 0;

        if (! CStrUtil::readInteger(str, &pos, &ival))
          return false;

        spos = pos;

        if (c == 'u' && ival < 0)
          return false;

        addInteger(ival);

        ++ns_;
      }
      else if (format_[fpos] == 'i') {
        ++fpos;

        while (spos < slen && isspace(str[spos]))
          ++spos;

        if (spos >= slen)
          return false;

        // signed integer, base 16 if it begins with 0x or 0X, base 8 if it begins 0,
        long ival { 0 };

        if (str[spos] == '0') {
          ++spos;

          if (spos < slen && (str[spos] == 'x' || str[spos] == 'X')) {
            ++spos;

            if (spos >= slen || ! ::isxdigit(str[spos]))
              return false;

            std::string hexStr;

            while (spos < slen && ::isxdigit(str[spos]))
              hexStr += str[spos++];

            uint h;

            if (! CStrUtil::decodeHexString(hexStr, &h))
              return false;

            ival = h;
          }
          else {
            if (spos >= slen || ! CStrUtil::isBaseChar(str[spos], 8))
              return false;

            int o = 0, o1 = 0;

            while (spos < slen && CStrUtil::isBaseChar(str[spos], 8, &o1)) {
              o = o*8 + o1;

              ++spos;
            }

            ival = o;
          }
        }
        else {
          uint pos = uint(spos);

          if (! CStrUtil::readInteger(str, &pos, &ival))
            return false;
        }

        addInteger(ival);

        ++ns_;
      }
      else if (format_[fpos] == 'o') {
        ++fpos;

        while (spos < slen && isspace(str[spos]))
          ++spos;

        if (spos >= slen || ! CStrUtil::isBaseChar(str[spos], 8))
          return false;

        long ival = 0;
        int  o1   = 0;

        while (spos < slen && CStrUtil::isBaseChar(str[spos], 8, &o1)) {
          ival = ival*8 + o1;

          ++spos;
        }

        addInteger(ival);

        ++ns_;
      }
      else if (format_[fpos] == 'x' || format_[fpos] == 'X') {
        ++fpos;

        while (spos < slen && isspace(str[spos]))
          ++spos;

        if (spos >= slen || ! ::isxdigit(str[spos]))
          return false;

        long ival = 0;

        std::string hexStr;

        while (spos < slen && ::isxdigit(str[spos]))
          hexStr += str[spos++];

        uint h;

        if (! CStrUtil::decodeHexString(hexStr, &h))
          return false;

        ival = h;

        addInteger(ival);

        ++ns_;
      }
      else if (format_[fpos] == 'f' || format_[fpos] == 'e' || format_[fpos] == 'g' ||
               format_[fpos] == 'E' || format_[fpos] == 'a') {
        //auto c = format_[fpos++];
        ++fpos;

        while (spos < slen && isspace(str[spos]))
          ++spos;

        uint   pos  = uint(spos);
        double rval = 0;

        if (! CStrUtil::readReal(str, &pos, &rval))
          return false;

        spos = pos;

        addReal(rval);

        ++ns_;
      }
      else if (format_[fpos] == 's') {
        ++fpos;

        while (spos < slen && isspace(str[spos]))
          ++spos;

        // TODO: max field len
        std::string s;

        while (spos < slen && ! isspace(str[spos]))
          s += str[spos++];

        addString(s);

        ++ns_;
      }
      else if (format_[fpos] == 'c') {
        ++fpos;

        int n = std::max(field_width, 1);

        std::string s;

        while (spos < slen && n > 0) {
          s += str[spos++];

          --n;
        }

        addString(s);

        ++ns_;
      }
      else if (format_[fpos] == '[') {
        ++fpos;

        std::set<char> chars;
        bool           invert = false;

        if      (fpos < flen && format_[fpos] == ']')
          chars.insert(format_[fpos++]);
        else if (fpos < flen && format_[fpos] == '-')
          chars.insert(format_[fpos++]);
        else if (fpos < flen && format_[fpos] == '^') {
          invert = true; ++fpos;
        }

        while (fpos < flen && format_[fpos] != ']') {
          if (format_[fpos] == '-' && fpos + 1 < flen) {
            auto c1 = format_[fpos - 1];
            auto c2 = format_[++fpos];

            if (c1 < c2) std::swap(c1, c2);

            for (auto c = c1; c <= c2; ++c)
              chars.insert(c);

            ++fpos;
          }
          else
            chars.insert(format_[fpos++]);
        }

        if (fpos < flen && format_[fpos] == ']')
          ++fpos;

        std::string s;

        while (spos < slen && chars.find(str[spos]) != chars.end())
          s += str[spos++];

        addString(s);

        ++ns_;
      }
      else if (format_[fpos] == 'p') {
        ++fpos;

        // TODO: pointer
        assert(false);
      }
      else if (format_[fpos] == 'n') {
        ++fpos;

        int n = int(spos);

        addInteger(n);

        ++ns_;
      }
      else {
        return false;
      }
    }
    else {
      if (spos >= slen || format_[fpos] != str[spos])
        return false;

      ++fpos;
      ++spos;
    }
  }

  // extra chars are error ?

  return true;
}

void
CScanF::
addInteger(long l)
{
  if (isDebug())
    std::cerr << l << "\n";

  values_.emplace_back(l);
}

void
CScanF::
addReal(double r)
{
  if (isDebug())
    std::cerr << r << "\n";

  values_.emplace_back(r);
}

void
CScanF::
addString(const std::string &s)
{
  if (isDebug())
    std::cerr << s << "\n";

  values_.emplace_back(s);
}
