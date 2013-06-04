#ifndef CSTR_FMT_H
#define CSTR_FMT_H

#include <string>
#include <vector>
#include <cstring>

enum CStrFmtAlignType {
  CSTR_FMT_ALIGN_LEFT,
  CSTR_FMT_ALIGN_RIGHT,
  CSTR_FMT_ALIGN_CENTER,
  CSTR_FMT_ALIGN_JUSTIFY
};

class CStrFmtFormat {
 public:
  CStrFmtFormat(const std::string &str1, int width1, CStrFmtAlignType align_type1) :
   str(str1), width(width1), align_type(align_type1) {
    last_char = '\0';
  }

 ~CStrFmtFormat() { }

  std::vector<std::string> format() {
    if (width <= 0) {
      lines.push_back(str);

      return lines;
    }

    //------

    last_char = '\0';

    int len = str.size();

    for (int i = 0; i < len; i++) {
      if      (str[i] == '\n') {
        addWord();

        last_char = '\0';
      }
      else if (isSplitter(str, i)) {
        if (! isspace(str[i]))
          word += str[i];

        addWord();

        if (isspace(str[i]))
          last_char = ' ';
        else
          last_char = '\0';
      }
      else
        word += str[i];
    }

    //------

    addWord();

    //------

    if (line.size() > 0)
      lines.push_back(line);

    //------

    int num_lines = lines.size();

    for (int i = 0; i < num_lines; i++)
      lines[i] = align(lines[i], width, align_type, '\0');

    return lines;
  }

  static std::string align(const std::string &str, int width=80,
                           CStrFmtAlignType align_type=CSTR_FMT_ALIGN_LEFT,
                           char pad_char=' ', bool clip=false) {
    static int direction = 0;

    std::string str1;

    int str_len = str.size();

    if (width <= 0 || str_len > width) {
      if (! clip)
        return str;

      str1 = str.substr(0, width);
    }
    else
      str1 = str;

    //------

    std::string str2 = "";

    int str2_len = 0;

    if (align_type == CSTR_FMT_ALIGN_JUSTIFY) {
      int num_chars = 0;
      int num_words = 1;

      int i = 0;

      while (i < str_len) {
        if (! isspace(str1[i])) {
          num_chars++;

          i++;
        }
        else {
          num_words++;

          i++;

          while (i < str_len && isspace(str1[i]))
            i++;
        }
      }

      //------

      int num_pads = width - num_chars;

      //------

      if (num_words > 1) {
        /* Share out the extra spaces evenly between the gaps
           between the words ... */

        int *pads = new int [num_words - 1];

        for (int i = 0; i < num_words - 1; i++)
          pads[i] = 0;

        if (direction == 0) {
          int i = 0;

          while (num_pads > 0) {
            pads[i++]++;

            num_pads--;

            if (i >= num_words - 1)
              i = 0;
          }

          direction = 1;
        }
        else {
          int i = num_words - 2;

          while (num_pads > 0) {
            pads[i--]++;

            num_pads--;

            if (i < 0)
              i = num_words - 2;
          }

          direction = 0;
        }

        //------

        num_words = 0;

        int i = 0;

        while (i < str_len) {
          if (isspace(str1[i])) {
            i++;

            while (i < str_len && isspace(str1[i]))
              i++;

            for (int j = 0; j < pads[num_words]; j++) {
              str2 += ' ';

              ++str2_len;
            }

            num_words++;
          }
          else {
            str2 += str1[i++];

            ++str2_len;
          }
        }

        delete [] pads;
      }

      /* otherwise just left align word in width ... */

      else {
        str2 = str1;

        str2_len = str_len;

        if (pad_char != '\0') {
          while (str2_len < width) {
            str2 += pad_char;

            ++str2_len;
          }
        }
      }
    }

    /* Left, Centre or Right Align Line */

    else {
      int num_pads = width - str_len;

      //------

      int pad_left  = 0;
      int pad_right = 0;

      if      (align_type == CSTR_FMT_ALIGN_CENTER) {
        pad_left  = num_pads/2;
        pad_right = num_pads - num_pads/2;
      }
      else if (align_type == CSTR_FMT_ALIGN_RIGHT)
        pad_left  = num_pads;
      else
        pad_right = num_pads;

      //------

      if (pad_char != '\0') {
        for (int j = 0; j < pad_left; j++) {
          str2 += pad_char;

          ++str2_len;
        }
      }

      str2 += str1;

      str2_len += str_len;

      if (pad_char != '\0') {
        for (int j = 0; j < pad_right; j++) {
          str2 += pad_char;

          ++str2_len;
        }
      }
    }

    return str2;
  }

 private:
  void addWord() {
    if (word.size() == 0)
      return;

    int last_char_len = 0;

    if (last_char != '\0')
      last_char_len = 1;

    int line_len = line.size();
    int word_len = word.size();

    if (line_len + word_len + last_char_len > width) {
      if (word_len > width) {
        if (last_char != '\0' && line_len > 0)
          line += last_char;

        while (word_len > width) {
          line += word.substr(0, width - line_len);

          word = word.substr(width - line_len);

          word_len = word.size();

          lines.push_back(line);

          line = "";

          line_len = 0;
        }

        last_char = '\0';
      }
      else {
        lines.push_back(line);

        line = "";

        line_len = 0;
      }
    }

    if (last_char != '\0' && line_len > 0)
      line += last_char;

    line += word;

    word = "";
  }

  bool isSplitter(const std::string &str, int pos) {
    /* Space is a Splitter ... */

    if (isspace(str[pos]))
      return true;

    //------

    /* Punctuation Characters followed by an End of String or
       a Space are Splitters ... */

    int str_len = str.size();

    if (ispunct(str[pos]) &&
        (pos == str_len - 1 || isspace(str[pos + 1])))
      return true;

    //------

    /* Comma, Colon, Semi Colon, Question Mark and Exclamation Mark
       followed by an End of String or a non-Punctuation Character
       is a Splitter ... */

    if (strchr(",:;?!", str[pos]) != NULL &&
        (pos == str_len - 1 || ! ispunct(str[pos + 1])))
      return true;

    //------

    /* Any other characters are not Splitters ... */

    return false;
  }

 private:
  std::string              str;
  int                      width;
  CStrFmtAlignType         align_type;
  std::vector<std::string> lines;
  std::string              line;
  std::string              word;
  char                     last_char;
};

namespace CStrFmt {
  inline std::vector<std::string> format(const std::string &str, int width=80,
                                         CStrFmtAlignType align_type=CSTR_FMT_ALIGN_LEFT) {
    CStrFmtFormat fmt(str, width, align_type);

    return fmt.format();
  }

  inline std::string align(const std::string &str, int width=80,
                           CStrFmtAlignType align_type=CSTR_FMT_ALIGN_LEFT,
                           char pad_char=' ', bool clip=false) {
    return CStrFmtFormat::align(str, width, align_type, pad_char, clip);
  }
}

#endif
