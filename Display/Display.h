#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////
//  Display.h - Displays analysed scopes in required format		   //		  
//																   // 
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that provides services necessary to display analysed scopes of a file.

Public Interface:
=================
Display display(filepath, scopes, showFuncSizeandComplexityOnly);  //Initialise Display instance with analysed scopes,format and file being analysed to display in
display.printOutput();									 //Display the analysed scopes
disply.printXML();										 //Displays XML to the standard output

Display display(nodes); // Intialises Display instance for showing similarity analysis results
bool isShowDiff = true;
display.showAnalysisResults(isShowDiff);// Prints out useful info about similar scopes such as line numbers and filename. Shows differences between those scopes, if passed isShowDiff as true.

Build Process:
==============
Required files
- Display.h, Display.cpp
Build commands
- devenv Display.sln

Maintenance History:
====================

ver 2.0 : Mar 21, 2014
- Added displaying output for scope similarity analysis 
	- Shows similarity between scopes
	- Shows Differences between similar scopes

ver 1.0 : 11 Feb 2014
- first release

*/

#include<vector>
#include<iostream>
#include "../element.h"
#include "../XmlWriter/XmlWriter/XmlWriter.h"
#include "../MTree/MTree/MNode.h"
#include "../MTree/MTree/MTree.h"
#include "../MTree/MTree/TreeNode.h"
//typedefs
typedef TMTree::MNode<Treenode> Node;
typedef TMTree::MTree<Node> Tree;
class Display{
public:

	typedef std::vector<Node*> elements;
	Display(std::string file, elements scopes, bool showFuncSizeAndComplexityOnly);
	Display(std::vector<Node*> scopes);

	void printOutput();
	void printXML(XmlWriter writer);
	
	void showAnalysisResults(bool isShowDiff);

private:
	elements _scopes;
	std::string _file;
	bool _showFuncSizeAndComplexityOnly;
	void printRegularOutputHeader();
	void printRegularOutput(Node* elem);
	void printFuncSizeComplexityHeader();
	void printFuncSizeComplexity(Node* elem);
	void printTreeNodeCount();
	void showSimilarityResults();
	void showDifferenceResults();
	void showSimilarity(Treenode tn1, Treenode tn2);
	void showDifferences(Treenode tn1, Treenode tn2);

};
#endif