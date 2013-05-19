#ifndef CCONV_H
#define CCONV_H

#include <sstream>

namespace CConv {
  template<class T>
  std::string toString(const T &t) {
    std::ostringstream ss;

    ss << t;

    return ss.str();
  }
}

#endif
