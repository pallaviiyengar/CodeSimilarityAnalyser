/////////////////////////////////////////////////////////////////////
//  SimilarityAnalyser.h -	Analyses similarity between scope structures	   //		  
//																   // 
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Similarity analysis, OOD Assignment 2, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that handles scope similarity analysis

Public Interface:
=================
std::vector<Node*> nodes;
SimilarityAnalyser analyser(nodes);
analyser.analyse();

Build Process:
==============
Required files
- SimilarityAnalyser.h, SimilarityAnalyser.cpp
Build commands
- devenv SimilarityAnalyser.sln

Maintenance History:
====================

ver 1.0 : Mar 28, 2014
- Analyses nodes for similarity in scope structure
*/

#include "../ConfigureParser.h"
#include <algorithm>
using namespace std;

class SimilarityAnalyser{
private:
	vector<Node*> nodes;

	void sort();
	bool compareNodes(Node* node1, Node* node2);

public:
	SimilarityAnalyser(std::vector<Node*> argNodes);
	void analyse();

};