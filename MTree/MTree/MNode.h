#ifndef MNODE_H
#define MNODE_H
///////////////////////////////////////////////////////////////////////////
// MNode.h   -  M-ary node class to use in M-ary tree                    //
// ver 1.0                                                               //
// Language:    Visual C++, 2008                                         //
// Platform:    Dell Precision T7400, Win7 Professional                  //
// Application: CSE687 - Demo for Project #1, Spring 2010                //
// Author:      Jim Fawcett, Syracuse University, CST 4-187,             //
//              (315) 443-3948, jfawcett@twcny.rr.com                    //
///////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * Provides a template-based M-ary Node class that holds a finite number
 * of child nodes.  
 * 
 * Required Files:
 * ===============
 * MNode.h, MNode.cpp
 *
 * Build Process:
 * ==============
 * cl /D:TEST_MNODE MNode.cpp
 * 
 * Maintenance History:
 * ====================
 * ver 2.1: Mar 21 14
 * - Added functionality to compare two nodes for scope structure simialrity
 * ver 2.0: Feb 11 2014
 * - Added functionality to get and set parent of a node
 * ver 1.0 : 27 Feb 10
 * - first release
 */


#include <iostream>
#include <vector>
#include "Treenode.h"
namespace TMTree
{
  /////////////////////////////////////////////////////////////////////////
  // MNode<T> class

  template <typename T>
  class MNode
  {
  public:
    MNode(const T& val);
    MNode(const MNode<T>& node);
    ~MNode();
    MNode<T>* clone();
    MNode<T>& operator=(const MNode<T>& node);
    T& value();
    void addChild(MNode<T>* pNode);
    bool removeChild(MNode<T>* pChild);
    size_t numChildren();
    MNode<T>* nextUnmarkedChild();
    void clearMarks();
	std::string ToString(); 
	
	//Get Parent node of this node
	MNode<T>* getParent();
	//Set Parent node of this node
	void setParent(MNode<T>* parent);
	bool compareNodes(MNode<T>* node2);
	
  private:;
    T val_;
    std::vector<MNode<T>*> children;
    size_t visitIndex;
	MNode<T>* _parent;
  };


  //----< return text >----------------------------------------------------

  template <typename T>
  T& MNode<T>::value() { return val_; }

  //----< promotion constructor >------------------------------------------

  template <typename T>
  MNode<T>::MNode(const T& t) : val_(t), visitIndex(0) {}

  //----< destructor >-------------------------------------------------------

  template <typename T>
  MNode<T>::~MNode()
  {
    //std::cout << "\n  deleting MNode";
    for(size_t i=0; i<children.size(); ++i)
    {
      delete children[i];
    }
  }
  //----< return copy of self without children >---------------------------

  template <typename T>
  MNode<T>* MNode<T>::clone()
  {
    MNode<T>* pNode = new MNode<T>(value());
    for(size_t i=0; i<children.size(); ++i)
      pNode->addChild(children[i]->clone());
    return pNode;
  }
  //----< copy constructor >-----------------------------------------------

  template <typename T>
  MNode<T>::MNode(const MNode<T>& node) : val_(node.val_)
  {
    for(size_t i=0; i<node.children.size(); ++i)
      children.push_back(node.children[i]->clone());
  }
  //----< assignment operator >----------------------------------------------

  template <typename T>
  MNode<T>& MNode<T>::operator =(const MNode<T>& el)
  {
    if(&el == this) return *this;
    for(size_t i=0; i<children.size(); ++i)
      delete children[i];
    children.clear();
    for(size_t i=0; i<el.children.size(); ++i)
      addChild(el.children[i]->clone());
    return *this;
  }
  //----< add child node >---------------------------------------------------

  template <typename T>
  void MNode<T>::addChild(MNode<T>* pMNode)
  {
    if(pMNode == this)
      throw std::exception("attempting to make node a child of itself!");
    children.push_back(pMNode);
  }

  //----< return number of children >----------------------------------------

  template <typename T>
  size_t MNode<T>::numChildren() { return children.size(); }

  //----< remove child node - returns false on failure, else true >----------

  template <typename T>
  bool MNode<T>::removeChild(MNode<T>* pMNode)
  {
    std::vector< MNode<T>* >::iterator iter = children.begin();
    for(iter=children.begin(); iter!=children.end(); ++iter)
    {
      if(pMNode == *iter)
      {
        delete pMNode;
        children.erase(iter);
        return true;
      }
    }
    return false;
  }
  //----< return pointer to next unvisited child or null >-----------------

  template <typename T>
  MNode<T>* MNode<T>::nextUnmarkedChild()
  {
    if(children.size() == 0)
      return 0;
    if(visitIndex < children.size())
    {
      return children[visitIndex++];
    }
    return 0;
  }
  //----< "mark" all children as unvisited >-------------------------------

  template <typename T>
  void MNode<T>::clearMarks()
  {
    visitIndex = 0;
  }
  //----< serialize content to string representation of node >-------------

  template <typename T>
  std::string MNode<T>::ToString()
  {
    std::ostringstream temp;
    temp << "<" << value() << ">";
    return temp.str();
  }


 // ----< compares  tree nodes for similar scope structure >---------------------------------------------------
 template <>
 inline bool MNode<Treenode>::compareNodes(MNode<Treenode>* node2)
 {
	 MNode<Treenode>* node1 = this;
	 Treenode tn1 = node1->value(); Treenode tn2 = node2->value();
	 std::string type_node1 = (tn1).getType(), type_node2 = (tn2).getType();
	 int complexity_node1 = (tn1).getComplexity(), complexity_node2 = (tn2).getComplexity();
	 size_t size1 = tn1.getSize(), size2 = tn2.getSize();
	 int  line_count_diff = size2 - size1;

	 if (complexity_node1 == complexity_node2
		 && node1->numChildren() == node2->numChildren()
		 && type_node1.compare(type_node2) == 0)
	//	 && (abs(line_count_diff) <= 10))
	 {

		 node1->clearMarks(); node2->clearMarks();
		 MNode<Treenode>* node_i; MNode<Treenode>* node_j;

		 while ((node_i = node1->nextUnmarkedChild()) && (node_j = node2->nextUnmarkedChild()))
		 {
			 if (!node_i->compareNodes(node_j)){
				 return false;
			 }
		 }
	 }
	 else{
		 return false;
	 }
	 return true;
 }

 //Get Parent node of this node
 template <typename T>
 MNode<T>* MNode<T>::getParent(){
	 return _parent;
 }

 //Set Parent node of this node
 template <typename T>
 void MNode<T>::setParent(MNode<T>* parent){
	 _parent = parent;
 }

}
#endif
