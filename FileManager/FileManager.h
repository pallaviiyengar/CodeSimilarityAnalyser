#ifndef FILEMANAGER_H
#define FILEMANAGER_H
/////////////////////////////////////////////////////////////////////
//  FileManager.h - Parses command line and lists files in a       //
//  specified path, according to a specified pattern,recursively   //
//  or not depending on the option provided						   //
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
This module defines a class that provides services necessary to parse
a command line and list files to be scope analysed.
It identifies a set of files by parsing the following command line  arguments:
1. a Path
2. One or more file pattern
3. /s or /S, if present, the entire directory rooted at the path is searched
4. /b, if present, shows only function size and complexity of analysed functions
5. /d, shows differences between similar scopes

Public Interface:
=================
FileManager fm;										//Get FileManager Instance
fm.parseCmdLine(argc, argv);						//Parses command line arguments and searches files according to pattern and switch '/s' or '/S' for recursive listing
std::vector<std::string> files = fm.listFiles();	//Search and Retrieve the files listed according to the command line arguments
bool isDisplayLess = fm.showFuncSizeAndComplexityOnly();//Returns if switch '/b' is passed, for showing only function size and complexity,
bool isShowDifferences = fm.isShowDifferences();//Returns if switch '/d' is present to show differences between similar scopes
Build Process:
==============
Required files
- FileSystem.h, FileSystem.cpp
Build commands (either one)
- devenv FileManager.vcxproj

Maintenance History:
====================
ver 2.0: 24 Mar 2014
- added option to show between similar scopes
ver 1.0 : 11 Feb 2014
- first release

*/
//
#include "FileSystem.h"
using namespace FileSystem;

class FileManager{
private:
	std::vector<std::string> files;
	std::string path;
	std::vector<std::string> searchPatterns;

	bool bShowFuncSizeComplexity, bListRecursively, bShowdifferences;
	void searchFiles(std::string path, std::string pattern, bool listRecursively);

public:
	FileManager();
	void parseCmdLine(int argc, char* argv[]);
	std::vector<std::string> listFiles();
	bool showFuncSizeAndComplexityOnly();
	bool isShowDifferences();
	bool excludeFileFromAnalysis(std::string path);

};
#endif