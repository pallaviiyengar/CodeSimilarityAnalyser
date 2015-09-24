/////////////////////////////////////////////////////////////////////
//  element.h - declares structure to store analysed elements and 
//  constants            for keywords
//  ver 2.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09    
//  Author:		   Pallavi Iyengar
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_H
#define ELEMENT_H

#include<string>
#include <sstream>
// ScopeStack element is application specific

struct element
{
	std::string type;
	std::string name;
	size_t lineCount; //start line
	size_t endLine;
	int complexity;

	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ", ";
		temp << endLine;
		temp << ")";
		return temp.str();
	}
};

//Constants for keywords
const static std::string TYPE_KEYS[] = { "class", "struct", "enum", "namespace", "union" };
const static std::string LOOP_KEYS[] = { "for", "while", "switch", "if", "catch", "try", "do", "else" };

#endif