#pragma once
//////////////////////////////////////////////////////////////////////////////
//	TypeTable.h		- helps create a table with the data from type analysis	//
//																			//
//	ver 1.0																	//
//	Language:		Visual C++ 2011, Visual Studio 2015 Community Edition	//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home					//
//	Application:	CSE 687 Project #2, Spring 2017							//
//	Author:			Amritbani Sondhi,										//
//					Graduate Student, Syracuse University					//
//					asondhi@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ==================
*	The TypeTable has to call the Tokenizer, then the Analyzer is run on the whole File Set.
*	As it traverses down the tree, and finds tokens, it puts them in the TypeTable.
*   The analysis part is done in DepAnal.h which calls this package when the values have to 
*   be saved in a Type Table. Global functions are not added in our TypeTable. 
*   Member functions are also not added to the TypeTable. 
*   The Actions and Rules, now, catches alias, typedef and enums too. 
*
* Public Methods:
* ==============
*	TypeTableData Class:	represents a data record in our TypeTable database through Properties
*							Defines all the properties for getting and setting the values
*	- show()		: Displaying all and Data of the Table
*	- saveChild()	: Saves a Child Vector into the Key
*
*	TypeTable Class	: is a key/value pair in-memory database, stores and retrieves the elements
*	- keys()		: Pushing Data into the Vector
*	- save()		: Saving the data into the database
*	- value()		: Getting the Element's Data from a Key
* 	- count()		: Getting the size of the Unordered Map
*	- addData()		: Adding Data to the Table
* 
* Build Process:
* ==============
*	- Required Files:
*		CppProperties.h, ActionsAndRules.h, Parser.h, GrammarHelpers.h,
*		itokcollection.h, ScopeStack.h, Tokenizer.h, SemiExp.h, 
*		AbstrSynTree.h, Logger.h, FileSystem.h, ActionsAndRules.cpp, 
*		Parser.cpp, ScopeStack.cpp, Tokenizer.cpp, SemiExp.cpp,
*		AbstrSynTree.cpp
*	- Build commands:
*		devenv CodeAnalyzerEx.sln
*
* Maintenance History:
* ===================
* ver 1.0 : 09 Mar 2017
* - first release
*/

#include "../CppProperties/CppProperties.h"
#include "../Parser/ActionsAndRules.h"
#include <unordered_map>
#include <vector>

using StringData = std::string;
using Name = std::string;
using FileName = std::string;
using Type = std::string;
using ChildVector = std::vector<std::string>;
using StringMap = std::unordered_map<StringData, StringData>;
using DbMain = std::unordered_map<StringData, std::vector<StringMap>>;

namespace TypeTableNamespace
{
	using Key = StringData;
	//using Keys = StringData;

	////////////////////////////////////////////////////////////////////
	// TypeTableData class creates a table with all the data attributes 
	//
	template<typename StringData>
	class TypeTableData
	{
	public:
		Property<Name> name;					// metadata
		Property<FileName> fileName;			// metadata
		Property<Type> type;					// metadata
		//Property<ChildVector> childVector;	// metadata
		ChildVector childVector;				// metadata

		StringData show();
		void saveChild(Key key);
	};

	// Displaying all the Data in TypeTableData
	template<typename StringData>
	StringData TypeTableData<StringData>::show()
	{
		std::ostringstream out;
		out.setf(std::ios::adjustfield, std::ios::left);
		out << "\n    " << std::setw(8) << "Name" << " : " << name;
		out << "\n    " << std::setw(8) << "File Name" << " : " << fileName;
		out << "\n    " << std::setw(8) << "Type" << " : " << type;

		// Loop for displaying the Child Vectors of the element
		for (int i = 0; i < childVector.size(); i++)
		{
			out << "\n    " << std::setw(8) << "Child Vector" << " : " << childVector[i];
		}

		out << "\n";
		return out.str();
	}

	// Saves a Child Vector into the Key
	template<typename StringData>
	void TypeTableData<StringData>::saveChild(Key key)
	{
		childVector.getValue().push_back(key);
	}

	///////////////////////////////////////////////////////////////////
	// TypeTable class creates a table of all the important components 
	//
	template<typename StringData>
	class TypeTable
	{
	public:
		//using Key = StringData;
		using Keys = std::vector<Key>;

		bool save(Key key, TypeTableData<StringData> tbData);
		TypeTableData<StringData> value(Key key);

		Keys keys();
		size_t count();

		TypeTable<StringData> addData(StringData name, StringData fileName, StringData type);
	private:
		using Item = std::pair<Key, TypeTableData<StringData>>;
		// To store the key and values of the TypeTable in an unordered map
		std::unordered_map<Key, TypeTableData<StringData>> table;
	};

	// Save the element data into the database table
	template<typename StringData>
	bool TypeTable<StringData>::save(Key key, TypeTableData<StringData> tbData)
	{
		if (table.find(key) != table.end())			// If element is present
			return false;
		table[key] = tbData;						// If not; Save
		return true;
	}

	// Get the Element's Data from a Key
	template<typename StringData>
	TypeTableData<StringData> TypeTable<StringData>::value(Key key)
	{
		if (table.find(key) != table.end())
			return table[key];
		return TypeTableData<StringData>();
	}

	// Pushing Data into the Vector
	template<typename StringData>
	typename TypeTable<StringData>::Keys TypeTable<StringData>::keys()
	{
		Keys keys;
		for (Item item : table)
		{
			keys.push_back(item.first);
		}
		return keys;
	}

	// Get the size of the Unordered Map
	template<typename StringData>
	size_t TypeTable<StringData>::count()
	{
		return table.size();
	}

	// Adding Data to the Table
	template<typename StringData>
	TypeTable<StringData> TypeTable<StringData>::addData(StringData name, StringData fileName, StringData type)
	{
		TypeTable<StringData> tableObj;

		TypeTableData<StringData> data1;	//Create Data Element of the database
		data1.name = name;					//key
		data1.fileName = fileName;
		data1.type = type;

		tableObj.save(data1.name, data1);	//store the data in the TypeTable

		// Printing the Database Table of all the Elements
		std::cout << "\n -------------------------------------------------------\n";
		std::cout << data1.show();
		std::cout << " -------------------------------------------------------\n";

		return TypeTable<StringData>();
	}

}