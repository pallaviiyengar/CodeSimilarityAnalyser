/////////////////////////////////////////////////////////////////////
//  FileManager.cpp - Parses command line and lists files		   // 
//					  accordingly                                  //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <vector>
using namespace FileSystem;

FileManager::FileManager() : bShowFuncSizeComplexity(false), bListRecursively(false), bShowdifferences(false){}

//----< parses command line arguments>--------------------------------
void FileManager::parseCmdLine(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++){
		std::string tmpArg = argv[i];
		if (tmpArg == "/s" || tmpArg == "/S"){
			bListRecursively = true;
		}
		else if (tmpArg == "/b"){
			bShowFuncSizeComplexity = true;
		}
		else if (tmpArg=="/d"){
			bShowdifferences = true;
		}
		else if (path.empty()){
			path = Path::getFullFileSpec(argv[i]);
		}
		else{
			searchPatterns.push_back(tmpArg);
		}
	}

	if (path.empty()){
		path = Path::getFullFileSpec(".");  // default path
	}

	if (searchPatterns.empty()){
		searchPatterns.push_back("*.*"); // default pattern
	}
}

//----< Lists the file recursively, if listRecursively is true >--------------------------------
void FileManager::searchFiles(std::string path, std::string pattern, bool listRecursively)
{
	if (!Directory::exists(path))
	{
		std::ostringstream out;
		out << "Path " << path << " does not exist";
		throw std::exception(out.str().c_str());
	}
	std::vector<std::string> tmpFiles = Directory::getFiles(path, pattern);
	for (auto tmpFile : tmpFiles){
		//files.push_back(path + "\\" + tmpFile);
		std::string filePath = FileSystem::Path::fileSpec(path, tmpFile);
		//std::cout << "\n File path :" << filePath;
		files.push_back(filePath);
	}

	if (listRecursively){
		std::vector<std::string> dirs = Directory::getDirectories(path);
		for (auto dir : dirs){
			if (dir == "." || dir == ".."){
				continue;
			}
			std::string dirPath = FileSystem::Path::fileSpec(path, dir);
			searchFiles(dirPath, pattern, listRecursively);
		}
	}

}

// searches and returns list of files found
std::vector<std::string> FileManager::listFiles(){
	for (auto pattern : searchPatterns){
		searchFiles(path, pattern, bListRecursively);
	}
	return files;
}

// returns if switch '/b' for displaying only function size and complexity is passed
bool FileManager::showFuncSizeAndComplexityOnly(){
	return bShowFuncSizeComplexity;
}

// returns if switch '/d' for displaying differences between similar scopes is present
bool FileManager::isShowDifferences(){
	return bShowdifferences;
}

// exclude non programming files from analysis
bool FileManager::excludeFileFromAnalysis(std::string file){
	std::string extension = Path::getExt(file);
	std::string exclude_exts[] = {"sdf", "obj", "tlog", "zip", "sln", "xml", "pdb", "idb", "exe", "vcproj", "vcxproj", "ilk", "suo", "filters","user", "dat", "bat"};
	bool isSkip = false;
	for (auto ext : exclude_exts)
	{
		if (ext == extension){ 
			//std::cout << "\nskipping file: "<< file<<"\n";
			isSkip = true;
			break; 
		}
	}
		
	return isSkip;
}
//test Stub
#ifdef TEST_FILE_MANAGER
int main(int argc, char* argv[])
{
	try{
		FileManager fileManager;
		fileManager.parseCmdLine(argc, argv);
		std::vector<std::string> files = fileManager.listFiles();
		std::cout << "\nFound files:";
		for (auto file : files){
			std::cout << "\n " << file;
		}
		bool isDisplayLess = fileManager.showFuncSizeAndComplexityOnly();
		std::cout << "\nDisplay only function size and complexity: " << (isDisplayLess ? "true" : "false") << "\n\n";
		std::cout << "Demonstrating with cmd line args in the order Path, Pattern and Options:";
		FileManager fileManager3;
		int nCmdParams = 6;
		char* cmdParams3[] = { "", "..//TEST", "*.cpp", "*.h", "/s", "/b" };
		for (int i = 1; i < nCmdParams; i++){
			std::cout << "\n arg:" << cmdParams3[i];
		}
		fileManager3.parseCmdLine(nCmdParams, cmdParams3);
		files = fileManager3.listFiles();
		std::cout << "\nFound files:";
		for (auto file : files){
			std::cout << "\n " << file;
		}
		isDisplayLess = fileManager3.showFuncSizeAndComplexityOnly();
		std::cout << "\nDisplay only function size and complexity: " << (isDisplayLess ? "true" : "false") << "\n\n";
		std::cout << "Demonstrating with options '/s', /b' appearing anywhere on the command line:";
		FileManager fileManager2; nCmdParams = 6;
		char* cmdParams2[] = { "", "/b", "..//TEST", "*.cpp", "*.h", "/s" };
		for (int i = 1; i < nCmdParams; i++){
			std::cout << "\n arg:" << cmdParams2[i];
		}
		fileManager2.parseCmdLine(nCmdParams, cmdParams2);
		files = fileManager2.listFiles();
		std::cout << "\nFound files:";
		for (auto file : files){
			std::cout << "\n " << file;
		}
		isDisplayLess = fileManager2.showFuncSizeAndComplexityOnly();
		std::cout << "\nDisplay only function size and complexity: " << (isDisplayLess ? "true" : "false");
		std::cout << "\n\n";
	}
	catch (std::exception ex){
	std::cout << ex.what();
	}
	return 0; 
}
#endif
