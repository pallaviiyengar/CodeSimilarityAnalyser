/////////////////////////////////////////////////////////////////////
//  Display.cpp - Displays the output in the specified format      //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "Display.h"
#include <iomanip>
#include <algorithm>
#include "..\Executive\SimilarityAnalyser.h"

// Constructs Display instance
Display::Display(std::vector<Node*> scopes) :_scopes(scopes),_showFuncSizeAndComplexityOnly(false){
}

//Show similarity analysis results according to the option of showing differences passed
void Display::showAnalysisResults(bool isShowDiff){
	if (isShowDiff){
		showDifferenceResults();
	}
	else{
		showSimilarityResults();
	}
}

//Shows only similar scopes with line number and filenames
void Display::showSimilarityResults(){
	//std::sort(_scopes.begin(), _scopes.end(), [](Node * elem1, Node * elem2) -> bool { return elem1->value().getFilename() < elem2->value().getFilename(); });
	std::cout << "\nUse /d option to see differences between similar scopes\n";

	std::cout << "\nDisplaying similar scopes with complexity greater than 2:";
	std::cout << "\n" << std::string(75, '-')<<"\n";

	for (unsigned int i = 0; i < (_scopes.size()); i++){
		Treenode node1 = _scopes[i]->value();
		std::vector<Treenode> similarNodes = node1.getSimilarNodes();

		for (Treenode node2 : similarNodes){
			showSimilarity(node1, node2);
		}
	}
}

//Shows similar scopes with differences between them
void Display::showDifferenceResults(){
	//std::sort(_scopes.begin(), _scopes.end(), [](Node * elem1, Node * elem2) -> bool { return elem1->value().getFilename() < elem2->value().getFilename(); });

	std::cout << "\nDisplaying similar scopes(with complexity greater than 2) and their differences:";
	std::cout << "\n" << std::string(75, '-') << "\n";

	for (unsigned int i = 0; i < (_scopes.size()); i++){
		Treenode node1 = _scopes[i]->value();
		std::vector<Treenode> similarNodes = node1.getSimilarNodes();

		for (Treenode node2 : similarNodes){
			showDifferences(node1, node2);
		}
	}
}


//Prints out similarity bewtween nodes info such as line number and filename
void Display::showSimilarity(Treenode tn1, Treenode tn2){
	std::string filename1 = tn1.getFilename();
	size_t start_line1 = tn1.getElement().lineCount;
	size_t end_line1 = tn1.getElement().endLine;

	std::string filename2 = tn2.getFilename();
	size_t start_line2 = tn2.getElement().lineCount;
	size_t end_line2 = tn2.getElement().endLine;
	
	std::cout << "\nFilename1: " << filename1 << " ,Start Line: " << start_line1 + 1 << ", End Line:" << end_line1 + 1;
	std::cout << "\nFilename2: " << filename2 << " ,Start Line: " << start_line2 + 1 << ", End Line:" << end_line2 + 1<<"\n";
}


//Prints out useful information about differences between similar scopes such as line numbers and filenames
void Display::showDifferences(Treenode tn1, Treenode tn2){
	std::string filename1 = tn1.getFilename();
	size_t start_line1 = tn1.getElement().lineCount;
	size_t end_line1 = tn1.getElement().endLine;

	std::string filename2 = tn2.getFilename();
	size_t start_line2 = tn2.getElement().lineCount;
	size_t end_line2 = tn2.getElement().endLine;

	size_t size1 = end_line1 - start_line1+1;
	if (size1 < 0){
		size1 = 0;
	}
	size_t size2 = end_line2 - start_line2+1;
	if (size2 < 0){
		size2 = 0;
	}
	std::cout << "\nFilename1: " << filename1 << " ,Start Line: " << start_line1 + 1 << ", End Line: " << end_line1 + 1<<" , Size: "<<size1;
	std::cout << "\nFilename2: " << filename2 << " ,Start Line: " << start_line2 + 1 << ", End Line: " << end_line2 + 1<<" , Size: "<<size2;
	std::cout << "\nSize Difference: " << abs((int)size2 - (int)size1)<<"\n";
}

// Constructs Display instance
Display::Display(std::string file, std::vector<Node*> scopes, bool showFuncSizeAndComplexityOnly){
	this->_file = file;
	this->_scopes = scopes;
	this->_showFuncSizeAndComplexityOnly = showFuncSizeAndComplexityOnly;
}

// Prints XML to standard output
void Display::printXML(XmlWriter wtr){
	std::cout << wtr.xml();
}

// Prints output of scope analysis to the standard output. Either displays information about
// all analysed elements or only prints function information .
void Display::printOutput(){
	bool printHeader = false;

	for (auto scope : _scopes){
		if ( _showFuncSizeAndComplexityOnly ){
			if (!printHeader){
				printHeader = true;
				printFuncSizeComplexityHeader();
			}
			if (scope->value().getType() == "function"){
				printFuncSizeComplexity(scope);
			}
		}
		else{
			if (!printHeader){
				printHeader = true;
				printRegularOutputHeader();
			}
			printRegularOutput(scope);
		}
	}

	printTreeNodeCount();
}

//Prints function and tree node count during regular display mode
void Display::printTreeNodeCount(){
	bool printHeader = false;
	for (auto scope : _scopes){
		if (!_showFuncSizeAndComplexityOnly){
			if (scope->value().getType() == "function"){
				if (!printHeader){
					printHeader = true;
					std::cout << "\n\n";
					std::cout << " " << std::left << std::setw(30) << "Name";
					std::cout << std::left << std::setw(15) << "Complexity(Tree Node Count)";
					std::cout << "\n " <<std::string(75, '-');
				}

				std::cout << "\n";
				std::cout << " " << std::left << std::setw(30) << scope->value().getName();
				std::cout << std::left << std::setw(15) << scope->value().getComplexity();
			}
		}
	}
}

//Prints header for regular display
void Display::printRegularOutputHeader(){
	std::cout << "\n";
	std::cout << " " << std::left << std::setw(30) << "Name";
	std::cout << std::left << std::setw(20) << "Type";
	std::cout << std::left << std::setw(15) << "start line";
	std::cout << std::left << std::setw(15) << "End Line";
	std::cout << "\n " << std::string(75, '-');
}

//Prints name, type, start line and end line of analysed elements
void Display::printRegularOutput(Node* elem){
	std::cout << "\n";
	std::cout << " " << std::left << std::setw(30) << elem->value().getName();
	std::cout << std::left << std::setw(20) << elem->value().getType();
	std::cout << std::left << std::setw(15) << elem->value().getElement().lineCount+1;
	std::cout << std::left << std::setw(15) << elem->value().getElement().endLine + 1;
}

//Prints header for showing function information
void Display::printFuncSizeComplexityHeader(){
	std::cout << "\n";
	std::cout << " " << std::left << std::setw(30) << "Name";
	std::cout << std::left << std::setw(15) << "Size";
	std::cout << std::left << std::setw(15) << "Complexity(Tree Node Count)";
	std::cout << "\n " << std::string(75, '-');
}

//Prints each analysed functions name, size and complexity
void Display::printFuncSizeComplexity(Node* elem){
	//if (elem->value().getType() == "function"){
		std::cout << "\n";
		std::cout << " " << std::left << std::setw(30) << elem->value().getName();
		std::cout << std::left << std::setw(15) << elem->value().getSize();
		std::cout << std::left << std::setw(15) << elem->value().getComplexity();
	//}
}

// test Stub
#ifdef TEST_DISPLAY
#include "../FileManager/FileManager.h"
#include "../ConfigureParser.h"
int main(int argc, char* argv[])
{
	if (argc < 2){
		std::cout << "\n Usage: [/s] [/b] path [pattern]";
		return 0;//check this
	}
	for (int i = 0; i < argc; i++){
		std::cout << "\n" << argv[i];
	}

	FileManager fileManager;
	fileManager.parseCmdLine(argc, argv);
	std::vector<std::string> files = fileManager.listFiles();
	std::vector<Node*> nodes;
	for (std::vector<std::string>::iterator itr = files.begin(); itr != files.end(); ++itr){
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try		{
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
			while (pParser->next())pParser->parse();
			std::vector<Node*> scopes = configure.getAnalysedScopes();
			for (auto scope : scopes){
				scope->value().setFilename(*itr);
				nodes.push_back(scope);
			}
			bool showFuncSizeandComplexityOnly = fileManager.showFuncSizeAndComplexityOnly();
			Display display(*itr, scopes, showFuncSizeandComplexityOnly);
			display.printOutput();		
		}
		catch (std::exception& ex){ std::cout<<"\n"<<ex.what();}
	}
	SimilarityAnalyser analyser(nodes); analyser.analyse();
	Display display(nodes);bool isShowDiff = fileManager.isShowDifferences();display.showAnalysisResults(isShowDiff);
	return 0;
}
#endif