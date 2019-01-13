#ifndef CSTR_PERMUTE_H
#define CSTR_PERMUTE_H

#include <algorithm>
#include <string>
#include <vector>
#include <sys/types.h>

template<class F>
void
CStrPermuteChars(char *src, uint len, char *dst, uint pos, uint n, F f)
{
  for (uint i = 0; i < len; ++i) {
    if (src[i] == '\0') continue;

    std::swap(dst[pos], src[i]);

    if (pos + 1 < n)
      CStrPermuteChars(src, len, dst, pos + 1, n, f);
    else
      f(&dst[0]);

    std::swap(dst[pos], src[i]);
  }
}

template<class F>
void
CStrPermute(const std::string &word, uint n, F f)
{
  uint len = word.size();

  std::vector<char> src(word.begin(), word.end());

  std::vector<char> dst;

  dst.resize(n, '\0');

  // for each position (0 to n-1) select each word char in turn

  CStrPermuteChars(&src[0], len, &dst[0], 0, n, f);
}

#endif
