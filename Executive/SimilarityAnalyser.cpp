/////////////////////////////////////////////////////////////////////
//  SimilarityAnalyser.cpp - Analyses similarity between scope structures      //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Similarity analysis, OOD Assignment 2, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////

#include "SimilarityAnalyser.h"
//Constructs analyser instance with a set of nodes
SimilarityAnalyser::SimilarityAnalyser(std::vector<Node*> argNodes) : nodes(argNodes) {
}

//Analyses similarity in scope structure between a set of nodes
void SimilarityAnalyser::analyse(){
	sort();

	if (nodes.size() == 0){
		return;
	}

	for (unsigned int i = 0; i < (nodes.size() - 1); i++){
		Treenode& node1 = nodes[i]->value();
		int complexity_node1 = (node1).getComplexity();
		if (complexity_node1 <= 2){
			break;
		}
		size_t size1 = node1.getSize(); std::string type1 = node1.getType();
		for (unsigned int j = i + 1; j < nodes.size(); j++){
			Treenode& node2 = nodes[j]->value();
			if (nodes[i]->compareNodes(nodes[j])){
				node1.addSimilarNode(node2);
			}
		}

	}
}

//Sorts the set of nodes according to complexity
void SimilarityAnalyser::sort(){
	std::sort(nodes.begin(), nodes.end(), [](Node * elem1, Node * elem2) -> bool { return elem1->value().getComplexity() > elem2->value().getComplexity(); });
}

// test Stub
#ifdef TEST_SIMILARITY
#include "../FileManager/FileManager.h"
#include "../ConfigureParser.h"
#include "../Display/Display.h"
int main(int argc, char* argv[])
{
	if (argc < 2){
		std::cout << "\n Usage: [/s] [/b] path [pattern]";
		return 0;//check this
	}
	for (int i = 0; i < argc; i++){
		std::cout << "\n" << argv[i];
	}

	FileManager fileManager;fileManager.parseCmdLine(argc, argv);
	std::vector<std::string> files = fileManager.listFiles();
	std::vector<Node*> nodes;

	for (std::vector<std::string>::iterator itr = files.begin(); itr != files.end(); ++itr){
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try{
			if (pParser){
				if (!configure.Attach(*itr)){
					std::cout << "\n  could not open file " << *itr << std::endl;
					continue;
				}
			}
			else{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			// now that parser is built, use it
			std::cout << "\n\nfile: " << *itr << std::endl;

			while (pParser->next())
				pParser->parse();
			std::vector<Node*> scopes = configure.getAnalysedScopes();
			for (auto scope : scopes){
				scope->value().setFilename(*itr);
				nodes.push_back(scope);
			}
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	SimilarityAnalyser analyser(nodes); analyser.analyse();
	Display display(nodes); bool isShowDiff = fileManager.isShowDifferences();
	display.showAnalysisResults(isShowDiff);
	return 0;
}
#endif