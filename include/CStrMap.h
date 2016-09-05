#ifndef CSTR_MAP_H
#define CSTR_MAP_H

template<typename T, typename VT=T>
class CStrMap {
 public:
  CStrMap() { }

  CStrMap(const char *name, ...) {
    va_list vargs;

    va_start(vargs, name);

    while (name != NULL) {
      T value = (T) va_arg(vargs, VT);

      add(name, value);

      name = va_arg(vargs, const char *);
    }

    va_end(vargs);
  }

  void add(const std::string &name, const T &value) {
    values_[name] = value;
  }

  bool map(const std::string &name, T &value) const {
    typename ValueMap::const_iterator p = values_.find(name);

    if (p != values_.end()) {
      value = (*p).second;
      return true;
    }

    if (allowUniqueMatchFirst_) {
      bool unique = false;

      typename ValueMap::const_iterator i1, i2, iu;

      for (i1 = values_.begin(), i2 = values_.end(); i1 != i2; ++i1) {
        const std::string &name = (*p).first;

        std::string::size_type pos = name.find(name);

        if (pos == std::string::npos || pos > 0) continue;

        if (! unique) {
          value  = (*p).second;
          unique = true;
        }
        else {
          unique = false;
          break;
        }
      }

      return unique;
    }

    return false;
  }

 private:
  typedef std::map<std::string,T> ValueMap;

  ValueMap values_;
  bool     allowUniqueMatchFirst_;
};

#endif
