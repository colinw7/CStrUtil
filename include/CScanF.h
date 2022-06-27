#ifndef CScanF_H
#define CScanF_H

#include <string>
#include <vector>
#include <cassert>

class CScanF {
 public:
  enum class ValueType {
    NONE,
    REAL,
    INTEGER,
    STRING
  };

  class Value {
   public:
    Value() { }

    Value(double             r) : type_(ValueType::REAL   ), r_(r) { }
    Value(long               i) : type_(ValueType::INTEGER), i_(i) { }
    Value(const std::string &s) : type_(ValueType::STRING ), s_(s) { }

    const ValueType &type() const { return type_; }

    double             real   () const { assert(type_ == ValueType::REAL   ); return r_; }
    long               integer() const { assert(type_ == ValueType::INTEGER); return i_; }
    const std::string &string () const { assert(type_ == ValueType::STRING ); return s_; }

   private:
    ValueType   type_ { ValueType::NONE };
    double      r_    { 0.0 };
    long        i_    { 0 };
    std::string s_;
  };

  using Values = std::vector<Value>;

 public:
  explicit CScanF(const std::string &format="");

  virtual ~CScanF() { }

  bool isDebug() const { return debug_; }
  void setDebug(bool b) { debug_ = b; }

  bool scan(const std::string &str);

  virtual void addInteger(long l);
  virtual void addReal   (double r);
  virtual void addString (const std::string &s);

  int numValues() const { return int(values_.size()); }

  const Value &value(int i) {
    assert(i >= 0 && i < int(values_.size())); return values_[size_t(i)];
  }

 private:
  std::string format_;
  int         ns_ { 0 };
  Values      values_;
  bool        debug_ { false };
};

#endif
