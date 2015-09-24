#ifndef MTREE_H
#define MTREE_H
///////////////////////////////////////////////////////////////////////////
// MTree.h   -  M-ary tree class                                         //
// ver 2.3                                                               //
// Language:    Visual C++, 2008                                         //
// Platform:    Dell Precision T7400, Win7 Professional                  //
// Application: CSE687 - Demo for Project #1, Spring 2010   
// Author:		Pallavi
// Source:      Jim Fawcett, Syracuse University, CST 4-187,             //
//              (315) 443-3948, jfawcett@twcny.rr.com                    //
///////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * Provides an MTree class that holds a finite number of MNodes and each
 * Node holds a finite number of child Node references.  MTree supports
 * visitation of each node, using depth first search.  
 * 
 * Required Files:
 * ===============
 * MTree.h, MTree.cpp, MNodes.h, MNode.cpp
 *
 * Build Process:
 * ==============
 * cl /D:TEST_MTREE MTree.cpp MNode.cpp
 * 
 * Maintenance History:
 * ====================
 * ver 2.4: 11 Feb 14
 * - Added functionality to get the tree size
 * - Added functionality to write out XML representation of the tree
 * ver 2.3 : 05 Jan 13
 * - minor changes to comments
 * ver 2.2 : 02 Mar 10
 * - fixed memory leak in MTree<Node>::finder(...)
 * ver 2.1 : 27 Feb 10
 * - converted all test stub testing to use MNode<T>.
 * - added public definitions for copy construction and assignment.
 * - fixed bug in searching by changing return type of walk to bool
 *   and returning as soon as a find happens.
 * ver 2.0 : 30 Jan 10
 * - removed vector scaffolding, now create test nodes on heap
 * ver 1.1 : 27 Jan 10
 * - added private declarations for copy and assignment
 *   to prevent compiler from generating incorrect versions
 * ver 1.0 : 23 Jan 09
 * - first release
 */


#include <iostream>
#include <vector>
#include <sstream>
#include "../../XmlWriter/XmlWriter/XmlWriter.h"
#include "../../element.h"

namespace TMTree
{
  /////////////////////////////////////////////////////////////////////////
  // Operation<Node> class

  template <typename Node>
  class Operation
  {
  public:
    Operation() : level_(0), verbose_(false) {}
    virtual ~Operation() {}
    size_t& level() { return level_; }
    bool& verbose() { return verbose_; }
    virtual void end(Node* pNode) {}
    virtual bool operator()(Node* pNode)
    {
      if(verbose_)
		  std::cout << "\n  " << pNode->value();
      return false;  // don't stop walk
    }
  protected:
    size_t level_;
    bool verbose_;
  };



  /////////////////////////////////////////////////////////////////////////
  // MTree<Node> class

  template <typename Node>
  class MTree
  {
  public:
    MTree(Operation<Node>* pOper=new Operation<Node>());
    MTree(const MTree<Node>& tree);
    ~MTree();
    MTree<Node>& operator=(const MTree<Node>& tree);
    Operation<Node>* setOperation(Operation<Node>* pOper);
    void makeRoot(Node* pNode);
	bool walk(Node* pStart, size_t count = 0, XmlWriter* writer=NULL);
    bool walk();
    // these two finders do the same thing in different ways
    Node* find(const std::string& value, Node* pNode = 0);
    Node* finder(const std::string& value, Node* pNode = 0);
    void clear();
    bool& verbose();
    bool isEmpty();

	void writeXML(XmlWriter& writer);
	size_t getSize(Node* node);
	size_t getSize();


  private:
    Node* pRoot;
    Node* pFound;
    Operation<Node>* pOper_;
    bool verbose_;

	//pallavi
	size_t node_count;
  };
  //----< constructor accepts operation to apply to each node >------------

  template <typename Node>
  MTree<Node>::MTree(Operation<Node>* pOper) 
    : pOper_(pOper), pRoot(0), verbose_(false), node_count(0) {}

  //----< copy constructor depends on virtual MNode::clone() >-----------

  template <typename Node>
  MTree<Node>::MTree(const MTree<Node> &tree)
  {
    pRoot = tree.pRoot->clone();
    pOper_ = tree.pOper_;
  }
  //----< destructor >-----------------------------------------------------

  template <typename Node>
  MTree<Node>::~MTree() { 

    if(verbose())
      std::cout << "\n  Deleting tree nodes:";
	
	delete pOper_;

	if (pRoot){
		//delete_walk(pRoot);
		delete pRoot;
	}
  }

  //----< assignment >-----------------------------------------------------

  template <typename Node>
  MTree<Node>& MTree<Node>::operator =(const MTree<Node>& tree)
  {
    if(this == &tree) return *this;
    delete pRoot;
    pRoot = tree.pRoot->clone();
    pOper_ = tree.pOper_;
    return *this;
  }
  //----< remove all nodes >-----------------------------------------------

  template <typename Node>
  void MTree<Node>::clear() { delete pRoot; }

  //----< set verbose true or false >----------------------------------------

  template <typename Node>
  bool& MTree<Node>::verbose() { return verbose_; }

  //----< give tree a node to serve as root of tree >------------------------

  template <typename Node>
  void MTree<Node>::makeRoot(Node* pMNode)
  {
    pRoot = pMNode;
  }
  //----< give tree an operation to apply to each node during traversal >----

  template <typename Node>
  Operation<Node>* MTree<Node>::setOperation(Operation<Node>* pOper)
  {
    Operation<Node>* temp = pOper_;
    pOper_ = pOper;
    return temp;    // return old operation so can restore later
  }
  //----< depth first search based traversal >-------------------------------

  template <typename Node>
  bool MTree<Node>::walk()
  {
    pOper_->verbose() = verbose_;
    pOper_->level() = 0;
    if((*pOper_)(pRoot)) return false;  // found something so stop walk
    pRoot->clearMarks();
    Node* pTemp;
    while(pTemp = pRoot->nextUnmarkedChild())
    {
      if(!walk(pTemp))
        return false;
      //if((*pOper_)(pTemp)) return;
      pOper_->end(pTemp);
      --(pOper_->level());
    }
    return true;
  }
  //----< traversal starting at specific node. Also counts number of nodes and if xmlwriter present writes out XML >------------------------------

  template <typename Node>
  bool MTree<Node>::walk(Node* pStart, size_t count = 0, XmlWriter* wtr = NULL)
  { 
	node_count = count+1;

    pOper_->verbose() = verbose_;
    ++(pOper_->level());
    if((*pOper_)(pStart)) return false; // found something so stop walk
    pStart->clearMarks();
    Node* pTemp;
    while(pTemp = pStart->nextUnmarkedChild())
    {
		if (wtr != NULL){
			std::string type = pTemp->value().getType();
			wtr->start(type);
			wtr->addAttribute("size", std::to_string(pTemp->value().getSize()));
			wtr->addAttribute("complexity", std::to_string(pTemp->value().getComplexity()));

		}
      if(!walk(pTemp, node_count, wtr))
        return false;
      pOper_->end(pTemp);
      --(pOper_->level());
	  if (wtr != NULL){
		  wtr->end();
	  }
    }
    return true;
  }
  //----< Operation supports finding node with given value >---------------

  template <typename Node>  
  class finderOp : public Operation<Node>
  {
  public:
    finderOp(const std::string& val, Node* pNode=0)
      : value_(val), pNode_(pNode), stop(false), verbose_(false) {}
    bool operator()(Node* pNode)
    {
      if(stop) return true;
      if(verbose())
        std::cout << "\n -- value = " << pNode->value();
      if(pNode->value() == value_)
      {  
        pNode_ = pNode;
        return (stop = true);
      }
      return false;
    }
    Node* getResult() { return pNode_; }
    bool& verbose() { return verbose_; }
  private:
    Node* pNode_;
    std::string value_;
    bool stop;
    bool verbose_;
  };
  //----< find node with specified value using Operation >-------------------

  template <typename Node>
  Node* MTree<Node>::finder(const std::string& value, Node* pNode=0)
  {
    finderOp<Node>* op = new finderOp<Node>(value);
    op->verbose() = verbose();
    Operation<Node>* oldOp = setOperation(op);
    walk();
    setOperation(oldOp);
    Node* pFoundNode = op->getResult();
    delete op;
    return pFoundNode;
  }
  //----< find node with specified value >-----------------------------------

  template <typename Node>
  Node* MTree<Node>::find(const std::string& tvalue, Node* pNode)
  {
    if(pNode == 0)
    {
      pNode = pRoot;
      pFound = 0;
    }
    else if(pFound)
      return pFound;
    if(verbose())
      std::cout << "\n  -- value = " << pNode->value();
    if(tvalue == pNode->value())
      return pFound = pNode;
    pNode->clearMarks();
    MNode* pTemp;
    while(pTemp = pNode->nextUnmarkedChild())
    {
      pFound = find(tvalue, pTemp);
    }
    return pFound;
  }
  //----< is the tree empty? >-----------------------------------------------

  template <typename Node>
  bool MTree<Node>::isEmpty()
  {
    return pRoot == 0;
  }


  template <typename Node>
  size_t MTree<Node>::getSize() {
	 // node_count = 0;
	  walk(pRoot); 
	  return node_count;
  }

  template <typename Node>
  size_t MTree<Node>::getSize(Node* node) {
	  // node_count = 0;
	  walk(node);
	  return node_count;
  }

  //Writes XML representation of the tree
  template <typename Node>
  void MTree<Node>::writeXML(XmlWriter& wtr) {	  
	  wtr.indent();
	  wtr.addDeclaration();
	  wtr.start("function");
	  wtr.addAttribute("name", pRoot->value().getName());
	  walk(pRoot, 0, &wtr);
	  wtr.end();		
  }
}
#endif
