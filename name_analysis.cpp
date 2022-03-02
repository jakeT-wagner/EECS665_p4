#include "ast.hpp"
#include "errors.hpp"

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

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	SemSymbol * temp = new SemSymbol(0, getTypeNode()->getType(), ID()->getName());
	if (temp->isValid()) {
		//make a function combining a search in the most recent scope to see if there's 
		//already a declaration 
		symTab->insertSymbol(ID()->getName(), temp);
	}
	else {
		std::cerr << myPos->span() << ": Invalid type in declaration\n";
		nameAnalysisOk = false;
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	std::list<FormalDeclNode *> * formals = getFormals();
	//create type of function
	std::string funct_type = "";
	bool firstFormal = true;
	for (auto formal : *formals) {
		if (firstFormal) { firstFormal = false; }
		else { funct_type += ", "; }
		funct_type += formal->getTypeNode()->getType();
	}
	funct_type += "->";
	funct_type += getTypeNode()->getType();
	SemSymbol * temp = new SemSymbol(1, funct_type, ID()->getName());
	//make a function combining a search in the most recent scope to see if there's 
	//already a declaration 
	symTab->insertSymbol(ID()->getName(), temp);
	symTab->addScopeTable();

	//formals
	for (auto formal : *formals) {
		nameAnalysisOk = formal->nameAnalysis(symTab);
	}
	//body
	std::list<StmtNode *> * body = getBody();
	for (auto stmt : *body) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); //test the program is working properly
	symTab->removeScopeTable(); //remove function from scope
	return nameAnalysisOk;
}

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	symTab->addScopeTable();
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); //test right before delete
	symTab->removeScopeTable();
	return nameAnalysisOk;
}

bool IfStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	symTab->addScopeTable();
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); //test right before delete
	symTab->removeScopeTable();
	return nameAnalysisOk;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable * symTab) {
	//different scope blocks for if else>
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab);
	//if block 
	symTab->addScopeTable();
	for (auto stmt : *myBodyTrue) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); //test right before delete
	symTab->removeScopeTable();

	//else block
	symTab->addScopeTable();
	for (auto stmt : *myBodyFalse) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); //test right before delete
	symTab->removeScopeTable();
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

bool CallExpNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myID->nameAnalysis(symTab);
	for (auto arg : *myArgs) {
		nameAnalysisOk = arg->nameAnalysis(symTab);
	}
	return nameAnalysisOk;
}

bool AssignStmtNode::nameAnalysis(SymbolTable * symTab) {
	return getExp()->nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable * symTab) {
	//attach symbol to assignee
	bool nameAnalysisOk = myDst->nameAnalysis(symTab);

	//attach symbols to assign expression
	nameAnalysisOk = mySrc->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool LValNode::nameAnalysis(SymbolTable* symTab) {
	std::cout <<"Lval called somehow\n";
	return false; //what is a Lval?? 

}

bool DerefNode::nameAnalysis(SymbolTable* symTab) {
    bool nameAnalysisOk = myID->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool RefNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myID->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool IDNode::nameAnalysis(SymbolTable* symTab) {
	SemSymbol * symbol = symTab->findSymbol(name);
	if (symbol ==nullptr) {
		std::cerr << myPos->span() << "Undeclared identifier\n";
		return false;
	}
	attachSymbol(symbol);
	return true;
}

bool ExpNode::nameAnalysis(SymbolTable* symTab) {
	std::cout <<"How did it get here";
	return false;
}

bool BinaryExpNode::nameAnalysis(SymbolTable* symTab) {
	//left operand
	bool nameAnalysisOk = myExp1->nameAnalysis(symTab);
	//right operand
	nameAnalysisOk = myExp2->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool UnaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

}
