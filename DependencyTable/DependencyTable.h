#pragma once
//////////////////////////////////////////////////////////////////////////////////
//	DependencyTable.h -	creates a table with the data from dependency analysis	//
//																				//
//	ver 1.0																		//
//	Language:		Visual C++ 2011, Visual Studio 2015 Community Edition		//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home						//
//	Application:	CSE 687 Project #2, Spring 2017								//
//	Author:			Amritbani Sondhi,											//
//					Graduate Student, Syracuse University						//
//					asondhi@syr.edu												//
//////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ==================
*	The Dependency Table package is used to create a dependency table with the data from
* the dependency analysis ie. it works with DepAnal.h present in CodeAnalyzer package.
* The tokens are compared with the data present in the TypeTable. If a dependency is seen
* the dependent filename is added as a childVector in the dependency table. Also, if a row 
* is already present for the token, another childVector is added to it (it is not replaced)
*
* Public Methods:
* ==============
*	DependencyTable Class : 
*	- performDependencyAnalysis() : 
*
* Build Process:
* ==============
*	- Required Files:
*		Tokenizer.h, TypeTable.h, DepAnal.h, CppProperties.h, ActionsAndRules.h,
*		Parser.h, GrammarHelpers.h, itokcollection.h, ScopeStack.h, Tokenizer.h, 
*		SemiExp.h, AbstrSynTree.h, Logger.h, FileSystem.h, ActionsAndRules.cpp, 
*		ScopeStack.cpp, Tokenizer.cpp, SemiExp.cpp, AbstrSynTree.cpp, Parser.cpp
*	- Build commands:
*		devenv CodeAnalyzerEx.sln
*
* Maintenance History:
* ===================
* ver 1.0 : 09 Mar 2017
* - first release
*/

#include "../Tokenizer/Tokenizer.h"
#include "../TypeTable/TypeTable.h"
#include "../Analyzer/DepAnal.h"

#include <string>
#include <vector>

using namespace Scanner;
using namespace TypeTableNamespace;
using namespace CodeAnalysis;

using StringData = std::string;
using FileName = std::string;
using Key = std::string;
using Keys = std::vector<StringData>;
using DepFiles = std::vector<StringData>;

namespace DependencyTableNamespace
{
	////////////////////////////////////////////////////////////////////////////////////////
	// DependencyTable class checks for any dependencies between the files and addes if it 
	// is not already present in the table. No duplicates are added in the table
	//
	class DependencyTable
	{
	public:
		TypeTable<StringData> performDependencyAnalysis(DbMain dbMain, TypeTable<StringData> dbTable, DepFiles depFiles);

	private:
		FileName fileName;
		TypeTableData<StringData> depTableInst;
		TypeAnal typeAnal;

		// Create a tokenizer instance
		Toker tokerInst;
	};

	TypeTable<StringData> DependencyTable::performDependencyAnalysis(DbMain dbMain, TypeTable<StringData> dbTable, DepFiles depFiles)
	{	
		// Search for file path and do a dependency check

		Keys keys;
		StringMap tempMap;

		for (auto i : dbMain)
		{
			keys.push_back(i.first);
			for (auto x : i.second)
			{
				for (auto y : x)
				{
					tempMap[y.first] = y.second;
				}
			}
		}

		TypeTableData<StringData> depTableData;
		for (FileName fileSpec : depFiles)
		{
			try
			{
				StringData file = FileSystem::Path::getName(fileSpec); // gets the file from path

				std::ifstream in(fileSpec);
				if (!in.good())				// check if file is good, i.e. can be opened
				{
					std::cout << "\n  cannot open file: " << fileSpec << "\n\n";
				}
				else
				{
					tokerInst.returnComments(false);			// remove comments
					tokerInst.attach(&in);
					do
					{
						StringData tok = tokerInst.getTok();	// collect the tokens

						for (StringData key : keys)
						{
							if (key == tok)						// check for token match
							{
								std::vector<StringMap> mapVector = dbMain[key];

								for (StringMap i : mapVector)	// loop for all the files in the vector
								{
									for (auto g : i)			
									{
										if (g.first == "file")	// check if the type is 'file'
										{
											depTableData.name = FileSystem::Path::getFullFileSpec(fileSpec);
											depTableData.childVector.push_back(g.second);		
											std::sort(depTableData.childVector.begin(), depTableData.childVector.end());
											depTableData.childVector.erase(std::unique(depTableData.childVector.begin(), depTableData.childVector.end()), depTableData.childVector.end());
											dbTable.save(depTableData.name, depTableData);		// save as a Vector
											std::cout << depTableData.show();					// print the added Dependency in the Table
										}
									}
								}
							}
						}
					} while (in.good());
				}
			}
			catch (std::logic_error& ex) // Exception Handling
			{
				std::cout << "\n  " << ex.what();
			}
		}
		return dbTable;
	}
}