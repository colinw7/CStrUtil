#ifndef CStrFmt_H
#define CStrFmt_H

#include <string>
#include <vector>

namespace CStrFmt {

enum class AlignType {
  LEFT,
  RIGHT,
  CENTER,
  JUSTIFY
};

enum class ClipType {
  NONE,
  LEFT,
  RIGHT,
  MIDDLE
};

using Strings = std::vector<std::string>;

Strings format(const std::string &str, int width=80, AlignType alignType=AlignType::LEFT);

std::string align(const std::string &str, int width=80, AlignType alignType=AlignType::LEFT,
                  char pad_char=' ', ClipType clipType=ClipType::NONE);

}

#endif
