/////////////////////////////////////////////////////////////////////
//  MTreeNode.h - This module defines a custom tree node with each // 
//  node composed of type"element" and a pointer to parent node    //
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
This module defines a custom tree node with each node composed of type
"element" and a pointer to parent node

Public Interface:
=================
element elem;
Treenode tn(elem);
Treenode tn2(elem);

tn.setElement(elem);
tn.setSize(0, 10);//start line, end line
tn.setComplexity(1);
tn.setFilename("myFile.cpp");
tn.addSimilarNode(tn2);

element elem = tn.getElement();
string type = tn.getType();
size_t size = tn.getSize();
string name = tn.getName();
string file = tn.getFilename();
int complexity = tn.getComplexity();
std::vector<Treenode> nodes = tn.getSimilarNodes();

cout<<tn; //prints out name, size, type, complexity of the element of the node

Build Process:
==============
Required files
- MNode.h, MTree.h, element.h
Build commands (either one)
- devenv MTree.sln

Maintenance History:
====================
ver 2.0: 21 Mar 2014
- Added functionality to add nodes similar in scope structure to a given node
- Added functionality to get list of similar nodes to a given node
- Added functionality to set and get the filename to which this node belongs to

ver 1.0 : 11 Feb 2014
- first release

*/
//
#ifndef MTREENODE_H
#define MTREENODE_H
#include<iostream>
#include "MTree.h"
#include "../../element.h"

class Treenode{
private:
	element _elem;
	size_t _size;
	std::string filename;
	std::vector<Treenode> similarNodes;
public:
	Treenode::Treenode(element elem){
		this->_elem = elem;
		_size = 0;
	}

	//Get element comprising the node
	element getElement(){
		return _elem;
	}

	//Get element type of the node
	std::string getType(){
		return _elem.type;
	}

	//Get element name of the node
	std::string getName(){
		return _elem.name;
	}

	//Get size i.e. line count of the node
	size_t getSize(){
		return (_size);
	}

	//Get complexity of the node
	int getComplexity(){
		return _elem.complexity;
	}

	//Set element compromising the node
	void setElement(element elem){
		this->_elem = elem;
	}
	//Set element size of the node
	void setSize(size_t start_line, size_t end_line){

		this->_size = (end_line - start_line + 1);
		this->_elem.endLine = end_line;
		this->_elem.lineCount = start_line;
	}

	//Set complexity of the element of the node
	void setComplexity(int complexity){
		this->_elem.complexity = complexity;
	}

	//Overloaded == operator to compare with string
	bool operator ==(std::string str){
		if (this->getName() == str){
			return true;
		}
		else{
			return false;
		}
	}

	//set the filename to which this node belongs to
	void setFilename(std::string filename){
		this->filename = filename;
	}

	//get the filename to which this node belongs to
	std::string getFilename(){
		return this->filename;
	}

	//----< add similar node >---------------------------------------------------
	void addSimilarNode(Treenode pNode){
		similarNodes.push_back(pNode);
	}

	//----< get list of similar nodes >---------------------------------------------------
	std::vector<Treenode> getSimilarNodes(){
		return similarNodes;
	}

	//Overloaded operator << to print contents of node to the output
	friend std::ostream& operator<<(std::ostream &stream, Treenode& tn){
		stream << "Type: " << tn.getType()
			<< ", Name: " << tn.getName()
			<< ", Size: " << tn.getSize()
			<<", Complexity" << tn.getComplexity();
		return stream;
	}
};
#endif
