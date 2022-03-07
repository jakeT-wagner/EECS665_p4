#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include <iostream>

namespace cminusminus{

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	if (myType->isVoid()) {
		std::cerr << "FATAL " << myType -> pos() -> begin() << ": Invalid type in declaration\n";
		nameAnalysisOk = false;
	}
	if (symTab -> contains(myID -> getName())) {
		std::cerr << "FATAL " << myID -> pos() -> begin() << ": Multiply declared identifier\n";
		nameAnalysisOk = false;
	}
	if (nameAnalysisOk) {
		SemSymbol* sym = new SemSymbol("var", myType -> getType());
		symTab -> AddSymbol(myID -> getName(), sym);
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){	
	bool nameAnalysisOk = true;
	if (symTab -> contains(myID -> getName())) {
		std::cerr << "FATAL " << myID -> pos() -> begin() << ": Multiply declared identifier\n";
		nameAnalysisOk = false;
	}

	std::string type = "";

	unsigned int i = 0;
	for (auto formal : *myFormals) {
		type += formal -> getTypeNode() -> getType();
		i++;
		if (i < myFormals -> size()) type += ", ";
	}
	
	type += "->" + myRetType -> getType();
	if (nameAnalysisOk) {
		SemSymbol* sym = new SemSymbol("fn", type);
		symTab -> AddSymbol(myID -> getName(), sym);
	}

	symTab -> AddScope();

	for (auto formal : *myFormals) {
		nameAnalysisOk = formal -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab -> DropScope();
	return nameAnalysisOk;
}

bool WhileStmtNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myCond -> nameAnalysis(symTab);
	symTab -> AddScope(true);
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab -> DropScope();
	return nameAnalysisOk;
}

bool IfStmtNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = true; 
	nameAnalysisOk = myCond -> nameAnalysis(symTab);
	
	symTab -> AddScope(true);
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab -> DropScope();
	return nameAnalysisOk;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = true; 
	nameAnalysisOk = myCond -> nameAnalysis(symTab);
	symTab -> AddScope(true);
	for (auto stmt : *myBodyTrue) {
		nameAnalysisOk = stmt -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab -> DropScope();
	symTab -> AddScope(true);
	for (auto stmt : *myBodyFalse) {
		nameAnalysisOk = stmt -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab -> DropScope();
	return nameAnalysisOk;
}

bool ReadStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myDst->nameAnalysis(symTab);
}

bool WriteStmtNode::nameAnalysis(SymbolTable * symTab) {
	return mySrc->nameAnalysis(symTab);
}

bool PostDecStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myLVal->nameAnalysis(symTab);
}

bool PostIncStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myLVal->nameAnalysis(symTab);
}

bool ReturnStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

bool CallStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myCallExp->nameAnalysis(symTab);
}

bool CallExpNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myID -> nameAnalysis(symTab);
	for (auto arg : *myArgs) {
		nameAnalysisOk = arg -> nameAnalysis(symTab) && nameAnalysisOk;
	}
	return nameAnalysisOk;
}

bool AssignStmtNode::nameAnalysis(SymbolTable* symTab) {
	return myExp -> nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myDst -> nameAnalysis(symTab);
	nameAnalysisOk = mySrc -> nameAnalysis(symTab) && nameAnalysisOk;
	return nameAnalysisOk;
}

bool DerefNode::nameAnalysis(SymbolTable* symTab) {
	return myID -> nameAnalysis(symTab);
}

bool RefNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myID->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool IDNode::nameAnalysis(SymbolTable* symTab) {
	SemSymbol* sym = symTab -> Find(name);
	if (sym == nullptr) {
		std::cerr << "FATAL " << myPos -> begin() << ": Undeclared identifier" << "\n";
		return false;
	}
	attachSymbol(sym);
	return true;
}

bool ExpNode::nameAnalysis(SymbolTable* symTab) {
	return true;
}

bool BinaryExpNode::nameAnalysis(SymbolTable* symTab) {
	//left operand
	bool nameAnalysisOk = myExp1->nameAnalysis(symTab);
	//right operand
	nameAnalysisOk = myExp2->nameAnalysis(symTab) && nameAnalysisOk;
	return nameAnalysisOk;
}

bool UnaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

bool LValNode::nameAnalysis(SymbolTable* symTab) {
	std::cout <<"Lval called somehow\n";
	return false; //what is a Lval?? 

}

bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to IntTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	return true;
}

void IDNode::attachSymbol(SemSymbol* symbolIn) {
	mySymbol = symbolIn;
}


}
