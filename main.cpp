#include "dirent.h"
#include <cstring>
#include "includes.hh"
#include "symtab.hh"
#include "visitor.hh"
#include "ast.hh"
#include "ast_utils.hh"
#include "utils.hh"
#include "typechecker.hh"

extern int yyparse();
extern FILE *yyin;

using namespace ASTUtils;

static ASTNode* root = NULL;
static ASTTypesVecNode* typesVec_node = NULL;
static SymbolTable symtab;


int main(int argc, char** argv){
	cout << "Running minijavap compiler. \n"; cout.flush();
	ASTUtils::init_ast_utils(symtab);
   
    	++argv, --argc;
    	yyin = (argc > 0) ? fopen(argv[0], "r") : stdin;
    	int yyres = yyparse();
    	if(yyres || not ASTUtils::get_varsok())
		err_exit(cout, "Terminating due to parsing errors.");    
    	else{
		cout << "Lexical analysis: passed." << endl;
		cout << "Syntax analysis: passed." << endl;
    	}
    	typesVec_node = ASTUtils::get_typesVec_node();
    	root = ASTUtils::get_root_node();
    	if(root == NULL)
		err_exit(cout, "AST not build.");
    
    	TypeChecker typecheck;
    	typesVec_node->accept(typecheck);
    	root->accept(typecheck);
	if(not typecheck.typeok){
		err_exit(cout, "Type checking: failed. Terminating ...");
		exit(1);
    	}else
		cout << "Type checking: passed." << endl;
    
    	return 0;
}
