// testBraceBug.cpp

/*
 * ver 1.5 - added test for multiple contiguous escape chars to test
 *           bug fix in Tokenizer by TA Mehmet Kaya
 * ver 1.4 - braces in quotes and comments caused increment bracecount
 *         - eliminated by adding:
 *             enum state { default_state, comment_state, quote_state }
 *             and incrementing only if in default_state
 */

#include <string>
using namespace std;

void test()
{
  std::string tok;
  char ch = 'a';

  if (tok == "{")
  {
  }
  if (ch == '{')
  {
  }
  // {{{
  /* }} */
  // the end

  /*\**/ 
  // a correct comment equivalent to /***/

  // test many escapes
  std::string s = "This\\\"s string variable";
  s = "This\\\\\"\"\\\"s string variable";
  s = "This\\\\\\\"\"\"s string variable";

  //int x;  // more tokens
}