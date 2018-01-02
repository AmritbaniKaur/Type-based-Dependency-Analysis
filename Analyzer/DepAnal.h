#pragma once
//////////////////////////////////////////////////////////////////////////////
// DepAnal.h		- developing dependency analyzer						//
//																			//
//	ver 1.2																	//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition	//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home					//
//	Application:	Used to support parsing source code						//
//					CSE 687 Project #2, Spring 2017							//
//	Author:			Amritbani Sondhi,										//
//					Graduate Student, Syracuse University					//
//					asondhi@syr.edu											//
//	Source:			Jim Fawcett, CST 4-187, Syracuse University				//
//					(315) 443-3948, jfawcett@twcny.rr.com					//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ==================
*
* Public Methods:
* ==============
*	TypeAnal Class:	developed dependency analyzer
*		doTypeAnal()	- scans Abstract Syntax Tree, saves type analysis
*						  info to the TypeTable
*
* Build Process:
* ==============
*	- Required Files:
*		ActionsAndRules.h, TypeTable.h, Parser.h, GrammarHelpers.h,
* 		itokcollection.h, ScopeStack.h, Tokenizer.h, SemiExp.h, 
*		AbstrSynTree.h, Logger.h, FileSystem.h, ActionsAndRules.cpp,
*		Parser.cpp, ScopeStack.cpp, Tokenizer.cpp, SemiExp.cpp, 
*		AbstrSynTree.cpp
*	- Build commands:
*		devenv CodeAnalyzerEx.sln
*
* Maintenance History:
* ===================
* ver 1.2 : 15 Mar 2017
* - added dependency analysis information to the dbMain and print
* ver 1.1 : 10 Mar 2017
* - added a call to add data to the TypeTable
* ver 1.0 : 26 Feb 2017
* - first release
*/

#include "../Parser/ActionsAndRules.h"
#include "../TypeTable/TypeTable.h"
#include <iostream>
#include <functional>

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

using namespace TypeTableNamespace;

namespace CodeAnalysis
{
  ////////////////////////////////////////////////////////////////////
  // TypeTableData class creates a table with all the data attributes 
  //

  class TypeAnal
  {
  public:
    using SPtr = std::shared_ptr<ASTNode*>;

	DbMain dbMain;

    TypeAnal();
	DbMain doTypeAnal();
  private:
    void DFS(ASTNode* pNode);
    AbstrSynTree& ASTref_;
    ScopeStack<ASTNode*> scopeStack_;
    Scanner::Toker& toker_;
	TypeTable<StringData> typeTableInst;
  };

  inline TypeAnal::TypeAnal() : 
    ASTref_(Repository::getInstance()->AST()),
    scopeStack_(Repository::getInstance()->scopeStack()),
    toker_(*(Repository::getInstance()->Toker()))
  {
    std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
  }                                              // It doesn't do anything useful for dep anal.

  inline bool doDisplay(ASTNode* pNode)
  {
    static std::string toDisplay[] = {
      "function", "lambda", "class", "struct", "enum", "alias", "typedef"
    };
    for (std::string type : toDisplay)
    {
      if (pNode->type_ == type)
        return true;
    }
    return false;
  }
  inline void TypeAnal::DFS(ASTNode* pNode)
  {
	// check if it satisfies the node rule, if it does..
	// push the parent node to stack / create a new node for it
    
	static std::string path = "";
    if (pNode->path_ != path)
    {
      std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
      path = pNode->path_;
    }

	//Add the type analysis information to the typeTable
	TypeTable<StringData> typeTable = typeTableInst.addData(pNode->name_, pNode->package_, pNode->type_);

    if (doDisplay(pNode))
    {
		std::cout << "\n -------------------------";
		std::cout << "\n name: " << pNode->name_;
		std::cout << ", type: " << pNode->type_;
		std::cout << "\n -------------------------";
		std::cout << "\n ";

		//populating dbMain
		StringMap strMap;
		strMap["name"] = pNode->name_;
		strMap["type"] = pNode->type_;
		strMap["file"] = pNode->path_;

		std::vector<StringMap> data = dbMain[pNode->name_];
		data.push_back(strMap);
		dbMain[pNode->name_] = data;
	}
	for (auto pChild : pNode->children_)
	{
		DFS(pChild);
		// pop the node from stack / new node previously created
	}
  }

  inline DbMain TypeAnal::doTypeAnal()
  {
    std::cout << "\n  starting type analysis:\n";
	std::cout << "\n  scanning AST and saving important things in the Type Table:";
	std::cout << "\n -------------------------------------------------------";
    ASTNode* pRoot = ASTref_.root();
    DFS(pRoot);

	//Diplaying the data from dbMain
	for (auto x : dbMain)
	{
		std::cout << "--------------------------------------\n";
		std::cout << x.first << std::endl;
		std::cout << "-------------------------------------- \n";

		for (int i = 0; i < x.second.size(); i++)
		{
			for (auto y : x.second[i])
			{
				std::cout << y.first << " :  \t  " << y.second << std::endl;
			}
		}
		std::cout << "-------------------------------------- \n\n";
	}
	return dbMain;
  }
}