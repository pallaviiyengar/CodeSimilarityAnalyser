/////////////////////////////////////////////////////////////////////
//  Executive.cpp - Makes call to the FileManager, Parser and      //
//					Display	classes								   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "../FileManager/FileManager.h"
#include "../ConfigureParser.h"
#include <algorithm>
#include "../Display/Display.h"

#include "../ChronoTimer/ChronoTimer/ChronoTimer.h"
#include "SimilarityAnalyser.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef TEST_EXECUTIVE

int main(int argc, char* argv[]){
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Timer timer; timer.start();
	try{
		FileManager fileManager;
		fileManager.parseCmdLine(argc, argv);
		std::vector<std::string> files = fileManager.listFiles(); std::vector<Node*> nodes; std::vector<Tree*> trees;
		for (auto file : files){
			try	{
				if (fileManager.excludeFileFromAnalysis(file)) //Exclude Non-programming files
					continue;
				//std::cout << "\n Analysing File: " << file << std::endl;
				ConfigParseToConsole configure;
				Parser* pParser = configure.Build();
				if (pParser){
					if (!configure.Attach(file)){
						std::cout << "\n  could not open file " << file << std::endl;
						continue;
					}
				}
				else{
					std::cout << "\n\n  Parser not built\n\n";
					return 1;
				}				
				while (pParser->next()) 
					pParser->parse();
				std::vector<Node*> scopes = configure.getAnalysedScopes();
				for (auto scope : scopes){
					scope->value().setFilename(file);
					nodes.push_back(scope);
				}
				trees.push_back(configure.getLargestTree());
			}
			catch (std::exception& ex){
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
		}
		SimilarityAnalyser analyser(nodes); analyser.analyse();
		timer.stop();
		std::cout << "\nTime taken for analysis in milli seconds = " << timer.elapsedTime<std::chrono::milliseconds>()<<"\n";
		Display display(nodes); bool isShowDiff = fileManager.isShowDifferences();
		display.showAnalysisResults(isShowDiff);
		std::cout << "\n\n";
		for (auto tree : trees) delete tree;
	}
	catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif