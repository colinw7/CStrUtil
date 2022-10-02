#include <CStrFmt.h>
#include <cstring>

class CStrFmtFormat {
 public:
  CStrFmtFormat(const std::string &str, uint width, CStrFmt::AlignType alighType);
 ~CStrFmtFormat();

  std::vector<std::string> format();

 private:
  void addWord();
  bool isSplitter(const std::string &sstr, uint pos);

 private:
  std::string              str;
  uint                     width { 0 };
  CStrFmt::AlignType       alighType;
  std::vector<std::string> lines;
  std::string              line;
  std::string              word;
  char                     lastChar { '\0' };
};

//---

std::vector<std::string>
CStrFmt::
format(const std::string &str, uint width, CStrFmt::AlignType alighType)
{
  CStrFmtFormat fmt(str, width, alighType);

  return fmt.format();
}

//---

CStrFmtFormat::
CStrFmtFormat(const std::string &str1, uint width1, CStrFmt::AlignType alighType1) :
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

  auto len = str.size();

  for (uint i = 0; i < len; i++) {
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

  auto num_lines = lines.size();

  for (uint i = 0; i < num_lines; i++)
    lines[i] = CStrFmt::align(lines[i], width, alighType, '\0');

  return lines;
}

void
CStrFmtFormat::
addWord()
{
  if (word.size() == 0)
    return;

  uint lastChar_len = 0;

  if (lastChar != '\0')
    lastChar_len = 1;

  auto line_len = line.size();
  auto word_len = word.size();

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
align(const std::string &str, uint width, AlignType alighType, char pad_char, ClipType clipType)
{
  static int direction = 0;

  std::string str1;

  auto strLen = uint(str.size());

  if (width > 0 && strLen > uint(width)) {
    if (clipType == ClipType::NONE)
      return str;

    uint nc = strLen - uint(width);

    if      (clipType == ClipType::LEFT)
      str1 = str.substr(nc, width);
    else if (clipType == ClipType::RIGHT)
      str1 = str.substr(0, width);
    else if (clipType == ClipType::MIDDLE) {
      uint nl = width/2;
      uint nr = uint(std::max(width - nl, 0U));

      str1 = str.substr(0, nl) + str.substr(nl + nc, nr);
    }
  }
  else
    str1 = str;

  //------

  std::string str2 = "";

  uint str2_len = 0;

  if (alighType == AlignType::JUSTIFY) {
    uint num_chars = 0;
    uint num_words = 1;

    uint i = 0;

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

    uint num_pads = uint(std::max(width - num_chars, 0U));

    //------

    if (num_words > 1) {
      /* Share out the extra spaces evenly between the gaps
         between the words ... */

      int *pads = new int [num_words - 1];

      for (uint ii = 0; ii < uint(num_words - 1); ii++)
        pads[ii] = 0;

      if (direction == 0) {
        uint ii = 0;

        while (num_pads > 0) {
          pads[ii++]++;

          num_pads--;

          if (ii >= num_words - 1)
            ii = 0;
        }

        direction = 1;
      }
      else {
        int ii = int(num_words - 2);

        while (num_pads > 0) {
          pads[ii--]++;

          num_pads--;

          if (ii < 0)
            ii = int(num_words - 2);
        }

        direction = 0;
      }

      //------

      num_words = 0;

      uint ii = 0;

      while (ii < strLen) {
        if (isspace(str1[ii])) {
          ii++;

          while (ii < strLen && isspace(str1[ii]))
            ii++;

          for (int j = 0; j < pads[num_words]; j++) {
            str2 += ' ';

            ++str2_len;
          }

          num_words++;
        }
        else {
          str2 += str1[ii++];

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
    uint num_pads = uint(std::max(width - strLen, 0U));

    //------

    uint pad_left  = 0;
    uint pad_right = 0;

    if      (alighType == AlignType::CENTER) {
      pad_left  = num_pads/2;
      pad_right = std::max(num_pads - num_pads/2, 0u);
    }
    else if (alighType == AlignType::RIGHT)
      pad_left  = num_pads;
    else
      pad_right = num_pads;

    //------

    if (pad_char != '\0') {
      for (uint j = 0; j < pad_left; j++) {
        str2 += pad_char;

        ++str2_len;
      }
    }

    str2 += str1;

    str2_len += strLen;

    if (pad_char != '\0') {
      for (uint j = 0; j < pad_right; j++) {
        str2 += pad_char;

        ++str2_len;
      }
    }
  }

  return str2;
}

bool
CStrFmtFormat::
isSplitter(const std::string &sstr, uint pos)
{
  /* Space is a Splitter ... */

  if (isspace(sstr[pos]))
    return true;

  //------

  /* Punctuation Characters followed by an End of String or
     a Space are Splitters ... */

  auto strLen = sstr.size();

  if (ispunct(sstr[pos]) && (pos == strLen - 1 || isspace(sstr[pos + 1])))
    return true;

  //------

  /* Comma, Colon, Semi Colon, Question Mark and Exclamation Mark
     followed by an End of String or a non-Punctuation Character
     is a Splitter ... */

  if (strchr(",:;?!", sstr[pos]) != NULL && (pos == strLen - 1 || ! ispunct(sstr[pos + 1])))
    return true;

  //------

  /* Any other characters are not Splitters ... */

  return false;
}
