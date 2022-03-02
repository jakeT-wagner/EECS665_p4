#include "symbol_table.hpp"
#include <iostream>
namespace cminusminus{

// ----SYMBOL TABLE BEGINS---- //
SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.
	scopeTableChain = new std::list<ScopeTable *>();
	addScopeTable(); //adds the global scope
}

void SymbolTable::addScopeTable() {
	scopeTableChain->push_front(new ScopeTable());
}

void SymbolTable::removeScopeTable() {
	scopeTableChain->pop_front();
}

void SymbolTable::insertSymbol(std::string symbName, SemSymbol* symbol) {
	scopeTableChain->front()->addSemSymbol(symbName, symbol);
}

SemSymbol* SymbolTable::findSymbol(std::string symbName) {
	std::string temp = symbName;
	for (auto scpTable : *scopeTableChain) {
		SemSymbol * temp = scpTable->findSymbol(symbName);
		if (temp != nullptr) return temp;
	}
	return nullptr;
}

void SymbolTable::printContents() {
	for (auto scpTable : *scopeTableChain) {
		std::cout << "--Scope Table--" << std::endl;
		scpTable->printContents();
		std::cout << "\n";
	}
}
// ----SYMBOL TABLE ENDS---- //




// ----SCOPE TABLE BEGINS---- //
ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}
void ScopeTable::addSemSymbol(std::string symbName, SemSymbol * symbol) {
	symbols->insert(std::make_pair(symbName, symbol)); //maybe?
	//symbols[key] = value; //doesn't seem to work as symbols is a pointer
}
SemSymbol* ScopeTable::findSymbol(std::string symbName) {
	//just lookup symbol
	try 
	{
		return symbols->at(symbName);
	}
	catch (std::out_of_range e) {
		return nullptr;
	}
}
void ScopeTable::printContents() {
	for (auto i : *symbols) {
		i.second->printContents();
	}
}
// ----SCOPE TABLE ENDS---- //




// ----SEMSYMBOL BEGINS---- //
void SemSymbol::printContents() {
	std::cout << "\tName: " << myName << std::endl;
	std::cout << "\tKind: ";
	if (myKind == 0) std::cout << "var\n";
	else std::cout << "fn\n";
	std::cout << "\tType: " << myType << "\n\n";
}
// --SemSymbol SETTERS-- //
void SemSymbol::setKind(int k) {
	myKind = k;
}
void SemSymbol::setType(std::string t) {
	myType = t;
}
void SemSymbol::setName(std::string n) {
	myName = n;
}
// --SemSymbol SETTERS END-- //

// --SemSymbol GETTERS-- //
int SemSymbol::getKind() {
	return myKind;
}
std::string SemSymbol::getType() {
	return myType;
}
std::string SemSymbol::getName() {
	return myName;
}
// --SemSymbol GETTERS END-- //
// ----SEMSYMBOL ENDS---- //
}
