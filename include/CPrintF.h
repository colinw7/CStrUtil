#ifndef CPRINT_F
#define CPRINT_F

#include <CConv.h>
#include <vector>

class CPrintF {
 public:
#ifdef _HAS_LONG_LONG
  typedef long long LLong;
#else
  typedef long LLong;
#endif

 public:
  enum Flags {
    LEFT_JUSTIFY         = (1<<0),
    DISPLAY_SIGN         = (1<<1),
    PAD_POSITIVE         = (1<<2),
    USE_ALTERNATE        = (1<<3),
    ZERO_PAD             = (1<<4),
    FIELD_WIDTH_AS_VALUE = (1<<5),
    PRECISION_AS_VALUE   = (1<<6)
  };

  CPrintF(const std::string &format="");

  virtual ~CPrintF() { }

  const std::string &formatString() const { return format_; }
  void setFormatString(const std::string &fmt) { format_ = fmt; }

  std::string format() const;

  virtual int         getInt     () const = 0;
  virtual long        getLong    () const = 0;
  virtual LLong       getLongLong() const = 0;
  virtual double      getDouble  () const = 0;
  virtual std::string getString  () const = 0;

  static bool readFormat(const std::string &str, int *pos, std::string &format, int *field_width,
                         int *precision, char *length_modifier, char *format_code, int *flags);

  static bool readRealFormat   (const std::string &str, int *pos, std::string &format);
  static bool readIntegerFormat(const std::string &str, int *pos, std::string &format);
  static bool readStringFormat (const std::string &str, int *pos, std::string &format);

  template<typename T1>
  static std::string format(const char *fmt, const T1 &t1) {
    std::vector<std::string> strs;

    strs.push_back(CConv::toString(t1));

    return formatStrings(fmt, strs);
  }

  template<typename T1, typename T2>
  static std::string format(const char *fmt, const T1 &t1, const T2 &t2) {
    std::vector<std::string> strs;

    strs.push_back(CConv::toString(t1));
    strs.push_back(CConv::toString(t2));

    return formatStrings(fmt, strs);
  }

  static std::string formatStrings(const char *fmt, const std::vector<std::string> &strs);

 private:
  std::string format_;
};

#endif
