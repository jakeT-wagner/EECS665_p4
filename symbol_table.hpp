#ifndef CMINUSMINUS_SYMBOL_TABLE_HPP
#define CMINUSMINUS_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <list>

//Use an alias template so that we can use
// "HashMap" and it means "std::unordered_map"
template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using namespace std;

namespace cminusminus{

//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols 
// exist for the lifetime of a scope in the 
// symbol table. 
class SemSymbol {
	//TODO add the fields that 
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
	//add for functions
	public:
		SemSymbol(int kind, std::string type, std::string name) 
		: myKind(kind), myType(type), myName(name)  {
			if (myType == "void") {
				valid = false;
			}
			else {
				valid = true;
			}
		}
		void setKind(int k);
		void setType(std::string t);
		void setName(std::string n);
		int getKind();
		std::string getType();
		std::string getName();
		bool isValid() {return valid;}
		void printContents();
	private:
		int myKind; //0 for variable, 1 for function
		std::string myType; //void, int, bool....
		std::string myName; 
		bool valid;
};

//A single scope. The symbol table is broken down into a 
// chain of scope tables, and each scope table holds 
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
		void addSemSymbol(std::string symbName, SemSymbol * symbol);
		SemSymbol * findSymbol(std::string symbName);
		void printContents();
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.
	private:
		HashMap<std::string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		void addScopeTable();
		void removeScopeTable();
		SemSymbol* findSymbol(std::string symbName);
		bool isInCurrentScope(std::string symbName) {
			SemSymbol* temp = scopeTableChain->front()->findSymbol(symbName);
			if (temp != nullptr) return true;
			return false;
		}
		void insertSymbol(std::string symbName, SemSymbol* symbol);
		void printContents();
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc. 
	private:
		std::list<ScopeTable *> * scopeTableChain;
};

	
}

#endif
