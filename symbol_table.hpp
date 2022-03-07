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
public:
	SemSymbol(string k, string t) {
		kind = k;
		type = t;
	}
	string GetKind() {
		return kind;
	}
	string GetType() {
		return type;
	}
	void Print();

private:
	std::string kind;
	std::string type;


	//TODO add the fields that 
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
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
		ScopeTable(bool shadow);
		~ScopeTable();
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.
		SemSymbol* Get(std::string k);
		void Add(std::string k, SemSymbol* sym);
		bool contains(std::string k);
		void Print();
		bool AllowsShadow() { return allowShadow; }
	private:
		HashMap<std::string, SemSymbol *> * symbols;
		bool allowShadow;
};

class SymbolTable{
	public:
		SymbolTable();
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc. 
		void AddSymbol(std::string id, SemSymbol* sym);
		void AddScope(bool allowShadow = false);
		void DropScope();
		bool contains(std::string id);
		SemSymbol* Find(std::string id);
		void Print();
	private:
		std::list<ScopeTable *> * scopeTableChain;
};

	
}

#endif
