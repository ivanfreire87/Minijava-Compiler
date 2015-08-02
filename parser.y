%{
#include "includes.hh"
#include "symtab.hh"
#include "ast.hh"
#include "ast_utils.hh"
#include "utils.hh"

using namespace std;
using namespace ASTUtils;

int yyerror(string s);
int yylex(void);

%}
%union{
  int        	ival;
  char*		tval;
  string*       vval;
  ASTNode*      node;
}
%start begin

%token <vval> ID OP
%token <ival> DIGIT

%type <node> begin program mainClass formalList formalRest statements statement exp
%type <node> operand unary primary methodDecl methodDecls classDecl classDecls varDecl varDecls
%type <tval> type

%token PUBLIC STATIC VOID MAIN CLASS EXTENDS STRING BOOLEAN TRUE FALSE IF ELSE WHILE LENGTH THIS NEW PRINT RETURN COMMA 
%token SEMICOLON OBRACES CBRACES OPAR CPAR OSQUAREPAR CSQUAREPAR DOT EQUAL EXCL READERS INTREADER READINT INT 

%left OP
%right EXCL
%error-verbose

%%

begin:	                						{ push_scope(); }
		program						        { set_root_node($2); mk_typesVec_node(); }

program: 	mainClass classDecls			                { $$ = mk_program_node($1,$2); assign_new_ids();}		
	;
		
mainClass: 	CLASS ID OBRACES PUBLIC STATIC VOID MAIN OPAR 
		STRING OSQUAREPAR CSQUAREPAR ID CPAR OBRACES 		
		statement CBRACES CBRACES 			        { $$ = mk_main_node($2,$15); }
	;
		
classDecls:	classDecls classDecl			                { $$ = mk_classes_node($2,$1); }
	|							        { $$ = NULL; }										
	;

classDecl:	CLASS ID OBRACES 				        { set_current_class($2); mk_cref($2); } 
		varDecls methodDecls CBRACES			        { unset_current_class(); $$ = mk_class_node($2,$5,$6);}
	|	CLASS ID EXTENDS ID OBRACES 			        { set_current_class($2); mk_cref($2,$4); } 
		varDecls methodDecls CBRACES 			        { unset_current_class(); $$ = mk_class_node($2,$7,$8,$4); }
	;
		
varDecls:	varDecls varDecl					{ $$ = mk_varDecls_node($2, $1); }	
        |								{ $$ = NULL; }									
	;
		
varDecl:	type ID semicolon					{ $$ = mk_vref_node($2, $1); }
	;		
		
methodDecls:	methodDecl methodDecls					{ $$ = mk_methods_node($1,$2); }
	|								{ $$ = NULL; }
	;
		
methodDecl:	PUBLIC type ID OPAR 					{ set_current_method($3); mk_mref($3,$2); set_reading_params(); } 	
		formalList						{ unset_reading_params(); }
		CPAR OBRACES 											
		varDecls statements RETURN exp SEMICOLON CBRACES 	{ unset_current_method(); $$ = mk_method_node($2,$3,$10,$11,$13); }	
	;
		
formalList:	type ID formalRest					{ mk_vref_node($2, $1); }
	|								{ $$ = NULL;  }
	;
		
formalRest:	COMMA type ID formalRest				{ mk_vref_node($3, $2); }
	|								{ $$ = NULL; }
	;
		
type:		INT OSQUAREPAR CSQUAREPAR   				{ $$ = "int[]"; }
	|	BOOLEAN			        			{ $$ = "boolean"; }
	|	INT							{ $$ = "int"; }
	|	ID		        				{ $$ = get_string_value($1); }
	;
				
statements:	statement statements 					{ $$ = mk_stmts_node($1, $2); }
	|								{ $$ = NULL; }
	;
		
statement:	OBRACES statements CBRACES				{ $$ = $2; }
	|	OBRACES error CBRACES 					{ yyerrok; yyclearin; }
	|	IF OPAR exp CPAR statement ELSE statement  		{ $$ = mk_istmt_node($3,$5,$7); }
	|	WHILE OPAR exp CPAR statement				{ $$ = mk_wstmt_node($3,$5); }
	|	PRINT OPAR exp CPAR semicolon				{ $$ = mk_pstmt_node($3); }
	|	ID EQUAL exp semicolon					{ $$ = mk_astmt_node($1,$3); }
	|	ID OSQUAREPAR exp CSQUAREPAR EQUAL exp semicolon 	{ $$ = mk_aivecstmt_node($1,$6,$3); }
	;
		
exp:		unary 							{ $$ = $1; }
	|	exp OP exp						{ $$ = mk_opexpr_node($2,$1,$3); }
	;
		
operand:	READERS DOT INTREADER DOT READINT OPAR CPAR 		{ $$ = NULL; }
	|	DIGIT 							{ $$ = mk_iconst_node($1); }
	|	TRUE 							{ $$ = mk_bconst_node(true); }
	|	FALSE 							{ $$ = mk_bconst_node(false); }
	|	ID 							{ $$ = get_vref_node($1); } 
	|	THIS 							{ $$ = get_current_class_node(); }
	|	NEW INT OSQUAREPAR exp CSQUAREPAR  			{ $$ = mk_int_vec_node($4); }
	|	NEW ID OPAR CPAR 					{ $$ = mk_newId($2); }
	|	OPAR exp CPAR 						{ $$ = $2; }
	;
		
primary:	operand 						{ $$ = $1; }
	|	primary OSQUAREPAR exp CSQUAREPAR 			{ $$ = mk_vec_value_node($1,$3); }
	|	primary DOT LENGTH					{ $$ = mk_length_node($1); }
	|	primary DOT ID OPAR 					{ set_assigning_method_params($1,$3); }
		expList CPAR						{ unset_assigning_method_params(); $$ = mk_call_method_node($1,$3); }
	;		
		
unary:		primary 						{ $$ = $1; }
	|	EXCL unary 						{ $$ = mk_negation_node($2); }
	;
	
expList:	exp expRest						{ mk_param_value($1); }
	|   						
	;
		
expRest: 	COMMA exp expRest					{ mk_param_value($2);}
	|  															
	;
		
semicolon:	SEMICOLON
	|	error SEMICOLON						{ yyerrok; yyclearin; }	
	;		

%%
int yyerror(string s){
	extern int yylineno;  
	extern char yytext[];
	cout << "error: " << s << " -> " << yytext << " @ line " << yylineno << endl;
}
