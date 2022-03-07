#include "symbol_table.hpp"
#include <iostream>
namespace cminusminus{

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
	allowShadow = false;
}

ScopeTable::ScopeTable(bool shadow) {
	symbols = new HashMap<std::string, SemSymbol *>();
	allowShadow = shadow;
}

SemSymbol* ScopeTable::Get(std::string k) {
	return (*symbols)[k];
}

void SemSymbol::Print() {
	std::cout << kind << ", " << type;
}

bool ScopeTable::contains(std::string k) {
	return symbols -> find(k) != symbols -> end();
}

void ScopeTable::Add(std::string id, SemSymbol* sym) {
	symbols -> insert( { id, sym } );
}

void ScopeTable::Print() {
	for (auto sym : *symbols) {
		std::cout << sym.first << " : ";
		sym.second -> Print();
		std::cout << "\n";
	}
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.
	scopeTableChain = new std::list<ScopeTable *>();
	AddScope();
}

void SymbolTable::AddScope(bool allowShadow) {
	//std::cout << "Adding Scope\n";
	this -> scopeTableChain -> push_front( new ScopeTable(allowShadow) );
	//Print();
}

void SymbolTable::AddSymbol(std::string id, SemSymbol* sym) {
	scopeTableChain -> front() -> Add(id, sym);
}

void SymbolTable::DropScope() {
	//std::cout << "Dropping Scope\n";
	//Print();
	this -> scopeTableChain -> pop_front();
	//std::cout << "Dropped\n";
	//Print();
}

bool SymbolTable::contains(std::string id) {
	ScopeTable* head = scopeTableChain -> front();
	if (head -> AllowsShadow()) {
		if (head -> contains(id)) { return true; }
		else { return false; }
	}
	for (auto scope : *scopeTableChain) {
		if (scope -> contains(id)) return true;
	}
	return false; 
}

SemSymbol* SymbolTable::Find(std::string id) {
	for (auto scope : *scopeTableChain) {
		if (scope -> contains(id)) return scope -> Get(id);
	}
	return nullptr; 
}

void SymbolTable::Print() {
	for (auto scope : *scopeTableChain) {
		std::cout << "-- Scope -- \n";
		scope -> Print();
		std::cout << '\n';
	}
}

}
