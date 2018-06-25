#include <CStrFmt.h>
#include <cstring>

class CStrFmtFormat {
 public:
  CStrFmtFormat(const std::string &str, int width, CStrFmt::AlignType alighType);
 ~CStrFmtFormat();

  std::vector<std::string> format();

 private:
  void addWord();
  bool isSplitter(const std::string &str, int pos);

 private:
  std::string              str;
  int                      width { 0 };
  CStrFmt::AlignType       alighType;
  std::vector<std::string> lines;
  std::string              line;
  std::string              word;
  char                     lastChar { '\0' };
};

//---

std::vector<std::string>
CStrFmt::
format(const std::string &str, int width, CStrFmt::AlignType alighType)
{
  CStrFmtFormat fmt(str, width, alighType);

  return fmt.format();
}

//---

CStrFmtFormat::
CStrFmtFormat(const std::string &str1, int width1, CStrFmt::AlignType alighType1) :
 str(str1), width(width1), alighType(alighType1)
{
  lastChar = '\0';
}

CStrFmtFormat::
~CStrFmtFormat()
{
}

std::vector<std::string>
CStrFmtFormat::
format()
{
  if (width <= 0) {
    lines.push_back(str);

    return lines;
  }

  //------

  lastChar = '\0';

  int len = str.size();

  for (int i = 0; i < len; i++) {
    if      (str[i] == '\n') {
      addWord();

      lastChar = '\0';
    }
    else if (isSplitter(str, i)) {
      if (! isspace(str[i]))
        word += str[i];

      addWord();

      if (isspace(str[i]))
        lastChar = ' ';
      else
        lastChar = '\0';
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
    lines[i] = CStrFmt::align(lines[i], width, alighType, '\0');

  return lines;
}

void
CStrFmtFormat::
addWord()
{
  if (word.size() == 0)
    return;

  int lastChar_len = 0;

  if (lastChar != '\0')
    lastChar_len = 1;

  int line_len = line.size();
  int word_len = word.size();

  if (line_len + word_len + lastChar_len > width) {
    if (word_len > width) {
      if (lastChar != '\0' && line_len > 0)
        line += lastChar;

      while (word_len > width) {
        line += word.substr(0, width - line_len);

        word = word.substr(width - line_len);

        word_len = word.size();

        lines.push_back(line);

        line = "";

        line_len = 0;
      }

      lastChar = '\0';
    }
    else {
      lines.push_back(line);

      line = "";

      line_len = 0;
    }
  }

  if (lastChar != '\0' && line_len > 0)
    line += lastChar;

  line += word;

  word = "";
}

std::string
CStrFmt::
align(const std::string &str, int width, AlignType alighType, char pad_char, ClipType clipType)
{
  static int direction = 0;

  std::string str1;

  int strLen = str.size();

  if (width > 0 && strLen > width) {
    if (clipType == ClipType::NONE)
      return str;

    int nc = strLen - width;

    if      (clipType == ClipType::LEFT)
      str1 = str.substr(nc, width);
    else if (clipType == ClipType::RIGHT)
      str1 = str.substr(0, width);
    else if (clipType == ClipType::MIDDLE) {
      int nl = width/2;
      int nr = width - nl;

      str1 = str.substr(0, nl) + str.substr(nl + nc, nr);
    }
  }
  else
    str1 = str;

  //------

  std::string str2 = "";

  int str2_len = 0;

  if (alighType == AlignType::JUSTIFY) {
    int num_chars = 0;
    int num_words = 1;

    int i = 0;

    while (i < strLen) {
      if (! isspace(str1[i])) {
        num_chars++;

        i++;
      }
      else {
        num_words++;

        i++;

        while (i < strLen && isspace(str1[i]))
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

      while (i < strLen) {
        if (isspace(str1[i])) {
          i++;

          while (i < strLen && isspace(str1[i]))
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

      str2_len = strLen;

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
    int num_pads = width - strLen;

    //------

    int pad_left  = 0;
    int pad_right = 0;

    if      (alighType == AlignType::CENTER) {
      pad_left  = num_pads/2;
      pad_right = num_pads - num_pads/2;
    }
    else if (alighType == AlignType::RIGHT)
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

    str2_len += strLen;

    if (pad_char != '\0') {
      for (int j = 0; j < pad_right; j++) {
        str2 += pad_char;

        ++str2_len;
      }
    }
  }

  return str2;
}

bool
CStrFmtFormat::
isSplitter(const std::string &str, int pos)
{
  /* Space is a Splitter ... */

  if (isspace(str[pos]))
    return true;

  //------

  /* Punctuation Characters followed by an End of String or
     a Space are Splitters ... */

  int strLen = str.size();

  if (ispunct(str[pos]) && (pos == strLen - 1 || isspace(str[pos + 1])))
    return true;

  //------

  /* Comma, Colon, Semi Colon, Question Mark and Exclamation Mark
     followed by an End of String or a non-Punctuation Character
     is a Splitter ... */

  if (strchr(",:;?!", str[pos]) != NULL && (pos == strLen - 1 || ! ispunct(str[pos + 1])))
    return true;

  //------

  /* Any other characters are not Splitters ... */

  return false;
}
