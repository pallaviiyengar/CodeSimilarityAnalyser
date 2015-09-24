///////////////////////////////////////////////////////////////////////////////
// ScopeStackTest.cpp - demo of template stack holding items with two values //
//                                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2011                 //
///////////////////////////////////////////////////////////////////////////////

#include "ScopeStack.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include <string>
#include <sstream>

bool isSpecialKeyWord(const std::string& tok)
{
  const static std::string keys[]
    = { "for", "while", "switch", "if", "catch" };
  for(int i=0; i<5; ++i)
    if(tok == keys[i])
      return true;
  return false;
}

struct element
{
  std::string name;
  std::string type;
  std::string show()
  {
    std::ostringstream out;
    out << "\n  (" << type << ", " << name << ")";
    return out.str();
  }
};

int main(int argc, char* argv[])
{
  std::cout << "\n  Demonstrating Scope Analysis\n "
            << std::string(30,'=') << std::endl;
  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');
    try
    {
  	  Toker toker(argv[i]);
      SemiExp se(&toker);
      ScopeStack<element> stack;
      while(se.get())
      {
        if(se.length() < 1)
          continue;
        //std::cout << se.show().c_str() << std::endl;
        if(se[se.length() - 1] == "}")
        {
          stack.pop();
          showStack<element>(stack);
        }
        if(se.length() < 3)
          continue;
        if(se[se.length() - 1] == "{")
        {
          element item;
          size_t pos = se.find("(");
          if(pos < se.length())
          {
            if(isSpecialKeyWord(se[pos-1]))
              item.type = "control";
            else
              item.type = "function";
            item.name = se[pos-1];
            stack.push(item);
            showStack<element>(stack);
          }
          else
          {
            item.type = se[se.length()-3];
            item.name = se[se.length()-2];
            stack.push(item);
            showStack<element>(stack);
          }
        }
      }
      std::cout << "\n\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }
}
