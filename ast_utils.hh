//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        ast_utils.hh
 *
 * Description: 
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _AST_UTILS_H
#define _AST_UTILS_H 1

#include "symtab.hh"
#include "ast.hh"
namespace ASTUtils{
    void
    init_ast_utils(SymbolTable& st);

    bool
    get_varsok();

    ASTNode*
    get_root_node();
    
    ASTTypesVecNode*
    get_typesVec_node();

    void
    set_root_node(ASTNode* rn);
    
    void
    set_current_class(string* id);
    
    void
    unset_current_class();
    
    void
    set_current_method(string* id);
    
    void
    unset_current_method();
    
    void
    set_reading_params();
    
    void
    unset_reading_params();
    
    void
    set_assigning_method_params(ASTNode* n1, string* id);
    
    void
    unset_assigning_method_params();
    
    void
    push_scope();

    void
    pop_scope();

    void
    enter_while();

    void
    exit_while();
    
    ASTNode*
    mk_program_node(ASTNode* n1, ASTNode* n2);
    
    ASTNode*
    mk_main_node(string* id, ASTNode* n1);
    
    void
    mk_cref(string* s);
	
    void
    mk_cref(string* s, string* ext);
    
    ASTNode*
    mk_class_node(string* id, ASTNode* n1, ASTNode* n2);
    
    ASTNode*
    mk_class_node(string* id, ASTNode* n1, ASTNode* n2, string* ext);
    
    ASTNode*
    mk_classes_node(ASTNode* n1, ASTNode* n2);
 
    void
    mk_mref(string* s, char* type); 
    
    ASTNode*
    mk_method_node(char *type, string *id, ASTNode* n1, ASTNode* n2, ASTNode* n3);
    
    ASTNode*
    mk_methods_node(ASTNode* n1, ASTNode* n2);

    ASTNode*
    mk_stmts_node(ASTNode* n1, ASTNode* n2);

    ASTNode*
    mk_astmt_node(string* name, ASTNode* n1);
    
    ASTNode*
    mk_aivecstmt_node(string* name, ASTNode* n1, ASTNode* n2);   

    ASTNode*
    mk_istmt_node(ASTNode* n0, ASTNode* n1, ASTNode* n2);

    ASTNode*
    mk_wstmt_node(ASTNode* n1, ASTNode* n2);

    ASTNode*
    mk_pstmt_node(ASTNode* n1);

    ASTNode*
    mk_opexpr_node(string* op, ASTNode* n1, ASTNode* n2);

    ASTNode*
    mk_iconst_node(int ival);

    ASTNode*
    mk_bconst_node(bool bval);

    ASTNode*
    mk_varDecls_node(ASTNode* n1, ASTNode* n2);

    ASTNode*
    mk_vref_node(string* name, char* type);

    ASTNode*
    get_vref_node(string* name);
       	
    ASTNode*
    mk_newId(string* s);
		
    ASTNode*
    mk_int_vec_node(ASTNode* n1);
	
    ASTNode*
    mk_vec_value_node(ASTNode* n1, ASTNode* n2);
	
    ASTNode*
    mk_length_node(ASTNode* n1);
	
    ASTNode*
    mk_negation_node(ASTNode* n1);
	
    ASTNode*
    mk_call_method_node(ASTNode* n1, string* mid);
	
    ASTNode*
    get_current_class_node();
	
    void
    mk_param_value(ASTNode* n1);
	
    char*
    get_string_value(string* s);
    
    void
    mk_typesVec_node();
	
    void
    assign_new_ids();
}

#endif /* _AST_UTILS_H */

/*----------------------------------------------------------------------------*/
