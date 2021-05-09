#ifndef CVPrintF_H
#define CVPrintF_H

namespace CVPrintF {
  void print(const char *s) {
    while (s && *s) {
      if (*s=='%' && *++s!='%')
        throw runtime_error("invalid format: missing arguments");

      std::cout << *s++;
    }
  }

  template<typename T, typename... Args>
  void print(const char *s, T value, Args... args) {
    while (s && *s) {
      if (*s == '%' && *++s != '%') {
        std::cout << value;

        return print(++s, args...);
      }

      std::cout << *s++;
    }
  }
}

#endif
