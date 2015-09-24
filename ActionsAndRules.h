#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09    
//  Author:		   Pallavi Iyengar
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 3.0: 13 March 2014
  - Modified to build, store and get a tree for an entire file.
  Any encountered scope elements are stored as nodes of the tree
 
 ver 3.0: 11 Feb 2014
  - added rules and actions to detect functions, loops, class, struct, enums and namespaces
  - added logic to build a tree for a function with scopes encountered as nodes
  - added logic to store the largest tree
  - added logic to count the complexity of a function

  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "./MTree/MTree/MNode.h"
#include "./MTree/MTree/MTree.h"
#include "element.h"
#include "./MTree/MTree/TreeNode.h"

typedef TMTree::MNode<Treenode> Node;
typedef TMTree::MTree<Node> Tree;
///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
class Repository  // application specific
{
  ScopeStack<element> stack;
  Toker* p_Toker;
  std::vector<Node*> analysedScopes;
  unsigned int highestNodeCount;
  Tree* curTree;
  Node* curParentInTree;

public:

  Repository(Toker* pToker) : curParentInTree(NULL), highestNodeCount(0)
  {
    p_Toker = pToker;

	class newOper : public TMTree::Operation<Node>
	{
	public:
		bool operator()(Node* pNode)
		{
			if (verbose_){
				std::cout << "\n -- " << pNode->ToString();
			}
			return  false;
		}
	};
	curTree = new Tree(new newOper);
	element elem; elem.type = "unknown"; elem.name = "root";
	elem.lineCount = 0; elem.endLine = 0; elem.complexity = 0;

	Treenode root(elem);
	Node*  rootNode = new Node(root);
	rootNode->setParent(NULL);
	curParentInTree = rootNode;

	curTree->makeRoot(rootNode);
	
  }

  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->lines());
  }
 
  // Returns vector of all the elements analysed
  std::vector<Node*>& getanalysedScopes(){
	return analysedScopes;
  }

  /*Returns the current node in the tree for a function,
  to which future encountered nodes added until this element's scope ends*/
  Node* getCurNodeInTree(){
	  return curParentInTree;
  }

  /*Sets as the current parent node to which future encountered nodes
  added until the node's element's scope ends*/
  void setCurNodeInTree(Node* node){
	  curParentInTree = node;
  }
  
  /*Constructs a node from the passed element and sets it as the current 
  parent node to which future encountered nodes are added until the node's 
  element's scope ends
  Note: Elements are added only if they come under scope of function*/
  void setCurNodeInTree(element elem){
	  if (curParentInTree != NULL){
		  Treenode child(elem);
		  Node*  childNode = new Node(child);
		  childNode->setParent(curParentInTree);
		  (curParentInTree)->addChild(childNode);
		  curParentInTree = childNode;
	  }
  }

  /*
  Sets the largest node count encountered till now
  */
  void setHighestNodeCount(int nodeCount){
	  highestNodeCount = nodeCount;
  }

  /*
  Gets the current largest node count
  */
  unsigned int getHighestNodeCount(){
	  return highestNodeCount;
  }

  /*
  Returns the current tree of the file
  */
  Tree* getTree(){
	  return curTree;
  }

  /*
  Sets a tree as the largest function tree in a given file
  */
  /*void setTree(Tree* tree){
	  curTree = tree;
  }*/
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
	elem.endLine = 0;
	elem.complexity = 0;
    p_Repos->scopeStack().push(elem);

  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }

  /*void storeTreeIfLargest(element& elem){
	  Node* curElemInTree = p_Repos->getCurNodeInTree();
	  if (curElemInTree != NULL){
		  //Construct Tree
		  class newOper : public TMTree::Operation<Node>
		  {
		  public:
			  bool operator()(Node* pNode)
			  {
				  if (verbose_){
					  std::cout << "\n -- " << pNode->ToString();
				  }
				  return  false;
			  }
		  };

		  Tree* tree = new Tree(new newOper);
		  tree->makeRoot(curElemInTree);
		  tree->verbose() = false;
		  tree->walk(curElemInTree);

		  //Get Node Count and set complexity
		  size_t node_count = tree->getSize();
		  elem.complexity = node_count;
		  curElemInTree->value().setComplexity(node_count);

		  //Set Current Largest Tree
		  if (node_count > p_Repos->getHighestNodeCount()){
			  p_Repos->setHighestNodeCount(node_count);

			  if (p_Repos->getTree() != NULL){
				  delete (p_Repos->getTree());
			  }
			  p_Repos->setTree(tree);
		  }
		  else{
			  delete tree;
			  tree = NULL;
		  }
	  }
  }
  */
  void doAction(ITokCollection*& pTc)
  {
	  try{
		  if (p_Repos->scopeStack().size() == 0)
			  return;

		  element elem = p_Repos->scopeStack().pop();
		  elem.endLine = p_Repos->lineCount();

		  //If element is not detected as any type(like an array intialization etc)
		  if (elem.type != "unknown"){
			  Node* curNode = p_Repos->getCurNodeInTree();
			  if (curNode != NULL){
				  curNode->value().setSize(elem.lineCount, elem.endLine);
				  
				 //Storing complexity for special cases like function inside a class defined in a function
				Tree tree;
				size_t node_count = tree.getSize(curNode);
				elem.complexity = node_count;
				curNode->value().setComplexity(node_count);
				(p_Repos->getanalysedScopes()).push_back(curNode);

				//Current element being popped out will be the current parent in tree. 
				 //Pop it out and move one level up to it's parent.
				 Node* parent = curNode->getParent();
				if (parent != NULL){
					 p_Repos->setCurNodeInTree(parent);
				 }
			  }
		 }
		
	
		  
	  }
	  catch (std::exception& ex){
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
  }
};


///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--PreprocStatement rule";
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        //std::cout << "\n--FunctionDefinition rule";
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    p_Repos->scopeStack().pop();

    // push function scope
	size_t pos =  pTc->find("(");
    std::string name = (*pTc)[pos - 1];
	if ((pos - 2) >= 0 && ((*pTc)[pos - 2] == "~" || (*pTc)[pos - 2] == "::~" || (*pTc)[pos - 2] == ">::~")){
		name = "~" + name; //For destructor
	}
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
	elem.endLine = 0;
	elem.complexity = 0;
    p_Repos->scopeStack().push(elem);
	
	try{
		//p_Repos->setCurFuncComplexity(0);
		/*Node* curNode = p_Repos->getCurNodeInTree();
		if (curNode == NULL){
			Treenode root(elem, NULL);
			p_Repos->setCurNodeInTree(new Node(root));
		}
		else{	
			//Set as cur node to add future child nodes until scope ends
			p_Repos->setCurNodeInTree(elem);
		}
		*/

		p_Repos->setCurNodeInTree(elem);

	}
	catch (std::exception& ex){
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}

  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect Keywords like for, while, switch, if, catch, do, try, else, anonymous scope

class LoopKeyword : public IRule
{
public:
	bool isLoopKeyword(ITokCollection& tok)
	{
		for (auto key : LOOP_KEYS){
			if (tok.find(key) < tok.length()){
				return true;
			}
		}
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;

		if (tc[tc.length() - 1] == "{")
		{
			if (tc.length() == 1){
				doActions(pTc);
				return true;
			}else{
				if (isLoopKeyword(tc))
				{
					doActions(pTc);
					return true;
				}
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push special keywords like for, while, switch, if, catch, do, try, else, and anonymous scopes

class PushLoopKeyword : public IAction
{
	Repository* p_Repos;
public:
	PushLoopKeyword(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();
		std::string type;
		// push  scope
		if (pTc->length() == 1){
			type = "anonymous";
		}
		else{
			size_t pos;
			for (auto key : LOOP_KEYS){
				if ((pos = (*pTc).find(key)) < (*pTc).length()){
					break;
				}
			}
			type = (*pTc)[pos];
		}

		element elem;
		elem.type = type;
		elem.name = type;
		elem.lineCount = p_Repos->lineCount();
		elem.endLine = 0;
		elem.complexity = 0;
		p_Repos->scopeStack().push(elem);
	
		//For Tree
		try{
			//Set as current node to add future child nodes until scope ends
			p_Repos->setCurNodeInTree(elem);
		}
		catch (std::exception& ex){
			std::cout << "\n\n  " << ex.what() << "\n\n";
		}

	}
};

///////////////////////////////////////////////////////////////
// rule to detect class, struct, enum, namespace, union
//////////////////////////////////////////////////////////////
class TypeKeyword : public IRule
{
public:
	bool searchTypeKeyWord(ITokCollection& pTc, const unsigned int posCurlyBrace)
	{
		for (auto key : TYPE_KEYS){
			if (pTc.find(key) < posCurlyBrace){
				return true;
			}
		}
		return false;
	}

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			unsigned int posCurlyBrace = tc.length() - 1;
			if (searchTypeKeyWord(tc, posCurlyBrace)){
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push keywords like class, struct, enum, namespace, union onto ScopeStack

class PushTypeKeyword : public IAction
{
	Repository* p_Repos;
public:
	PushTypeKeyword(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		size_t len;
		for (auto key : TYPE_KEYS){
			len = (*pTc).find(key);
			if (len < (*pTc).length()){
				break;
			}
		}

		std::string name = "anonymous";
		std::string type = "anonymous";
		
		//Checking if not of anonymous type and storing the name
		size_t pos_name = len + 1;
		if ((pos_name < (*pTc).length()) &&
			((*pTc)[pos_name] != "{")){
			name = (*pTc)[len + 1];
		}
		type = (*pTc)[len];

		element elem;
		elem.type = type;
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		elem.endLine = 0;
		elem.complexity = 0;
		p_Repos->scopeStack().push(elem);


		//Set as current node to add future child nodes until scope ends
		p_Repos->setCurNodeInTree(elem);
	}
};



#endif
