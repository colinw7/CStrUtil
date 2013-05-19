#include <CPrintF.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>

CPrintF::
CPrintF(const std::string &format) :
 format_(format)
{
}

std::string
CPrintF::
format() const
{
  std::string str = "";

  int format_len = format_.size();

  int i = 0;

  while (i < format_len) {
    switch (format_[i]) {
      case '%': {
        bool field_width_as_value = false;
        bool precision_as_value   = false;

        std::string value_format = "";

        value_format += format_[i++];

        while (i < format_len && strchr("-+ #0", format_[i]) != NULL)
          value_format += format_[i++];

        if (format_[i] == '*') {
          field_width_as_value = true;

          value_format += format_[i++];
        }
        else {
          while (i < format_len && isdigit(format_[i]))
            value_format += format_[i++];
        }

        if (format_[i] == '.') {
          value_format += format_[i++];

          if (format_[i] == '*') {
            precision_as_value = true;

            value_format += format_[i++];
          }
          else {
            while (i < format_len && isdigit(format_[i]))
              value_format += format_[i++];
          }
        }

        int format_qualifier = '\0';

        while (i < format_len && strchr("hlL", format_[i]) != NULL) {
          if      (format_qualifier == 'l' && format_qualifier == 'l')
            format_qualifier = 'j';
          else if (format_qualifier == '\0')
            format_qualifier = format_[i];

          value_format += format_[i++];
        }

        int format_code = format_[i];

        value_format += format_[i++];

        if (strchr("diouxXfeEgGcs%", format_code) == NULL) {
          str += value_format;

          break;
        }

        if (format_code == '%') {
          str += (char) format_code;

          break;
        }

        long field_width = 0;

        if (field_width_as_value)
          field_width = getInt();

        int precision = 0;

        if (precision_as_value)
          precision = getInt();

        switch (format_code) {
          case 'd':
          case 'i':
          case 'o':
          case 'u':
          case 'x':
          case 'X':
          case 'c': {
            LLong integer;

            if      (format_qualifier == 'h')
              integer = (LLong) getInt();
            else if (format_qualifier == 'l')
              integer = (LLong) getLong();
            else if (format_qualifier == 'j')
              integer = (LLong) getLongLong();
            else
              integer = (LLong) getInt();

            if (format_code == 'c') {
              if (integer == 0 || ! isprint(integer))
                integer = '.';
            }

            char *buffer = (char *) malloc(field_width + precision + 256);

            const char *value_format1 = value_format.c_str();

            if      (field_width_as_value && precision_as_value)
              ::sprintf(buffer, value_format1, field_width, precision, integer);
            else if (field_width_as_value)
              ::sprintf(buffer, value_format1, field_width, integer);
            else if (precision_as_value)
              ::sprintf(buffer, value_format1, precision, integer);
            else
              ::sprintf(buffer, value_format1, integer);

            str += buffer;

            free((char *) buffer);

            break;
          }

          case 's': {
            std::string str1 = getString();

            char *buffer = (char *) malloc(field_width + precision + str1.size() + 256);

            const char *value_format1 = value_format.c_str();

            if      (field_width_as_value && precision_as_value)
              ::sprintf(buffer, value_format1, field_width, precision, str1.c_str());
            else if (field_width_as_value)
              ::sprintf(buffer, value_format1, field_width, str1.c_str());
            else if (precision_as_value)
              ::sprintf(buffer, value_format1, precision, str1.c_str());
            else
              ::sprintf(buffer, value_format1, str1.c_str());

            str += buffer;

            free((char *) buffer);

            break;
          }

          case 'f':
          case 'e':
          case 'E':
          case 'g':
          case 'G': {
            double real = getDouble();

            char *buffer = (char *) malloc(field_width + precision + 256);

            const char *value_format1 = value_format.c_str();

            if      (field_width_as_value && precision_as_value)
              ::sprintf(buffer, value_format1, field_width, precision, real);
            else if (field_width_as_value)
              ::sprintf(buffer, value_format1, field_width, real);
            else if (precision_as_value)
              ::sprintf(buffer, value_format1, precision, real);
            else
              ::sprintf(buffer, value_format1, real);

            str += buffer;

            free((char *) buffer);

            break;
          }
        }

      } break;

      default: {
        str += format_[i++];

      } break;
    }
  }

  return str;
}

bool
CPrintF::
readFormat(const std::string &str, int *pos, std::string &format, int *field_width, int *precision,
           char *length_modifier, char *format_code, int *flags)
{
  int save_pos = *pos;

  int len = str.size();

  format = "";

  *flags = 0;

  /* Required '%' */

  if (*pos >= len || str[*pos] != '%') {
    *pos = save_pos;
    return false;
  }

  format += str[(*pos)++];

  /*------*/

  /* Optional '-+ #0' chars (multiples valid ?) */

  while (*pos < len && strchr("-+ #0", str[*pos]) != NULL) {
    if      (str[*pos] == '-')
      *flags |= LEFT_JUSTIFY;
    else if (str[*pos] == '+')
      *flags |= DISPLAY_SIGN;
    else if (str[*pos] == ' ')
      *flags |= PAD_POSITIVE;
    else if (str[*pos] == '#')
      *flags |= USE_ALTERNATE;
    else if (str[*pos] == '0')
      *flags |= ZERO_PAD;

    format += str[(*pos)++];
  }

  /*------*/

  /* optional field width */

  *field_width = -1;

  if (*pos < len && str[*pos] == '*') {
    *flags |= FIELD_WIDTH_AS_VALUE;

    format += str[(*pos)++];
  }
  else {
    std::string field_width_str;

    while (*pos < len && isdigit(str[*pos]))
      field_width_str += str[(*pos)++];

    *field_width = atoi(field_width_str.c_str());

    format += field_width_str;
  }

  /*------*/

  /* optional precision */

  *precision = -1;

  if (*pos < len && str[*pos] == '.') {
    format = str[(*pos)++];

    /* optional precision value */

    if (*pos < len && str[*pos] == '*') {
      *flags |= PRECISION_AS_VALUE;

      format = str[(*pos)++];
    }
    else {
      std::string precision_str;

      while (*pos < len && isdigit(str[*pos]))
        precision_str += str[(*pos)++];

      *precision = atoi(precision_str.c_str());

      format += precision_str;
    }
  }

  /*------*/

  /* optional length modifier */

  if (*pos < len &&
      (str[*pos] == 'h' || str[*pos] == 'l' ||  str[*pos] == 'L')) {
    *length_modifier = str[(*pos)++];

    format += *length_modifier;
  }

  /*------*/

  /* Required format code */

  if (*pos >= len) {
    *pos = save_pos;
    return false;
  }

  *format_code = str[(*pos)++];

  format += format_code;

  return true;
}

bool
CPrintF::
readRealFormat(const std::string &str, int *pos, std::string &format)
{
  int  flags;
  char format_code;
  char length_modifier;
  int  field_width, precision;

  if (! readFormat(str, pos, format, &field_width, &precision, &length_modifier,
                   &format_code, &flags))
    return false;

  if (strchr("feEgG", format_code) == NULL)
    return false;

  return true;
}

bool
CPrintF::
readIntegerFormat(const std::string &str, int *pos, std::string &format)
{
  int  flags;
  char format_code;
  char length_modifier;
  int  field_width, precision;

  if (! readFormat(str, pos, format, &field_width, &precision, &length_modifier,
                   &format_code, &flags))
    return false;

  if (strchr("diouxXc", format_code) == NULL)
    return false;

  return true;
}

bool
CPrintF::
readStringFormat(const std::string &str, int *pos, std::string &format)
{
  int  flags;
  char format_code;
  char length_modifier;
  int  field_width, precision;

  if (! readFormat(str, pos, format, &field_width, &precision, &length_modifier,
                   &format_code, &flags))
    return false;

  if (format_code != 's')
    return false;

  return true;
}

std::string
CPrintF::
formatStrings(const char *fmt, const std::vector<std::string> &strs)
{
  uint num_strs = strs.size();

  std::string format(fmt);

  std::string str = "";

  int format_len = format.size();

  int i = 0;

  while (i < format_len) {
    switch (format[i]) {
      case '%': {
        if (i < format_len - 1 && format[i + 1] == '%') {
          str += format[++i];
          break;
        }

        std::string value_format = "";

        value_format += format[i++];

        if (i >= format_len || ! isdigit(format[i])) {
          str += value_format;

          break;
        }

        uint arg_num = 0;

        while (i < format_len && isdigit(format[i])) {
          arg_num = arg_num*10 + (format[i] - '0');

          value_format += format[i++];
        }

        if (arg_num < 1 || arg_num > num_strs) {
          str += value_format;

          break;
        }

        const std::string &str1 = strs[arg_num - 1];

        str += str1;

        break;
      }
      default: {
        str += format[i++];

        break;
      }
    }
  }

  return str;
}
