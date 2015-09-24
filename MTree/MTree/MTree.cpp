//////////////////////////////////////////////////////////////////////////
// MTree.cpp  -  Holds information extracted from Xml string or file   //
// ver 2.1                                                               //
// Language:    Visual C++, 2008                                         //
// Platform:    Dell Precision T7400, Vista Ultimate, SP1                //
// Application: CSE687 - Demo for Project #1, Spring 2010                //
// Author:		Pallavi 
// Source:      Jim Fawcett, Syracuse University, CST 4-187,             //
//              (315) 443-3948, jfawcett@twcny.rr.com                    //
///////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#ifdef TEST_MTREE

#include <string>
#include "MTree.h"
#include "MNode.h"
#include "Treenode.h"
using namespace TMTree;

void main(){
  std::cout << "\n  MTree class demonstration";
  std::cout << "\n =============================\n";
  typedef MNode<Treenode> Node;element elem;
  elem.type = "unknown";elem.name = "root";elem.lineCount = 0;
  elem.endLine = 0;elem.complexity = 0;
  Treenode root(elem);
  Node* pRoot = new Node( root);elem.name = "Child1";
  Treenode child1(elem);
  Node* pChild1 = new Node(child1);elem.name = "child2";
  Treenode child2(elem);
  Node* pChild2 = new Node(child2);elem.name = "Grandchild11";
  Treenode grandchild11(elem);
  Node* pGrandchild11 = new Node(grandchild11);
  pChild1->addChild(pGrandchild11);  pRoot->addChild(pChild1);pRoot->addChild(pChild2);
  class newOper : public Operation<Node>{
  public:
    bool operator()(Node* pNode){
      std::cout << "\n -- " << pNode->ToString();
      return false;
    }
  };
  MTree<Node> tree(new newOper);tree.makeRoot(pRoot);tree.verbose() = true;tree.walk(pRoot);
  std::cout << std::endl;std::string val = "Grandchild11";
  Node* pFound = tree.finder(val);
  if(pFound)
    std::cout << "\n  found \"" << pFound->value() << "\"";
  else
    std::cout << "\n  couldn't find \"" << val << "\"";
  val = "Child1";pFound = tree.finder(val);
  if(pFound)
    std::cout << "\n  found \"" << pFound->value() << "\"";
  else
    std::cout << "\n  couldn't find \"" << val << "\"";
  val = "FooBar";
  pFound = tree.finder(val);
  if(pFound)
    std::cout << "\n  found \"" << pFound->value() << "\"";
  else
    std::cout << "\n  couldn't find \"" << val << "\"";
  std::cout << "\n\n  removing first child";
  pRoot->removeChild(pChild1);
  tree.walk();
  int node_count = tree.getSize();
  std::cout << "\n\n  Node count "<< node_count;
  XmlWriter wtr;
  tree.writeXML(wtr);
  std::cout <<"\n"<< wtr.xml();
}
#endif

