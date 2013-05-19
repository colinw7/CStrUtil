#include <CStrUtil.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int
main(int argc, char **argv)
{
  string istring = CStrUtil::toString(45);

  cout << istring << endl;

  string rstring = CStrUtil::toString(3.563);

  cout << rstring << endl;

  cout << CStrUtil::toInteger(istring) << endl;

  cout << CStrUtil::toReal(rstring) << endl;

  cout << "'" << CStrUtil::stripSpaces("fred")                    <<
          "'" << endl;
  cout << "'" << CStrUtil::stripSpaces("  fred")                  <<
          "'" << endl;
  cout << "'" << CStrUtil::stripSpaces("fred  ")                  <<
          "'" << endl;
  cout << "'" << CStrUtil::stripSpaces(" fred    ")               <<
          "'" << endl;
  cout << "'" << CStrUtil::stripSpaces(" fred    ", true, false)  <<
          "'" << endl;
  cout << "'" << CStrUtil::stripSpaces(" fred    ", false, true)  <<
          "'" << endl;
  cout << "'" << CStrUtil::stripSpaces(" fred    ", false, false) <<
          "'" << endl;

  cout << CStrUtil::toLower   ("heLLo") << endl;
  cout << CStrUtil::toUpper   ("heLLo") << endl;
  cout << CStrUtil::capitalize("heLLo") << endl;

  cout << CStrUtil::translate("1234", "0123456789", "1234567890") << endl;
  cout << CStrUtil::translate("a2aa2a", "a", "", true) << endl;

  CStrWords::iterator word_p;

  CStrWords word_list = CStrUtil::toWords(" One Two    Three   ", NULL);

  cout << " One Two    Three   " << endl;

  for (word_p = word_list.begin(); word_p != word_list.end(); ++word_p)
    cout << ">" << (*word_p).getWord() << "<" << endl;

  word_list = CStrUtil::toWords("'Fred\\'s Cat' Fred", NULL);

  cout << "'Fred\\'s Cat' Fred" << endl;

  for (word_p = word_list.begin(); word_p != word_list.end(); ++word_p)
    cout << ">" << (*word_p).getWord() << "<" << endl;

  word_list = CStrUtil::toWords("   ", NULL);

  cout << "   " << endl;

  for (word_p = word_list.begin(); word_p != word_list.end(); ++word_p)
    cout << ">" << (*word_p).getWord() << "<" << endl;

  word_list = CStrUtil::toFields("Fred:Bill:Harry", ":");

  cout << "Fred:Bill:Harry" << endl;

  for (word_p = word_list.begin(); word_p != word_list.end(); ++word_p)
    cout << ">" << (*word_p).getWord() << "<" << endl;

  word_list = CStrUtil::toFields(":Fred:Bill:Harry:", ":");

  cout << ":Fred:Bill:Harry:" << endl;

  for (word_p = word_list.begin(); word_p != word_list.end(); ++word_p)
    cout << ">" << (*word_p).getWord() << "<" << endl;

  int n = 1;

  for (int i = 0; i < 10; ++i) {
    cout << CStrUtil::toOctStringInWidth(n, 3);

    n *= 2;
  }

  cout << endl;
}
