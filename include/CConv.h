#ifndef CCONV_H
#define CCONV_H

#include <sstream>
#include <vector>

namespace CConv {
  template<class T>
  std::string toString(const T &t) {
    std::ostringstream ss;

    ss << t;

    return ss.str();
  }
}

namespace CConv {
  template<class T>
  std::vector<T> toVector(const T *a, uint n) {
    std::vector<T> v;

    v.resize(n);

    for (uint i = 0; i < n; ++i)
      v[i] = a[i];

    return v;
  }

  template<class T, class U>
  std::vector<T> toVector(const U *a, const U *b, uint n) {
    std::vector<T> v;

    v.resize(n);

    for (uint i = 0; i < n; ++i)
      v[i] = T(a[i], b[i]);

    return v;
  }

  template<class T>
  std::vector<T> toVector(const T &t1) {
    std::vector<T> v;

    v.push_back(t1);

    return v;
  }

  template<class T>
  std::vector<T> toVector(const T &t1, const T &t2) {
    std::vector<T> v;

    v.push_back(t1);
    v.push_back(t2);

    return v;
  }

  template<class T>
  std::vector<T> toVector(const T &t1, const T &t2, const T &t3) {
    std::vector<T> v;

    v.push_back(t1);
    v.push_back(t2);
    v.push_back(t3);

    return v;
  }

  template<class T>
  std::vector<T> toVector(const T &t1, const T &t2, const T &t3, const T &t4) {
    std::vector<T> v;

    v.push_back(t1);
    v.push_back(t2);
    v.push_back(t3);
    v.push_back(t4);

    return v;
  }
}

#endif
